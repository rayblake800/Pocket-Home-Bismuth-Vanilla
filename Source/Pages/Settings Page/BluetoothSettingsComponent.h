/**
 * @file BluetoothSettingsComponent.h
 * 
 * Bluetooth is not yet implemented, for now this does pretty much nothing.
 * TODO: documentation
 */
#pragma once
#include "BluetoothStatus.h"
#include "ConnectionSettingsComponent.h"

class BluetoothSettingsComponent : public ConnectionSettingsComponent
{
public:
    BluetoothSettingsComponent(
            std::function<void() > openBluetoothPage,
            ComponentConfigFile& config);

    virtual ~BluetoothSettingsComponent() { }

private:
    /**
     * @return true iff bluetooth is turned on
     */
    bool connectionEnabled() override;

    /**
     * @return true iff bluetooth is connecting
     */
    bool isBusy() override;

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