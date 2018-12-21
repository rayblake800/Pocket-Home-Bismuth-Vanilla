#define WIFI_IMPLEMENTATION
#include "Wifi/Connection/Listener.h"
#include "Wifi/Connection/RecordResource.h"
#include "Wifi/AccessPoint/AccessPoint.h"

namespace WifiConnect = Wifi::Connection;

WifiConnect::Listener::Listener() { }

/*
 * Signals that a new active Wifi connection is being opened.
 */
void WifiConnect::Listener::startedConnecting
(const AccessPoint connectingAP) { }

/*
 * Signals that an attempt to open a Wifi connection failed due to improper
 * authentication, usually because of an incorrect Wifi password.
 */
void WifiConnect::Listener::connectionAuthFailed
(const AccessPoint connectingAP) { }

/*
 * Signals that a new Wifi connection was opened successfully.
 */
void WifiConnect::Listener::connected(const AccessPoint connectedAP) { }

/*
 * Signals that an active Wifi connection was closed.
 */
void WifiConnect::Listener::disconnected(const AccessPoint disconnectedAP) { }
