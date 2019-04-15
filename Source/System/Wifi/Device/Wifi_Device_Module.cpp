#define WIFI_IMPLEMENTATION
#include "Wifi_Device_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_Device_UpdateInterface.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_Client.h"

// Checks the initial Wifi device state.
Wifi::Device::Module::Module(Resource& parentResource) :
    Wifi::Module(parentResource) { }


// Checks if a Wifi device managed by NetworkManager exists.
bool Wifi::Device::Module::wifiDeviceExists() const
{
    return deviceExists;
}


// Checks if the wifi device is enabled.
bool Wifi::Device::Module::wifiDeviceEnabled() const
{
    return deviceExists && deviceEnabled;
}


// Checks if wifi is currently being enabled or disabled.
bool Wifi::Device::Module::isDeviceStateChanging() const
{
    return stateChanging;
}


// Connects to NetworkManager to update the Wifi device state, notifying all
// DeviceListeners if the state changes.
void Wifi::Device::Module::updateDeviceState(const bool notifyListeners)
{
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, notifyListeners, &nmThread]()
    {
        LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
        bool exists = !wifiDevice.isNull();
        LibNM::Client client = nmThread->getClient();
        bool enabled = client.wirelessEnabled();
        updateDeviceState(exists, enabled, notifyListeners);
    });
}


// Updates the Device::Module's saved wifi device state, notifying all
// DeviceListeners if the state changes.
void Wifi::Device::Module::updateDeviceState
(const bool exists, const bool enabled, const bool notifyListeners)
{
    stateChanging = false;
    const bool wasEnabled = wifiDeviceEnabled();
    deviceExists = exists;
    deviceEnabled = enabled;
    const bool isEnabled = wifiDeviceEnabled();
    if (isEnabled != wasEnabled && notifyListeners)
    {
        foreachModuleHandler<UpdateInterface>([isEnabled]
        (UpdateInterface* listener)
        {
            if (isEnabled)
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


// Notifies the device tracker that Wifi is about to be enabled or disabled.
void Wifi::Device::Module::signalDeviceStateChanging()
{
    stateChanging = true;
}
