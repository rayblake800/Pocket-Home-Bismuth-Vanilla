/*
  ==============================================================================

    BluetoothCategoryItemComponent.cpp
    Created: 2 Jan 2018 11:32:09pm
    Author:  anthony

  ==============================================================================
 */
#include <numeric>
#include "../PocketHomeApplication.h"
#include "../Utils.h"
#include "BluetoothCategoryItemComponent.h"

BluetoothCategoryItemComponent::BluetoothCategoryItemComponent()
: SettingsCategoryItemComponent("bluetooth")
{
    iconDrawable = Drawable::createFromImageFile(assetFile("bluetoothIcon.png"));
    icon->setImages(iconDrawable);
    updateButtonText();
}

void BluetoothCategoryItemComponent::enabledStateChanged(bool enabled)
{
    PocketHomeApplication::getInstance()->getBluetoothStatus().enabled 
            = enabled;
    button->setEnabled(enabled);
    updateButtonText();
}

void BluetoothCategoryItemComponent::updateButtonText()
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
            button->setText(std::to_string(connectedDeviceCount) + " Devices Connected");
        } else
        {
            button->setText("No Devices Connected");
        }
    } else
    {
        button->setText("Bluetooth Off");
    }
}