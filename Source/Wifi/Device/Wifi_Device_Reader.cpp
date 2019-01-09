#define WIFI_IMPLEMENTATION
#include "Wifi_Device_Reader.h"
#include "Wifi_Device_Tracker.h"

Wifi::Device::Reader::Reader() { }

/*
 * Checks if the wifi device is enabled.
 */
bool Wifi::Device::Reader::wifiDeviceEnabled() const
{
    SharedResource::LockedPtr<Tracker> wifiDeviceTracker
        = getReadLockedResource();
    return wifiDeviceTracker->wifiDeviceEnabled();

}

/*
 * Checks if a Wifi device managed by NetworkManager exists.
 */
bool Wifi::Device::Reader::wifiDeviceExists() const
{
    SharedResource::LockedPtr<Tracker> wifiDeviceTracker
        = getReadLockedResource();
    return wifiDeviceTracker->wifiDeviceExists();
}

/*
 * Checks if wifi is currently being enabled or disabled.
 */
bool Wifi::Device::Reader::isDeviceStateChanging() const
{
    SharedResource::LockedPtr<Tracker> wifiDeviceTracker
        = getReadLockedResource();
    return wifiDeviceTracker->isDeviceStateChanging();
}
