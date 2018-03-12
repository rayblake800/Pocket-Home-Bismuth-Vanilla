#include "../../PocketHomeApplication.h"
#include "WifiSettingsComponent.h"

WifiSettingsComponent::WifiSettingsComponent
(std::function<void() > openWifiPage) :
ConnectionSettingsComponent(openWifiPage, "wifi")
{

#if JUCE_DEBUG
    setName("WifiSettingsComponent");
#endif
    setIcon("wifiIcon.svg");
    WifiStatus* wifiStatus = WifiStatus::getInstance();
    if (wifiStatus == nullptr)
    {
        DBG(__func__ << ":wifi thread is null!");
        setPageButtonEnabled(false);
        setToggleState(false);
    }
    else
    {
        wifiStatus->addListener(this);
        setPageButtonEnabled(wifiStatus->isEnabled());
        setToggleState(wifiStatus->isEnabled());
    }
    addChildComponent(spinner);
    updateButtonText();
}

WifiSettingsComponent::~WifiSettingsComponent() { }

void WifiSettingsComponent::resized()
{
    ConnectionSettingsComponent::resized();
    Rectangle<int> spinnerBounds = getLocalBounds();
    spinnerBounds.setWidth(spinnerBounds.getHeight());
    spinner.setBounds(spinnerBounds);
}

void WifiSettingsComponent::enabledStateChanged(bool enabled)
{
    updateButtonText();
    WifiStatus* wifiStatus = WifiStatus::getInstance();
    if (wifiStatus != nullptr)
    {
        wifiStatus->isEnabled() ? wifiStatus->enableWifi()
                : wifiStatus->disableWifi();
    }
}

void WifiSettingsComponent::handleWifiEnabled()
{
    enableWifiActions();
}

void WifiSettingsComponent::handleWifiDisabled()
{
    enableWifiActions();
}

void WifiSettingsComponent::handleWifiConnected()
{
    enableWifiActions();
}

void WifiSettingsComponent::handleWifiDisconnected()
{
    enableWifiActions();
}

void WifiSettingsComponent::handleWifiFailedConnect()
{
    enableWifiActions();
}

void WifiSettingsComponent::handleWifiBusy()
{
    disableWifiActions();
}

void WifiSettingsComponent::enableWifiActions()
{
    bool enabled = wifiEnabled();
    spinner.setVisible(false);
    setIconVisible(true);
    setPageButtonEnabled(enabled);
    updateButtonText();
    setToggleState(enabled);
}

void WifiSettingsComponent::disableWifiActions()
{
    bool enabled = wifiEnabled();
    spinner.setVisible(true);
    setIconVisible(false);
    setPageButtonEnabled(enabled);
    updateButtonText();
    setToggleState(enabled);
}

void WifiSettingsComponent::updateButtonText()
{
    if (wifiEnabled())
    {
        WifiStatus* status = WifiStatus::getInstance();
        WifiAccessPoint wifiAP = status->connectedAccessPoint();
        if (!wifiAP.isNull())
        {
            setPageButtonText(wifiAP.getSSID());
        }
        else
        {
            setPageButtonText("Not Connected");
        }
    }
    else
    {
        setPageButtonText("WiFi Off");
    }
}

/**
 * @return true if wifi is enabled, false if wifi is disabled or
 * the wifi thread hasn't been created.
 */
bool WifiSettingsComponent::wifiEnabled()
{
    WifiStatus* status = WifiStatus::getInstance();
    if (status == nullptr)
    {
        return false;
    }
    return status->isEnabled();
}