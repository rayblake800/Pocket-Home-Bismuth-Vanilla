#define WIFI_IMPLEMENTATION
#include "Wifi_Device_Listener.h"
#include "Wifi_Device_Module.h"
#include "Wifi_Resource.h"

Wifi::Device::Listener::Listener() { }


// Checks if a Wifi device managed by NetworkManager exists.
bool Wifi::Device::Listener::wifiDeviceExists() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> deviceModule
            = getReadLockedResource();
    return deviceModule->wifiDeviceExists();
}


// Checks if the managed Wifi device is currently enabled.
bool Wifi::Device::Listener::wifiDeviceEnabled() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> deviceModule
            = getReadLockedResource();
    return deviceModule->wifiDeviceEnabled();
}
