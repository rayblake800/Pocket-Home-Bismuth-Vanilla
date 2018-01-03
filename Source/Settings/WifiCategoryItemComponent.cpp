/*
  ==============================================================================

    WifiCategoryItemComponent.cpp
    Created: 2 Jan 2018 11:31:54pm
    Author:  anthony

  ==============================================================================
 */
#include "../PocketHomeApplication.h"
#include "WifiCategoryItemComponent.h"

WifiCategoryItemComponent::WifiCategoryItemComponent() :
SettingsCategoryItemComponent("wifi"),
spinner(new WifiSpinner("SettingsWifiSpinner"))
{
    iconDrawable =
            Drawable::createFromImageFile(assetFile("wifiIcon.png"));
    icon->setImages(iconDrawable);
    bool isEnabled = PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
    toggle->setToggleState(isEnabled, NotificationType::dontSendNotification);
    button->setEnabled(isEnabled);
    addChildComponent(spinner);
    updateButtonText();
}

void WifiCategoryItemComponent::resized()
{
    SettingsCategoryItemComponent::resized();
    const auto& sb = icon->getBoundsInParent();
    spinner->setBoundsToFit(sb.getX(), sb.getY(), sb.getWidth(), sb.getHeight(), Justification::centred, true);
}

void WifiCategoryItemComponent::enabledStateChanged(bool enabled)
{
    updateButtonText();
    WifiStatus& wifiStatus = PocketHomeApplication::getInstance()
            ->getWifiStatus();
            
    enabled ? wifiStatus.setEnabled() : wifiStatus.setDisabled();
}

void WifiCategoryItemComponent::handleWifiEnabled()
{
    enableWifiActions();
}

void WifiCategoryItemComponent::handleWifiDisabled()
{
    enableWifiActions();
}

void WifiCategoryItemComponent::handleWifiConnected()
{
    enableWifiActions();
}

void WifiCategoryItemComponent::handleWifiDisconnected()
{
    enableWifiActions();
}

void WifiCategoryItemComponent::handleWifiFailedConnect()
{
    enableWifiActions();
}

void WifiCategoryItemComponent::handleWifiBusy()
{
    disableWifiActions();
}

void WifiCategoryItemComponent::enableWifiActions()
{
    
   bool wifiEnabled= PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
    spinner->hide();
    icon->setVisible(true);

    button->setEnabled(wifiEnabled);
    toggle->setEnabled(true);

    updateButtonText();
    toggle->setToggleState(wifiEnabled, NotificationType::dontSendNotification);
}

void WifiCategoryItemComponent::disableWifiActions()
{
    bool wifiEnabled= PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
    spinner->show();
    icon->setVisible(false);

    button->setEnabled(wifiEnabled);
    toggle->setEnabled(false);

    updateButtonText();
    toggle->setToggleState(wifiEnabled, NotificationType::dontSendNotification);
}

void WifiCategoryItemComponent::updateButtonText()
{
    const WifiStatus &status = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    if (status.isEnabled())
    {
        if (status.isConnected() && status.connectedAccessPoint())
        {
            button->setText(status.connectedAccessPoint()->ssid);
        } else
        {
            button->setText("Not Connected");
        }
    } else
    {
        button->setText("WiFi Off");
    }
}