#pragma once
#include "Locale/TextUser.h"
#include "WifiStateManager.h"
#include "ConnectionSettingsComponent.h"

/**
 * @file  WifiSettingsComponent.h
 * 
 * @brief  Shows wifi state, lets the user enable or disable wifi, and opens the
 *         wifi settings page.
 */

class WifiSettingsComponent : public ConnectionSettingsComponent,
public WifiStateManager::Listener, public Locale::TextUser
{
public:
    /**
     * @param openWifiPage   Callback function that opens the wifi page.
     */
    WifiSettingsComponent(std::function<void() > openWifiPage);

    virtual ~WifiSettingsComponent() { }
private:
    /**
     * @brief  Checks if wifi is currently turned on.
     * 
     * @return  True if wifi is enabled, false if disabled.
     */
    bool connectionEnabled() override;
       
    /**
     * @brief  Determines if the loading spinner should be shown.
     * 
     * @return  True whenever wifi is in a transition state (connecting,
     *          disconnecting, etc.), and the spinner should be visible.
     */
    bool shouldShowSpinner() override;

    /**
     * @brief  Determines if the connection switch should be enabled.
     * 
     * @return  True iff the wifi device is present and isn't being turned on or
     *          off.
     */
    bool allowConnectionToggle() override;

    
    /**
     * @brief  Determines if the connection page should be accessible.
     * 
     * @return  True whenever wifi is enabled and not being disabled.
     */
    bool connectionPageAvailable() override;

    /**
     * @brief  Gets the wifi icon path.
     * 
     * @return  The wifi icon path.
     */
    juce::String getIconAsset() override;

    /**
     * Enable or disable the wifi radio.
     * 
     * @param enabled   This sets which action to take.
     */
    void enabledStateChanged(bool enabled) override;

    /**
     * Sets the wifi button text based on the current wifi state.
     */
    juce::String updateButtonText() override;

    /**
     * Use wifi status updates to keep the component updated.
     * 
     * @param state
     */
    void wifiStateChanged(const WifiState state) override;

    //if true, show the loading spinner and disable controls.
    bool wifiBusy = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsComponent)
};
