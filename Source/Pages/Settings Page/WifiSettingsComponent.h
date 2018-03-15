/**
 * @file WifiSettingsComponent.h
 * 
 *TODO:document, get rid of all the unnecessary redundancy.
 */
#pragma once

#include "Spinner.h"
#include "WifiStatus.h"
#include "ConnectionSettingsComponent.h"
class WifiSettingsComponent : public ConnectionSettingsComponent, 
        public WifiStatus::Listener {
public:
    WifiSettingsComponent(std::function<void()> openWifiPage);
    virtual ~WifiSettingsComponent();

    void resized() override;
    void enabledStateChanged(bool enabled) override;
    void updateButtonText() override;

    void handleWifiEvent(WifiStatus::WifiEvent event) override;

private:
    void enableWifiActions();
    void disableWifiActions();
    Spinner spinner;
    
    /**
     * @return true if wifi is enabled, false if wifi is disabled or
     * the wifi thread hasn't been created.
     */
    bool wifiEnabled();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsComponent)
};