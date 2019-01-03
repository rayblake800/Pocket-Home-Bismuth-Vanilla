#define WIFI_IMPLEMENTATION
#include "Wifi/Connection/Controller.h"
#include "Wifi/Connection/RecordReader.h"
#include "Wifi/Connection/RecordWriter.h"
#include "Wifi/Connection/Event.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPointList/APListReader.h"
#include "Wifi/AccessPointList/NMAPListReader.h"
#include "LibNM/ContextTest.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/Data/SSID.h"
#include "LibNM/Data/SecurityType.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "LibNM/OwnedObjects/Connection.h"
#include "GLib/SmartPointers/ErrorPtr.h"

namespace WifiConnect = Wifi::Connection;

/*
 * Attempts to open a Wifi network connection point.
 */
void WifiConnect::Controller::connectToAccessPoint
(const AccessPoint toConnect, juce::String securityKey) const
{
    if(toConnect.isNull())
    {
        DBG("WifiConnect::Controller::" << __func__ 
                << ": Attempted to connect to null AP.");
        return;
    }

    if(toConnect == RecordReader().getActiveAP())
    {
        DBG("WifiConnect::Controller::" << __func__ 
                << ": Access point is already connected or connecting.");
        return;
    }

    const LibNM::ThreadHandler nmThread;
    nmThread.call([this, &nmThread, &toConnect, &securityKey]()
    {
        const NMAPListReader nmAPReader;
        const RecordReader connectionRecord;
        const LibNM::AccessPoint& nmAccessPoint 
                = nmAPReader.getStrongestNMAccessPoint(toConnect);
        if(nmAccessPoint.isNull())
        {
            DBG("WifiConnect::Controller::connectToAccessPoint: "
                    << "Failed to find a matching LibNM access point.");
            return;
        }
        LibNM::DeviceWifi wifiDevice = nmThread.getWifiDevice();
        if(wifiDevice.getActiveAccessPoint() == nmAccessPoint)
        {
            DBG("WifiConnect::Controller::connectToAccessPoint: "
                    << ": Access point is already connected or connecting.");
            return;
        }

        // Create connection object:
        LibNM::Connection newConnection;
        newConnection.addWifiSettings(toConnect.getSSID());
        const bool foundSavedConnection = connectionRecord.hasSavedConnection
            (toConnect);

        // Add security settings if necessary:
        using WifiSecurity = LibNM::SecurityType;
        LibNM::SecurityType securityType = toConnect.getSecurityType();
        if(securityKey.isNotEmpty() 
                && !foundSavedConnection
                && securityType != WifiSecurity::unsecured)
        {
            bool pskAdded = (securityType == WifiSecurity::securedWEP) ?
                newConnection.addWEPSettings(securityKey) :
                newConnection.addWPASettings(securityKey);
            if(!pskAdded)
            {
                DBG("WifiConnect::Controller::connectToAccessPoint: "
                        << "Invalid PSK, couldn't set security settings.");
                RecordWriter connectionRecordWriter;
                connectionRecordWriter.addConnectionEvent(
                        Event(toConnect, EventType::connectionAuthFailed));
                return;
            }
        }

        // Signal that a connection attempt is about to happen:
        RecordWriter connectionRecordWriter;
        connectionRecordWriter.addConnectionEvent(
                Event(toConnect, EventType::connectionRequested));

        // Have the Client activate the connection:
        LibNM::Client networkClient = nmThread.getClient();
        networkClient.activateConnection(
                newConnection,
                wifiDevice,
                nmAccessPoint,
                (LibNM::Client::ConnectionHandler*) this,
                foundSavedConnection);
    });
}

/*
 * Disconnects the active Wifi connection. If there is no active wifi
 * connection, no action is taken.
 */
void WifiConnect::Controller::disconnect() const
{
    const LibNM::ThreadHandler nmThread;
    nmThread.call([this, &nmThread]()
    {
        LibNM::DeviceWifi wifiDevice = nmThread.getWifiDevice();
        if(!wifiDevice.getActiveConnection().isNull())
        {
            wifiDevice.disconnect();
        }
    });
}

/*
 * Discards all saved connection data linked to an access point.
 */
void WifiConnect::Controller::removeSavedConnection
(const AccessPoint toRemove) const
{
    const RecordWriter connectionRecordWriter;
    connectionRecordWriter.removeSavedConnection(toRemove);
}

/**
 * @brief  Gets the LibNM::AccessPoint object used to establish an active 
 *         connection.
 *
 * @param connection  An active connection object.
 *
 * @return            The LibNM access point used to activate the connection, or
 *                    a null access point if the connection is null.
 */
static LibNM::AccessPoint getActiveConnectionNMAP
(const LibNM::ActiveConnection connection)
{
    ASSERT_NM_CONTEXT;
    const LibNM::ThreadHandler nmThread;
    const LibNM::DeviceWifi wifiDevice = nmThread.getWifiDevice();
    return wifiDevice.getAccessPoint(connection.getAccessPointPath());
}

/**
 * @brief  Gets the Wifi::AccessPoint object used to establish an active 
 *         connection.
 *
 * @param connection  An active connection object.
 *
 * @return            The access point used to activate the connection, or a 
 *                    null access point if the connection is null.
 */
static Wifi::AccessPoint getActiveConnectionAP
(const LibNM::ActiveConnection connection)
{
    const LibNM::AccessPoint nmAP = getActiveConnectionNMAP(connection);
    const Wifi::APListReader apListReader;
    return apListReader.getAccessPoint(nmAP.generateHash());
}

/*
 * Signals that a connection is being opened.
 */
void WifiConnect::Controller::openingConnection
(LibNM::ActiveConnection connection)
{
    ASSERT_NM_CONTEXT;
    const AccessPoint connectionAP = getActiveConnectionAP(connection);
    const RecordReader connectionRecordReader;
    Event latestEvent = connectionRecordReader.getLastEvent();
    if(latestEvent.getEventAP() == connectionAP
            && latestEvent.getEventType() == EventType::startedConnecting)
    {
        DBG("Wifi::Connection::Controller::openingConnection: "
                << "NDeviceSignalHandler already logged connection attempt");
    }
    else
    {
        DBG("Wifi::Connection::Controller::openingConnection: "
                << "Logging new connection attempt.");
        const RecordWriter connectionRecordWriter;
        connectionRecordWriter.addConnectionEvent(Event(connectionAP, 
                    EventType::startedConnecting));
    }
}

/*
 * Signals that an attempt to open a connection failed.
 */
void WifiConnect::Controller::openingConnectionFailed
(LibNM::ActiveConnection connection, GError* error)
{
    ASSERT_NM_CONTEXT;
    GLib::ErrorPtr connectionError(error);
    DBG("WifiConnect::Controller::openingConnectionFailed" << ": Error "
                << error->code << ":" << error->message);

    const AccessPoint connectionAP = getActiveConnectionAP(connection);
    const RecordReader connectionRecordReader;
    const RecordWriter connectionRecordWriter;

    if(connectionRecordReader.hasSavedConnection(connectionAP))
    {
        DBG("WifiConnect::Controller::openingConnectionFailed" 
                << ": Deleting new saved connection " << connection.getID());
        connectionRecordWriter.removeSavedConnection(connectionAP);
    }
    else
    {
       DBG("WifiConnect::Controller::openingConnectionFailed" 
               << ": Previously established connection " << connection.getID()
               << " failed to activate fully.");

       //TODO: Try other compatible LibNM::AccessPoint objects
    }

    EventType connectionEvent;
    if(connectionAP.getSecurityType() == LibNM::SecurityType::unsecured)
    {
        connectionEvent = EventType::connectionAuthFailed;
    }
    else
    {
        connectionEvent = EventType::disconnected;
    }
    connectionRecordWriter.addConnectionEvent(Event(connectionAP,
                connectionEvent));
}
