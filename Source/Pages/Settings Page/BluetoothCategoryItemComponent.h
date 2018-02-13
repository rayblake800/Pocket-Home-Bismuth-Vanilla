/**
 * @file BluetoothCategoryItemComponent.h
 * 
 * TODO: documentation
 */
#pragma once
#include "SettingsCategoryItemComponent.h"
class BluetoothCategoryItemComponent : public SettingsCategoryItemComponent {
public:
    BluetoothCategoryItemComponent();

    void enabledStateChanged(bool enabled) override;
    void updateButtonText() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluetoothCategoryItemComponent)
};