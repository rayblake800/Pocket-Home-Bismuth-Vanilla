/*
  ==============================================================================

    BluetoothCategoryItemComponent.h
    Created: 2 Jan 2018 11:32:09pm
    Author:  anthony

  ==============================================================================
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