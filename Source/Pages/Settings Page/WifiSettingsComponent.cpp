#include "PocketHomeApplication.h"
#include "ComponentConfigFile.h"
#include "WifiSettingsComponent.h"

WifiSettingsComponent::WifiSettingsComponent
(std::function<void() > openWifiPage) :
ConnectionSettingsComponent(openWifiPage, "wifi")
{

#    if JUCE_DEBUG
    setName("WifiSettingsComponent");
#    endif
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    wifiManager.addListener(this);
    refresh();
}

/**
 * @return true if wifi is enabled, false if disabled
 */
bool WifiSettingsComponent::connectionEnabled()
{

    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    return wifiManager.isEnabled();
}

/**
 * @return true iff wifi is connecting, disconnecting, turning on, or
 * turning off.
 */
bool WifiSettingsComponent::isBusy()
{
    switch (getWifiState())
    {
        case WifiStateManager::turningOn:
        case WifiStateManager::turningOff:
        case WifiStateManager::connecting:
        case WifiStateManager::disconnecting:
        case WifiStateManager::switchingConnection:
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
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
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
 * Use wifi status updates to keep the component updated.
 */
void WifiSettingsComponent::wifiStateChanged(WifiStateManager::WifiState state)
{
    MessageManager::callAsync([this]()
    {
        refresh();
    });
}

/**
 * @return Wifi on, Wifi off, or the active connection name. 
 */
String WifiSettingsComponent::updateButtonText()
{
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    switch (wifiManager.getWifiState())
    {
        case WifiStateManager::noStateManager:
        case WifiStateManager::missingNetworkInterface:
            return "WiFi Not Found";
        case WifiStateManager::disabled:
            return "WiFi Disabled";
        case WifiStateManager::turningOn:
            return "WiFi Turning On...";
        case WifiStateManager::enabled:
            return "Not Connected";
        case WifiStateManager::turningOff:
            return "WiFi Turning Off...";
        case WifiStateManager::connecting:
        case WifiStateManager::switchingConnection:
        {
            WifiAccessPoint ap = wifiManager.getConnectingAP();
            if (ap.isNull())
            {
                DBG("WifiSettingsComponent::" << __func__ << ": wifi is "
                        << "connecting, but can't get the connecting AP.");
                return "Connecting...";
            }
            return String("Connecting to ") + ap.getSSID();
        }
        case WifiStateManager::connected:
        {
            WifiAccessPoint ap = wifiManager.getConnectedAP();
            return ap.getSSID();
        }
        case WifiStateManager::disconnecting:
            return "Disconnecting...";
    }
}
