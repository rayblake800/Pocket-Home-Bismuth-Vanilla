#include "Wifi/Connection/ConnectionListener.h"

Wifi::ConnectionListener::~ConnectionListener()
{
}

/*
 * Signals that a new active Wifi connection is being opened.
 */
void Wifi::ConnectionListener::startedConnecting
(const AccessPoint connectingAP)
{
}

/*
 * Signals that an attempt to open a Wifi connection failed due to improper
 * authentication, usually because of an incorrect Wifi password.
 */
void Wifi::ConnectionListener::connectionAuthFailed
(const AccessPoint connectingAP)
{
}

/*
 * Signals that a new Wifi connection was opened successfully.
 */
void Wifi::ConnectionListener::connected(const AccessPoint connectedAP)
{
}

/*
 * Signals that an active Wifi connection was closed.
 */
void Wifi::ConnectionListener::disconnected(const AccessPoint disconnectedAP)
{
}