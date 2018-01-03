/*
  ==============================================================================

    WifiCategoryItemComponent.cpp
    Created: 2 Jan 2018 11:31:54pm
    Author:  anthony

  ==============================================================================
*/
#include "../Main.h"
#include "WifiCategoryItemComponent.h"
WifiCategoryItemComponent::WifiCategoryItemComponent() :
SettingsCategoryItemComponent("wifi"),
spinner(new WifiSpinner("SettingsWifiSpinner"))
{
    iconDrawable =
            Drawable::createFromImageFile(assetFile("wifiIcon.png"));
    icon->setImages(iconDrawable);
    bool isEnabled = getWifiStatus().isEnabled();
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

    enabled ? getWifiStatus().setEnabled() : getWifiStatus().setDisabled();
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
    spinner->hide();
    icon->setVisible(true);

    button->setEnabled(getWifiStatus().isEnabled());
    toggle->setEnabled(true);

    updateButtonText();
    toggle->setToggleState(getWifiStatus().isEnabled(), NotificationType::dontSendNotification);
}

void WifiCategoryItemComponent::disableWifiActions()
{
    spinner->show();
    icon->setVisible(false);

    button->setEnabled(getWifiStatus().isEnabled());
    toggle->setEnabled(false);

    updateButtonText();
    toggle->setToggleState(getWifiStatus().isEnabled(), NotificationType::dontSendNotification);
}

void WifiCategoryItemComponent::updateButtonText()
{
    const auto &status = getWifiStatus();
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