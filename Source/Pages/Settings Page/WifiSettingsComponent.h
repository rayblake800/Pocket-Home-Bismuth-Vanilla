/**
 * @file WifiSettingsComponent.h
 * 
 *TODO:document, get rid of all the unnecessary redundancy.
 */
#pragma once
#include "WifiStateManager.h"
#include "ConnectionSettingsComponent.h"

class WifiSettingsComponent : public ConnectionSettingsComponent, 
        public WifiStateManager::Listener {
public:
    WifiSettingsComponent(
            WifiStateManager& wifiManager, 
            std::function<void()> openWifiPage,
            ComponentConfigFile& config);
    
    virtual ~WifiSettingsComponent() {}
private:
    /**
     * @return true if wifi is enabled, false if disabled
     */
    bool connectionEnabled() override;
    
    /**
     * @return true iff wifi is connecting, disconnecting, turning on, or
     * turning off.
     */
    bool isBusy() override;
    
    
    /**
     * @return the wifi icon
     */
    String getIconAsset() override;
    
    /**
     * Enable or disable the wifi radio.
     * @param enabled sets which action to take.
     */
    void enabledStateChanged(bool enabled) override;
    
    /**
     */
    String updateButtonText() override;

    /**
     * Use wifi status updates to keep the component updated.
     * @param event
     */
    void wifiStateChanged(WifiStateManager::WifiState state) override;

    bool wifiBusy = false;
    
    WifiStateManager& wifiManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsComponent)
};