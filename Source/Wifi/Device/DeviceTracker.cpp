#include "Wifi/Device/DeviceTracker.h"
#include "Wifi/Device/DeviceUpdateInterface.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"


/* SharedResource object instance key: */
const juce::Identifier Wifi::DeviceTracker::resourceKey = "Wifi_DeviceTracker";

/*
 * Checks the initial Wifi device state.
 */ 
Wifi::DeviceTracker::DeviceTracker() : SharedResource::Resource(resourceKey)
{
    updateDeviceState(false);
} 

/*
 * Checks if a Wifi device managed by NetworkManager exists.
 */
bool Wifi::DeviceTracker::wifiDeviceExists() const
{
    return deviceExists;
}

/*
 * Checks if the wifi device is enabled. 
 */
bool Wifi::DeviceTracker::wifiDeviceEnabled() const
{
    return deviceExists && deviceEnabled;
}

/*
 * Checks if wifi is currently being enabled or disabled.
 */
bool Wifi::DeviceTracker::isDeviceStateChanging() const
{
    return stateChanging;
}

/*
 * Connects to NetworkManager to update the Wifi device state, notifying all 
 * DeviceListeners if the state changes.
 */
void Wifi::DeviceTracker::updateDeviceState(const bool notifyListeners)
{
    bool exists = deviceExists;
    bool enabled = deviceEnabled;
    LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler, &exists, &enabled]()
    {
        LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        exists = !wifiDevice.isNull();
        LibNM::Client client = nmThreadHandler.getClient();
        enabled = client.wirelessEnabled();
    });
    updateDeviceState(exists, enabled);
}

/*
 * Updates the DeviceTracker's saved wifi device state, notifying all 
 * DeviceListeners if the state changes.
 */
void Wifi::DeviceTracker::updateDeviceState
(const bool exists, const bool enabled)
{
    stateChanging = false;
    const bool wasEnabled = wifiDeviceEnabled();
    deviceExists = exists;
    deviceEnabled = enabled;
    const bool isEnabled = wifiDeviceEnabled();
    if(isEnabled != wasEnabled)
    {
        foreachHandler<DeviceUpdateInterface>([isEnabled]
            (DeviceUpdateInterface* listener)
        {
            if(isEnabled)
            {
                listener->wirelessEnabled();
            }
            else
            {
                listener->wirelessDisabled();
            }
        });
    }
}
    
/*
 * Notifies the device tracker that Wifi is about to be enabled or disabled.
 */
void Wifi::DeviceTracker::signalDeviceStateChanging()
{
    stateChanging = true;
}

