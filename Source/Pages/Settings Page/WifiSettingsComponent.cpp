#include "../../PocketHomeApplication.h"
#include "WifiSettingsPage.h"
#include "WifiSettingsComponent.h"

WifiSettingsComponent::WifiSettingsComponent() :
ConnectionSettingsComponent("wifi", new WifiSettingsPage())
{
    setIcon("wifiIcon.svg");
    bool isEnabled = PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
    setPageButtonEnabled(isEnabled);
    setToggleState(isEnabled);
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

    enabled ? wifiStatus.setEnabled() : wifiStatus.setDisabled();
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
        if (status.isConnected())
        {
            try
            {
                setPageButtonText(status.connectedAccessPoint().ssid);
            }
            catch (WifiStatus::MissingAccessPointException e)
            {
                setPageButtonText("Not Connected");
            }
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