#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_ModuleUpdater.h"
#include "Wifi_Connection_Control_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_LibNM_AccessPoint.h"

Wifi::Connection::Control::ModuleUpdater::ModuleUpdater() { }

/*
 * Notifies the Control::Module that a new access point was spotted, just in
 * case it's needed to establish a connection.
 */
void Wifi::Connection::Control::ModuleUpdater::signalAPAdded
(LibNM::AccessPoint newAP)
{
    SharedResource::Modular::LockedPtr<Resource, Module> controller
            = getWriteLockedResource();
    controller->signalAPAdded(newAP);
}

/*
 * Notifies the Control::Module that wireless networking was disabled.
 */
void Wifi::Connection::Control::ModuleUpdater::signalWifiDisabled()
{
    SharedResource::Modular::LockedPtr<Resource, Module> controller
            = getWriteLockedResource();
    controller->signalWifiDisabled();
}

/*
 * Notifies the Control::Module that the Wifi device state has changed.
 */
void Wifi::Connection::Control::ModuleUpdater::signalWifiStateChange
(NMDeviceState newState, NMDeviceState oldState, NMDeviceStateReason reason)
{
    SharedResource::Modular::LockedPtr<Resource, Module> controller
            = getWriteLockedResource();
    controller->signalWifiStateChange(newState, oldState, reason);
}
