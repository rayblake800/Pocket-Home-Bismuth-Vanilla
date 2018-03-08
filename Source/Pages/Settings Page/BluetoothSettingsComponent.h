/**
 * @file BluetoothSettingsComponent.h
 * 
 * TODO: documentation
 */
#pragma once
#include "ConnectionSettingsComponent.h"
class BluetoothSettingsComponent : public ConnectionSettingsComponent {
public:
    BluetoothSettingsComponent(std::function<void()> openBluetoothPage);
    virtual ~BluetoothSettingsComponent();

    void enabledStateChanged(bool enabled) override;
    void updateButtonText() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluetoothSettingsComponent)
};