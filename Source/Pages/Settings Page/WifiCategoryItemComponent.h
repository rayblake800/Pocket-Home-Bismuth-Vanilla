/**
 * @file WifiCategoryItemComponent.h
 * 
 *TODO:document, get rid of all the unnecessary redundancy.
 */
#pragma once

#include "../../Wifi/WifiStatus.h"
#include "SettingsCategoryItemComponent.h"
class WifiCategoryItemComponent : public SettingsCategoryItemComponent, 
        public WifiStatus::Listener {
public:
    WifiCategoryItemComponent();

    void resized() override;
    void enabledStateChanged(bool enabled) override;
    void updateButtonText() override;

    void handleWifiEnabled() override;
    void handleWifiDisabled() override;
    void handleWifiConnected() override;
    void handleWifiDisconnected() override;
    void handleWifiFailedConnect() override;
    void handleWifiBusy() override;

private:
    void enableWifiActions();
    void disableWifiActions();
    ScopedPointer<WifiSpinner> spinner;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiCategoryItemComponent)
};