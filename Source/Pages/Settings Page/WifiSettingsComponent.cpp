#include "PocketHomeApplication.h"
#include "WifiSettingsComponent.h"

WifiSettingsComponent::WifiSettingsComponent
(std::function<void() > openWifiPage) :
ConnectionSettingsComponent(openWifiPage, "wifi"),
Localized("WifiSettingsComponent")
{
#    if JUCE_DEBUG
    setName("WifiSettingsComponent");
#    endif
    WifiStateManager wifiManager;
    wifiManager.addListener(this);
    refresh();
}

/**
 * Checks if wifi is currently turned on.
 */
bool WifiSettingsComponent::connectionEnabled()
{
    WifiStateManager wifiManager;
    return wifiManager.isEnabled();
}

/**
 * Checks if the wifi device is currently busy.
 */
bool WifiSettingsComponent::isBusy()
{
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
        case WifiStateManager::turningOn:
        case WifiStateManager::turningOff:
        case WifiStateManager::connecting:
        case WifiStateManager::disconnecting:
            return true;
    }
    return false;
}

/**
 * @return the wifi icon
 */
String WifiSettingsComponent::getIconAsset()
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
String WifiSettingsComponent::updateButtonText()
{
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
        case WifiStateManager::missingNetworkDevice:
            return localeText(wifi_not_found);
        case WifiStateManager::disabled:
            return localeText(wifi_disabled);
        case WifiStateManager::turningOn:
            return localeText(wifi_turning_on);
        case WifiStateManager::enabled:
            return localeText(not_connected);
        case WifiStateManager::turningOff:
            return localeText(wifi_turning_off);
        case WifiStateManager::connecting:
        {
            WifiAccessPoint ap = wifiManager.getConnectingAP();
            if (ap.isVoid())
            {
                DBG("WifiSettingsComponent::" << __func__ << ": wifi is "
                        << "connecting, but can't get the connecting AP.");
                return localeText(connecting_to_unknown);
            }
            return String(localeText(connecting_to_ap)) + ap.getSSID();
        }
        case WifiStateManager::connected:
        {
            WifiAccessPoint ap = wifiManager.getConnectedAP();
            return ap.getSSID();
        }
        case WifiStateManager::disconnecting:
            return localeText(disconnecting);
        default:
            return "Unknown State";
    }
}

/**
 * Use wifi status updates to keep the component updated.
 */
void WifiSettingsComponent::wifiStateChanged
(WifiStateManager::WifiState state)
{
    MessageManager::callAsync([this]()
    {
        refresh();
    });
}

