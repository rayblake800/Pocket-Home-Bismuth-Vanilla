#include "Wifi/Device/DeviceListener.h"
#include "Wifi/Device/DeviceTracker.h"

Wifi::DeviceListener::DeviceListener() { }

/*
 * Checks if a Wifi device managed by NetworkManager exists.
 */
bool Wifi::DeviceListener::wifiDeviceExists() const
{
    SharedResource::LockedPtr<DeviceTracker> deviceTracker
        = getReadLockedResource();
    return deviceTracker->wifiDeviceExists();
}

/*
 * Checks if the managed Wifi device is currently enabled.
 */
bool Wifi::DeviceListener::wifiDeviceEnabled() const
{
    SharedResource::LockedPtr<DeviceTracker> deviceTracker
        = getReadLockedResource();
    return deviceTracker->wifiDeviceEnabled();
}
