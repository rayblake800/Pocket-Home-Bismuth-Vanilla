#include "Wifi/SavedState/DeviceTracker.h"
#include "Wifi/Listeners/UpdateInterfaces/DeviceUpdateInterface.h"
#include "LibNM/ThreadHandler.h"


/* SharedResource object instance key: */
const juce::Identifier Wifi::DeviceTracker::resourceKey = "Wifi::DeviceTracker";

/*
 * Checks the initial Wifi device state.
 */ 
Wifi::DeviceTracker::DeviceTracker() : SharedResource::Resource(resourceKey)
{
    updateDeviceState(false);
} 

/*
 * Checks if the wifi device is enabled. 
 */
bool Wifi::DeviceTracker::wifiDeviceEnabled() const
{
    return deviceExists && deviceEnabled;
}

/*
 * Checks if a Wifi device managed by NetworkManager exists.
 */
bool Wifi::DeviceTracker::wifiDeviceExists() const
{
    return deviceExists;
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
