#include <numeric>
#include "Utils.h"
#include "BluetoothSettingsPage.h"
#include "BluetoothSettingsComponent.h"

BluetoothSettingsComponent::BluetoothSettingsComponent
(std::function<void() > openBluetoothPage) :
ConnectionSettingsComponent(openBluetoothPage)
{

#    if JUCE_DEBUG
    setName("BluetoothSettingsComponent");
#    endif
    refresh();
}

/**
 * @return true iff bluetooth is turned on
 */
bool BluetoothSettingsComponent::connectionEnabled()
{
    return false;
}

/**
 * @return true iff bluetooth is connecting
 */
bool BluetoothSettingsComponent::isBusy()
{
    return false;
}

/**
 * @return the bluetooth icon asset name
 */
String BluetoothSettingsComponent::getIconAsset()
{
    return "bluetoothIcon.svg";
}

/**
 * Enable/disable bluetooth, once this program can actually do that.
 * @param enabled
 */
void BluetoothSettingsComponent::enabledStateChanged(bool enabled) { }

/**
 * @return "coming soon!"
 */
String BluetoothSettingsComponent::updateButtonText()
{
    return "coming soon!";
}