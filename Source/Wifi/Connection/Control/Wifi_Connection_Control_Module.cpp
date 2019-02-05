#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Connection_Saved_Module.h"
#include "Wifi_APList_Module.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_LibNM_SSID.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_Connection.h"
#include "Wifi_LibNM_ConnectionHandler.h"
#include "GLib_ErrorPtr.h"

#ifdef JUCE_DEBUG
// Print the complete class name before all debug output.
static const constexpr char* dbgPrefix = "Wifi::Connection::Control::Module::";
#endif

namespace WifiConnect = Wifi::Connection;

WifiConnect::Control::Module::Module(Resource& wifiResource) :
    SharedResource::Modular::Module<Resource>(wifiResource) { }

/*
 * Attempts to open a Wifi network connection point.
 */
void WifiConnect::Control::Module::connectToAccessPoint(
        const AccessPoint toConnect, 
        LibNM::ConnectionHandler* connectionHandler,
        juce::String securityKey)
{
    if(toConnect.isNull())
    {
        DBG(dbgPrefix << __func__ << ": Attempted to connect to null AP.");
        return;
    }

    const Record::Module* connectionRecord 
            = getConstSiblingModule<Record::Module>();
    if(toConnect == connectionRecord->getActiveAP())
    {
        DBG(dbgPrefix << __func__ 
                << ": Access point is already connected or connecting.");
        return;
    }

    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->callAsync(
    [this, toConnect, connectionHandler, securityKey]()
    {
        Saved::Module* savedConnections = getSiblingModule<Saved::Module>();
        // Create connection object:
        LibNM::Connection newConnection 
                = savedConnections->getSavedConnection(toConnect);
        LibNM::Thread::Module* nmThread 
                = getSiblingModule<LibNM::Thread::Module>();
        const APList::Module* apList = getConstSiblingModule<APList::Module>();
        const LibNM::AccessPoint& nmAccessPoint 
                = apList->getStrongestNMAccessPoint(toConnect);
        if(nmAccessPoint.isNull())
        {
            DBG(dbgPrefix << "connectToAccessPoint: "
                    << "Failed to find a matching LibNM access point.");
            return;
        }
        LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
        if(wifiDevice.getActiveAccessPoint() == nmAccessPoint)
        {
            DBG(dbgPrefix << "connectToAccessPoint: "
                    << ": Access point is already connected or connecting.");
            return;
        }

        const bool foundSavedConnection = !newConnection.isNull();
        
        if(!foundSavedConnection)
        {
            newConnection.addWifiSettings(toConnect.getSSID());
        }

        // Add security settings if necessary:
        using WifiSecurity = LibNM::SecurityType;
        LibNM::SecurityType securityType = toConnect.getSecurityType();
        Record::Module* connectionRecord = getSiblingModule<Record::Module>();
        if(!foundSavedConnection
                && securityKey.isNotEmpty() 
                && securityType != WifiSecurity::unsecured)
        {
            bool pskAdded = (securityType == WifiSecurity::securedWEP) ?
                newConnection.addWEPSettings(securityKey) :
                newConnection.addWPASettings(securityKey);
            if(!pskAdded)
            {
                DBG(dbgPrefix << "connectToAccessPoint: "
                        << "Invalid PSK, couldn't set security settings.");
                connectionRecord->addConnectionEvent(Event(toConnect, 
                        EventType::connectionAuthFailed));
                return;
            }
        }

        // Signal that a connection attempt is about to happen:
        connectionRecord->addConnectionEvent(
                Event(toConnect, EventType::connectionRequested));

        // Have the Client activate the connection:
        LibNM::Client networkClient = nmThread->getClient();
        networkClient.activateConnection(
                newConnection,
                wifiDevice,
                nmAccessPoint,
                connectionHandler,
                foundSavedConnection);
    });
}

/*
 * Disconnects the active Wifi connection. If there is no active wifi
 * connection, no action is taken.
 */
void WifiConnect::Control::Module::disconnect()
{
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, nmThread]()
    {
        LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
        if(!wifiDevice.getActiveConnection().isNull())
        {
            wifiDevice.disconnect();
        }
    });
}

/*
 * Signals that a connection is being opened.
 */
void WifiConnect::Control::Module::openingConnection
(LibNM::ActiveConnection connection)
{
    ASSERT_NM_CONTEXT;
    const AccessPoint connectionAP = getActiveConnectionAP(connection);
    Record::Module* record = getSiblingModule<Record::Module>();
    Event latestEvent = record->getLatestEvent();
    if(latestEvent.getEventAP() == connectionAP
            && latestEvent.getEventType() == EventType::startedConnecting)
    {
        DBG(dbgPrefix << __func__ << ": NDeviceSignalHandler already "
                << "recorded connection attempt");
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Recording new connection attempt.");
        record->addConnectionEvent(Event(connectionAP, 
                    EventType::startedConnecting));
    }
}

/*
 * Signals that an attempt to open a connection failed.
 */
void WifiConnect::Control::Module::openingConnectionFailed
(LibNM::ActiveConnection connection, GError* error)
{
    ASSERT_NM_CONTEXT;
    GLib::ErrorPtr connectionError(error);
    DBG("Wifi::Connection::Control::Module::openingConnectionFailed" 
            << ": Error " << error->code << ":" << error->message);

    const AccessPoint connectionAP = getActiveConnectionAP(connection);
    Saved::Module* savedConnections = getSiblingModule<Saved::Module>();

    if(savedConnections->hasSavedConnection(connectionAP))
    {
        DBG("Wifi::Connection::Control::Module::openingConnectionFailed" 
                << ": Deleting new saved connection " << connection.getID());
        savedConnections->removeSavedConnection(connectionAP);
    }
    else
    {
       DBG(dbgPrefix << __func__ << ": Previously established connection " 
               << connection.getID() << " failed to activate fully.");
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
    getSiblingModule<Record::Module>()->addConnectionEvent(Event(connectionAP,
                connectionEvent));
}

/*
 * Gets the LibNM::AccessPoint object used to establish an active connection.
 */
Wifi::LibNM::AccessPoint WifiConnect::Control::Module::getActiveConnectionNMAP
(const Wifi::LibNM::ActiveConnection connection)
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    const Wifi::LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
    return wifiDevice.getAccessPoint(connection.getAccessPointPath());
}

/*
 * Gets the Wifi::AccessPoint object used to establish an active connection.
 */
Wifi::AccessPoint WifiConnect::Control::Module::getActiveConnectionAP
(const Wifi::LibNM::ActiveConnection connection)
{
    const LibNM::AccessPoint nmAP = getActiveConnectionNMAP(connection);
    const APList::Module* apList = getConstSiblingModule<APList::Module>();
    return apList->getAccessPoint(nmAP.generateHash());
}

