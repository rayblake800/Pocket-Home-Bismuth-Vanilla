#define WIFI_IMPLEMENTATION
#include "Wifi_Device_Reader.h"
#include "Wifi_Device_Module.h"
#include "Wifi_Resource.h"

Wifi::Device::Reader::Reader() { }


// Checks if the wifi device is enabled.
bool Wifi::Device::Reader::wifiDeviceEnabled() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> deviceModule
            = getReadLockedResource();
    return deviceModule->wifiDeviceEnabled();

}


// Checks if a Wifi device managed by NetworkManager exists.
bool Wifi::Device::Reader::wifiDeviceExists() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> deviceModule
            = getReadLockedResource();
    return deviceModule->wifiDeviceExists();
}


// Checks if wifi is currently being enabled or disabled.
bool Wifi::Device::Reader::isDeviceStateChanging() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> deviceModule
            = getReadLockedResource();
    return deviceModule->isDeviceStateChanging();
}
