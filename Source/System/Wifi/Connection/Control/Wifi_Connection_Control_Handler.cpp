#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_Handler.h"
#include "Wifi_Connection_Control_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_ContextTest.h"

namespace WifiConnect = Wifi::Connection;

WifiConnect::Control::Handler::Handler() { }

/*
 * Attempts to open a Wifi network connection point.
 */
void WifiConnect::Control::Handler::connectToAccessPoint
(const AccessPoint toConnect, juce::String securityKey) 
{
    SharedResource::Modular::LockedPtr<Resource, Module> controller
            = getWriteLockedResource();
    controller->connectToAccessPoint(toConnect, securityKey);
}

/*
 * Disconnects the active Wifi connection. If there is no active wifi 
 * connection, no action is taken.
 */
void WifiConnect::Control::Handler::disconnect() 
{
    SharedResource::Modular::LockedPtr<Resource, Module> controller
            = getWriteLockedResource();
    controller->disconnect();
}
