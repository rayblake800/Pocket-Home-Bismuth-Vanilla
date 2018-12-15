#include "Wifi/Connection/ConnectionResource.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPointList/NMAPListReader.h"
#include "LibNM/ThreadHandler.h"

/*
 * Attempts to open a Wifi network connection point.
 */
void Wifi::ConnectionResource::connectToAccessPoint
(const AccessPoint toConnect, juce::String securityKey) const
{
    if(toConnect.isNull())
    {
        DBG("Wifi::ConnectionResource::" << __func__ 
                << ": Attempted to connect to null AP.");
        return;
    }

    if(toConnect == getActiveAP())
    {
        DBG("Wifi::ConnectionResource::" << __func__ 
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
            DBG("Wifi::ConnectionResource::connectToAccessPoint: "
                    << "Failed to find a matching LibNM access point.");
            return;
        }

        
    });
}

/*
 * Disconnects the active Wifi connection. If there is no active wifi
 * connection, no action is taken.
 */
void Wifi::ConnectionResource::disconnect() const
{
}

/*
 * Discards all saved connection data linked to an access point.
 */
void Wifi::ConnectionResource::forgetConnection
(const AccessPoint toForget) const
{
}

/*
 * Checks if Wifi is currently connected.
 */
bool Wifi::ConnectionResource::isWifiConnected() const
{
}

/*
 * Checks if Wifi is currently connecting.
 */
bool Wifi::ConnectionResource::isConnecting() const
{
}

/*
 * Gets the access point used by the current active or activating connection.
 */
Wifi::AccessPoint Wifi::ConnectionResource::getActiveAP() const
{
}

/*
 * Checks if a saved network connection exists for a particular access point.
 */
bool Wifi::ConnectionResource::hasSavedConnection(const AccessPoint& toCheck)
{
}

/*
 * Finds the last time the system was connected with a given access point.
 */
juce::Time Wifi::ConnectionResource::lastConnectionTime
(const AccessPoint& toCheck) const
{
}

/*
 * Signals that a connection is being opened.
 */
void Wifi::ConnectionResource::openingConnection
(LibNM::ActiveConnection connection, bool isNew)
{
}

/*
 * Signals that an attempt to open a connection failed.
 */
void Wifi::ConnectionResource::openingConnectionFailed
(LibNM::ActiveConnection connection, GError* error, bool isNew)
{
}
