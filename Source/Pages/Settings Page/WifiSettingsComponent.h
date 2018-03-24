/**
 * @file WifiSettingsComponent.h
 * 
 * WifiSettingsComponent shows wifi state, lets the user enable or disable wifi,
 * and opens the wifi settings page.
 */
#pragma once
#include "Localized.h"
#include "WifiStateManager.h"
#include "ConnectionSettingsComponent.h"

class WifiSettingsComponent : public ConnectionSettingsComponent,
public WifiStateManager::Listener, private Localized
{
public:
    /**
     * @param wifiManager    The component's source of wifi data, shared
     *                        between all objects tracking wifi.
     * 
     * @param openWifiPage   Callback function that opens the wifi page.
     * 
     * @param config         Shared UI component preference data.
     */
    WifiSettingsComponent(
            WifiStateManager& wifiManager,
            std::function<void() > openWifiPage,
            ComponentConfigFile& config);

    virtual ~WifiSettingsComponent() { }
private:
    /**
     * Checks if wifi is currently turned on.
     * 
     * @return true if wifi is enabled, false if disabled.
     */
    bool connectionEnabled() override;

    /**
     * Checks if the wifi device is currently busy.
     * 
     * @return true iff wifi is connecting, disconnecting, turning on, or
     *          turning off.
     */
    bool isBusy() override;

    /**
     * @return the wifi icon
     */
    String getIconAsset() override;

    /**
     * Enable or disable the wifi radio.
     * 
     * @param enabled   This sets which action to take.
     */
    void enabledStateChanged(bool enabled) override;

    /**
     * Sets the wifi button text based on the current wifi state.
     */
    String updateButtonText() override;

    /**
     * Use wifi status updates to keep the component updated.
     * 
     * @param state
     */
    void wifiStateChanged(WifiStateManager::WifiState state) override;

    //if true, show the loading spinner and disable controls.
    bool wifiBusy = false;
    //Used to enable and disable wifi
    WifiStateManager& wifiManager;


    //localized text keys
    static const constexpr char * wifi_not_found = "wifi_not_found";
    static const constexpr char * wifi_disabled = "wifi_disabled";
    static const constexpr char * wifi_turning_on = "wifi_turning_on";
    static const constexpr char * not_connected = "not_connected";
    static const constexpr char * wifi_turning_off = "wifi_turning_off";
    static const constexpr char * connecting_to_ap = "connecting_to_ap";
    static const constexpr char * connecting_to_unknown
            = "connecting_to_unknown";
    static const constexpr char * disconnecting = "disconnecting";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsComponent)
};