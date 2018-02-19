#include <numeric>
#include "../../PocketHomeApplication.h"
#include "../../Utils.h"
#include "BluetoothSettingsPage.h"
#include "BluetoothSettingsComponent.h"

BluetoothSettingsComponent::BluetoothSettingsComponent()
: ConnectionSettingsComponent("bluetooth", new BluetoothSettingsPage())
{
    setIcon("bluetoothIcon.png");
    updateButtonText();
}

BluetoothSettingsComponent::~BluetoothSettingsComponent(){}

void BluetoothSettingsComponent::enabledStateChanged(bool enabled)
{
    PocketHomeApplication::getInstance()->getBluetoothStatus().enabled 
            = enabled;
    setPageButtonEnabled(enabled);
    updateButtonText();
}

void BluetoothSettingsComponent::updateButtonText()
{
    const auto &status = PocketHomeApplication::getInstance()
            ->getBluetoothStatus();
    if (status.enabled)
    {
        int connectedDeviceCount =
                std::accumulate(status.devices.begin(), status.devices.end(), 0,
                [](int n, BluetoothStatus::BluetoothDevice * d)
                {
                    return n + d->connected; });
        if (connectedDeviceCount > 0)
        {
            setPageButtonText(std::to_string(connectedDeviceCount) + " Devices Connected");
        } else
        {
            setPageButtonText("No Devices Connected");
        }
    } else
    {
        setPageButtonText("Bluetooth Off");
    }
}