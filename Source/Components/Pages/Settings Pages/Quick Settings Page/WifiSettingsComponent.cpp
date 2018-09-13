#include "PocketHomeApplication.h"
#include "WifiSettingsComponent.h"

/* Localized text keys: */
static const constexpr char * wifi_not_found = "wifi_not_found";
static const constexpr char * wifi_disabled = "wifi_disabled";
static const constexpr char * wifi_turning_on = "wifi_turning_on";
static const constexpr char * not_connected = "not_connected";
static const constexpr char * wifi_turning_off = "wifi_turning_off";
static const constexpr char * connecting_to_ap = "connecting_to_ap";
static const constexpr char * connecting_to_unknown
        = "connecting_to_unknown";   
static const constexpr char * missing_psk = "missing_psk";
static const constexpr char * disconnecting = "disconnecting";
    
WifiSettingsComponent::WifiSettingsComponent
(std::function<void() > openWifiPage) :
ConnectionSettingsComponent(openWifiPage, "wifi"),
Localized("WifiSettingsComponent")
{
#    if JUCE_DEBUG
    setName("WifiSettingsComponent");
#    endif
    refresh();
}

/*
 * Checks if wifi is currently turned on.
 */
bool WifiSettingsComponent::connectionEnabled()
{
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
        case WifiState::turningOn:
        case WifiState::enabled:
        case WifiState::connecting:
        case WifiState::missingPassword:
        case WifiState::connected:
        case WifiState::disconnecting:
            return true;
        default:
            return false;
    }
    return false;
}

/*
 * Determines if the loading spinner should be shown.
 */
bool WifiSettingsComponent::shouldShowSpinner() 
{
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
        case WifiState::turningOn:
        case WifiState::turningOff:
        case WifiState::connecting:
        case WifiState::disconnecting:
        case WifiState::missingPassword:
            return true;
        default:
            return false;
    }
    return false;
}

/*
 * Determines if the connection switch should be enabled.
 */
bool WifiSettingsComponent::allowConnectionToggle()
{
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
        case WifiState::turningOn:
        case WifiState::turningOff:
            return false;
        default:
            return true;
    }
    return true;
}

/*
 * Determines if the connection page should be accessible.
 */
bool WifiSettingsComponent::connectionPageAvailable() 
{
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
        case WifiState::turningOn:
        case WifiState::turningOff:
        case WifiState::missingNetworkDevice:
        case WifiState::disabled:
            return false;
        default:
            return true;
    }
    return true;
}


/*
 * Gets the wifi icon path.
 */
juce::String WifiSettingsComponent::getIconAsset()
{
    return "wifiIcon.svg";
}

/**
 * Enable or disable the wifi radio.
 */
void WifiSettingsComponent::enabledStateChanged(bool enabled)
{
    WifiStateManager wifiManager;
    if (enabled)
    {
        wifiManager.enableWifi();
    }
    else
    {
        wifiManager.disableWifi();
    }
}

/**
 * Sets the wifi button text based on the current wifi state.
 */
juce::String WifiSettingsComponent::updateButtonText()
{
    using namespace juce;
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
        case WifiState::missingNetworkDevice:
            return localeText(wifi_not_found);
        case WifiState::disabled:
            return localeText(wifi_disabled);
        case WifiState::turningOn:
            return localeText(wifi_turning_on);
        case WifiState::enabled:
            return localeText(not_connected);
        case WifiState::turningOff:
            return localeText(wifi_turning_off);
        case WifiState::connecting:
        {
            WifiAccessPoint ap = wifiManager.getActiveAP();
            if (ap.isNull())
            {
                DBG("WifiSettingsComponent::" << __func__ << ": wifi is "
                        << "connecting, but can't get the connecting AP.");
                return localeText(connecting_to_unknown);
            }
            return String(localeText(connecting_to_ap)) + ap.getSSID();
        }
        case WifiState::missingPassword:
            return localeText(missing_psk);
        case WifiState::connected:
        {
            WifiAccessPoint ap = wifiManager.getActiveAP();
            if(ap.isNull())
            {
                return "Error: connected AP missing!";
            }
            return ap.getSSID();
        }
        case WifiState::disconnecting:
            return localeText(disconnecting);
        default:
            return "Unknown State";
    }
}

/**
 * Use wifi status updates to keep the component updated.
 */
void WifiSettingsComponent::wifiStateChanged(const WifiState state)
{
    refresh();
}

