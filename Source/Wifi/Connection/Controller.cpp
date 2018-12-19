#define WIFI_IMPLEMENTATION
#include "Wifi/Connection/Controller.h"
#include "Wifi/Connection/RecordReader.h"
#include "Wifi/Connection/RecordWriter.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPointList/NMAPListReader.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/NMObjects/DeviceWifi.h"

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

        LibNM::Client networkClient = nmThread.getClient();
        // Look for saved connections
        // Get connection object
        // Add psk to connection object if needed
        // Activate connection with client
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
void WifiConnect::Controller::forgetConnection
(const AccessPoint toForget) const
{
}

/*
 * Signals that a connection is being opened.
 */
void WifiConnect::Controller::openingConnection
(LibNM::ActiveConnection connection, bool isNew)
{
}

/*
 * Signals that an attempt to open a connection failed.
 */
void WifiConnect::Controller::openingConnectionFailed
(LibNM::ActiveConnection connection, GError* error, bool isNew)
{
}
