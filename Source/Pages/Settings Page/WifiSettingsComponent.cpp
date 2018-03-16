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
    WifiStatus* wifiStatus = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    if (wifiStatus != nullptr)
    {
        wifiStatus->addListener(this);
    }
    refresh();
}

/**
 * @return true if wifi is enabled, false if disabled
 */
bool WifiSettingsComponent::connectionEnabled()
{

    WifiStatus* wifiStatus = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    if (wifiStatus == nullptr)
    {
        return false;
    }
    return wifiStatus->isEnabled();
}

/**
 * @return true iff wifi is connecting, disconnecting, turning on, or
 * turning off.
 */
bool WifiSettingsComponent::isBusy()
{
    return wifiBusy;
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
    WifiStatus* wifiStatus = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    if (wifiStatus != nullptr)
    {
        if (!enabled && wifiStatus->isEnabled())
        {
            wifiStatus->disableWifi();
        }
        else if (enabled && !wifiStatus->isEnabled())
        {
            wifiStatus->enableWifi();
        }
    }
    else
    {
        DBG("WifiSettingsComponent::" << __func__ << ": wifiStatus is null!");
    }
}

/**
 * Use wifi status updates to keep the component updated.
 */
void WifiSettingsComponent::handleWifiEvent(WifiStatus::WifiEvent event)
{
    wifiBusy = (event == WifiStatus::wifiBusy);
    refresh();
}

/**
 * @return Wifi on, Wifi off, or the active connection name. 
 */
String WifiSettingsComponent::updateButtonText()
{
    WifiStatus* status = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    if (status == nullptr || !status->isEnabled())
    {
        return "WiFi Off";
    }
    WifiAccessPoint wifiAP = status->getConnectedAccessPoint();
    if (!wifiAP.isNull())
    {
        return wifiAP.getSSID();
    }
    else
    {
        return "Not Connected";
    }
}
