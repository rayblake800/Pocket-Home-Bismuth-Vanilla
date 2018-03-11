#include "../../PocketHomeApplication.h"
#include "WifiSettingsComponent.h"

WifiSettingsComponent::WifiSettingsComponent
(std::function<void()> openWifiPage) :
ConnectionSettingsComponent(openWifiPage,"wifi")
{
    
#if JUCE_DEBUG
    setName("WifiSettingsComponent");
#endif
    setIcon("wifiIcon.svg");
    WifiStatus& status = 
    PocketHomeApplication::getInstance()->getWifiStatus();
    status.addListener(this);
    setPageButtonEnabled(status.isEnabled());
    setToggleState(status.isEnabled());
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
    WifiStatus& wifiStatus = PocketHomeApplication::getInstance()
            ->getWifiStatus();

    enabled ? wifiStatus.enableWifi() : wifiStatus.disableWifi();
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

    bool wifiEnabled = PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
    spinner.setVisible(false);
    setIconVisible(true);
    setPageButtonEnabled(wifiEnabled);
    updateButtonText();
    setToggleState(wifiEnabled);
}

void WifiSettingsComponent::disableWifiActions()
{
    bool wifiEnabled = PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
    spinner.setVisible(true);
    setIconVisible(false);
    setPageButtonEnabled(wifiEnabled);
    updateButtonText();
    setToggleState(wifiEnabled);
}

void WifiSettingsComponent::updateButtonText()
{
    const WifiStatus &status = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    if (status.isEnabled())
    {
        WifiAccessPoint wifiAP = status.connectedAccessPoint();
        if (wifiAP != WifiAccessPoint::null)
        {
            setPageButtonText(wifiAP.ssid);
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