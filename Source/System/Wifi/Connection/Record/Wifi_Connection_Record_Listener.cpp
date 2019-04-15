#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Record_Listener.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_AccessPoint.h"

namespace WifiRecord = Wifi::Connection::Record;

WifiRecord::Listener::Listener() { }


// Signals that a new active Wifi connection is being opened.
void WifiRecord::Listener::startedConnecting
(const AccessPoint connectingAP) { }


// Signals that an attempt to open a Wifi connection failed due to improper
// authentication, usually because of an incorrect Wifi password.
void WifiRecord::Listener::connectionAuthFailed
(const AccessPoint connectingAP) { }


// Signals that a new Wifi connection was opened successfully.
void WifiRecord::Listener::connected(const AccessPoint connectedAP) { }


// Signals that an active Wifi connection was closed.
void WifiRecord::Listener::disconnected(const AccessPoint disconnectedAP) { }
