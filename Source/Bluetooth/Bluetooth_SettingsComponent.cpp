#include "Bluetooth_SettingsPage.h"
#include "Bluetooth_SettingsComponent.h"
#include "Utils.h"
#include <numeric>

Bluetooth::SettingsComponent::SettingsComponent
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
bool Bluetooth::SettingsComponent::connectionEnabled()
{
    return false;
}


/**
 * This method is used by the component to determine if it should show the
 * loading spinner.
 * 
 * @return false
 */
bool Bluetooth::SettingsComponent::shouldShowSpinner()
{
    return false;
}

/**
 * This method is used by the component to determine if the connection 
 * switch should be enabled.
 * 
 * @return false
 */
bool Bluetooth::SettingsComponent::allowConnectionToggle()
{
    return false;
}

/**
 * This method is used by the component to determine if the connection 
 * page should be accessible.
 * 
 * @return false
 */
bool Bluetooth::SettingsComponent::connectionPageAvailable()
{
    return false;
}

/**
 * @return the bluetooth icon asset name
 */
juce::String Bluetooth::SettingsComponent::getIconAsset()
{
    return "bluetoothIcon.svg";
}

/**
 * Enable/disable bluetooth, once this program can actually do that.
 * @param enabled
 */
void Bluetooth::SettingsComponent::enabledStateChanged(bool enabled) { }

/**
 * @return "coming soon!"
 */
juce::String Bluetooth::SettingsComponent::updateButtonText()
{
    return "coming soon!";
}
