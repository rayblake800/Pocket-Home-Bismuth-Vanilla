#include <numeric>
#include "Utils.h"
#include "BluetoothSettingsPage.h"
#include "BluetoothSettingsComponent.h"

BluetoothSettingsComponent::BluetoothSettingsComponent
(std::function<void() > openBluetoothPage) :
ConnectionSettingsComponent(openBluetoothPage)
{

#if JUCE_DEBUG
    setName("BluetoothSettingsComponent");
#endif
    setIcon("bluetoothIcon.svg");
    updateButtonText();
}

BluetoothSettingsComponent::~BluetoothSettingsComponent() { }

void BluetoothSettingsComponent::enabledStateChanged(bool enabled)
{
    bluetoothStatus.enabled  = enabled;
    setPageButtonEnabled(enabled);
    updateButtonText();
}

void BluetoothSettingsComponent::updateButtonText()
{
    
    if (bluetoothStatus.enabled)
    {
        int connectedDeviceCount =
                std::accumulate(bluetoothStatus.devices.begin(),
                bluetoothStatus.devices.end(), 0,
                [](int n, BluetoothDevice * d)
                {
                    return n + d->connected; });
        if (connectedDeviceCount > 0)
        {
            setPageButtonText(std::to_string(connectedDeviceCount) 
                    + " Devices Connected");
        }
        else
        {
            setPageButtonText("No Devices Connected");
        }
    }
    else
    {
        setPageButtonText("Bluetooth Off");
    }
}