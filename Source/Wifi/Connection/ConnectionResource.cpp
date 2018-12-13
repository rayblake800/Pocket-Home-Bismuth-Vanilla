#include "Wifi/Connection/ConnectionResource.h"

/*
 * Attempts to open a Wifi network connection point.
 */
void Wifi::ConnectionResource::connectToAccessPoint
        const AccessPoint toConnect,
        juce::String securityKey) const
{
}

/*
 * Disconnects the active Wifi connection. If there is no active wifi
 * connection, no action is taken.
 */
void Wifi::ConnectionResource::disconnect) const
{
}

/*
 * Discards all saved connection data linked to an access point.
 */
void Wifi::ConnectionResource::forgetConnection
        const AccessPoint toForget) const
{
}

/*
 * Checks if Wifi is currently connected.
 */
bool Wifi::ConnectionResource::isWifiConnected) const
{
}

/*
 * Checks if Wifi is currently connecting.
 */
bool Wifi::ConnectionResource::isConnecting) const
{
}

/*
 * Gets the access point used by the current active or activating connection.
 */
AccessPoint Wifi::ConnectionResource::getActiveAP) const
{
}

/*
 * Checks if a saved network connection exists for a particular access point.
 */
bool Wifi::ConnectionResource::hasSavedConnection
        const AccessPoint& toCheck)
{
}

/*
 * Finds the last time the system was connected with a given access point.
 */
juce::Time Wifi::ConnectionResource::lastConnectionTime
        const AccessPoint& toCheck) const
{
}

/*
 * Signals that a connection is being opened.
 */
void Wifi::ConnectionResource::openingConnection
        LibNM::ActiveConnection connection,
        bool isNew)
{
}

/*
 * Signals that an attempt to open a connection failed.
 */
void Wifi::ConnectionResource::openingConnectionFailed
        LibNM::ActiveConnection connection,
        GError* error,
        bool isNew)
{
}