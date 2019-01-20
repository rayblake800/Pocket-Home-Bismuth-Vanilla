#include "Wifi_Device_Listener.h"
#include "Wifi_Device_Tracker.h"

Wifi::Device::Listener::Listener() { }

/*
 * Checks if a Wifi device managed by NetworkManager exists.
 */
bool Wifi::Device::Listener::wifiDeviceExists() const
{
    SharedResource::LockedPtr<const Tracker> deviceTracker
            = getReadLockedResource();
    return deviceTracker->wifiDeviceExists();
}

/*
 * Checks if the managed Wifi device is currently enabled.
 */
bool Wifi::Device::Listener::wifiDeviceEnabled() const
{
    SharedResource::LockedPtr<const Tracker> deviceTracker
            = getReadLockedResource();
    return deviceTracker->wifiDeviceEnabled();
}
