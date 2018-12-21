#define WIFI_IMPLEMENTATION
#include "Wifi/Device/DeviceViewer.h"
#include "Wifi/Device/DeviceTracker.h"

Wifi::DeviceViewer::DeviceViewer() { }

/*
 * Checks if the wifi device is enabled.
 */
bool Wifi::DeviceViewer::wifiDeviceEnabled() const
{
    SharedResource::LockedPtr<DeviceTracker> wifiDeviceTracker
        = getReadLockedResource();
    return wifiDeviceTracker->wifiDeviceEnabled();

}

/*
 * Checks if a Wifi device managed by NetworkManager exists.
 */
bool Wifi::DeviceViewer::wifiDeviceExists() const
{
    SharedResource::LockedPtr<DeviceTracker> wifiDeviceTracker
        = getReadLockedResource();
    return wifiDeviceTracker->wifiDeviceExists();
}

/*
 * Checks if wifi is currently being enabled or disabled.
 */
bool Wifi::DeviceViewer::isDeviceStateChanging() const
{
    SharedResource::LockedPtr<DeviceTracker> wifiDeviceTracker
        = getReadLockedResource();
    return wifiDeviceTracker->isDeviceStateChanging();
}
