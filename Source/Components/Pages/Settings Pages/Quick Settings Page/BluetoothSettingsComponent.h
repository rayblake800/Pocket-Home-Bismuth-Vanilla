#pragma once
#include "BluetoothStatus.h"
#include "ConnectionSettingsComponent.h"

/**
 * @file BluetoothSettingsComponent.h
 * 
 * Bluetooth is not yet implemented, for now this does pretty much nothing.
 * TODO: documentation
 */

class BluetoothSettingsComponent : public ConnectionSettingsComponent
{
public:
    BluetoothSettingsComponent(std::function<void() > openBluetoothPage);

    virtual ~BluetoothSettingsComponent() { }

private:
    /**
     * @return true iff bluetooth is turned on
     */
    bool connectionEnabled() override;

    /**
     * This method is used by the component to determine if it should show the
     * loading spinner.
     * 
     * @return false
     */
    bool shouldShowSpinner() override;

    /**
     * This method is used by the component to determine if the connection 
     * switch should be enabled.
     * 
     * @return false
     */
    bool allowConnectionToggle() override;
  
    /**
     * This method is used by the component to determine if the connection 
     * page should be accessible.
     * 
     * @return false
     */
    bool connectionPageAvailable() override;
    
    /**
     * @return the bluetooth icon asset name
     */
    String getIconAsset() override;

    /**
     * Enable/disable bluetooth, once this program can actually do that.
     * @param enabled
     */
    void enabledStateChanged(bool enabled) override;

    /**
     * @return "coming soon!"
     */
    String updateButtonText() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluetoothSettingsComponent)
};