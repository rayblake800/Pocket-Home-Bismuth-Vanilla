#include "PocketHomeApplication.h"
#include "WifiSettingsComponent.h"


/* Localized object class key: */
static const juce::Identifier localeClassKey = "WifiSettingsComponent";

/* Localized text keys: */
static const juce::Identifier wifiNotFoundTextKey      = "wifiNotFound";
static const juce::Identifier wifiDisabledTextKey      = "wifiDisabled";
static const juce::Identifier turningOnTextKey         = "turningOn";
static const juce::Identifier notConnectedTextKey      = "notConnected";
static const juce::Identifier turningOffTextKey        = "turningOff";
static const juce::Identifier connectingAPTextKey      = "connectingAP";
static const juce::Identifier connectingUnknownTextKey = "connectingUnknown";   
static const juce::Identifier missingPSKTextKey        = "missingPSK";
static const juce::Identifier disconnectingTextKey     = "disconnecting";
    
WifiSettingsComponent::WifiSettingsComponent
(std::function<void() > openWifiPage) :
ConnectionSettingsComponent(openWifiPage, "wifi"),
Locale::TextUser(localeClassKey)
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
            return localeText(wifiNotFoundTextKey);
        case WifiState::disabled:
            return localeText(wifiDisabledTextKey);
        case WifiState::turningOn:
            return localeText(turningOnTextKey);
        case WifiState::enabled:
            return localeText(notConnectedTextKey);
        case WifiState::turningOff:
            return localeText(turningOffTextKey);
        case WifiState::connecting:
        {
            WifiAccessPoint ap = wifiManager.getActiveAP();
            if (ap.isNull())
            {
                DBG("WifiSettingsComponent::" << __func__ << ": wifi is "
                        << "connecting, but can't get the connecting AP.");
                return localeText(connectingUnknownTextKey);
            }
            return String(localeText(connectingAPTextKey)) + ap.getSSID();
        }
        case WifiState::missingPassword:
            return localeText(missingPSKTextKey);
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
            return localeText(disconnectingTextKey);
        default:
            return "Unknown State";
    }
}

/**
 * Use wifi status updates to keep the component updated.
 */
void WifiSettingsComponent::wifiStateChanged(const WifiState state)
{
    using namespace juce;
    DBG("Wifi state change");
    refresh();
}

