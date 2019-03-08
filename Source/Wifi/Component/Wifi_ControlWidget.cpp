#define WIFI_IMPLEMENTATION
#include "Wifi_ControlWidget.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_Device_Reader.h"
#include "Wifi_Device_Controller.h"
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_Connection_Event.h"
#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_AssetList.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "Wifi::ControlWidget";

/* Localized text keys: */
namespace TextKey
{
    static const juce::Identifier wifiNotFound  = "wifiNotFound";
    static const juce::Identifier wifiDisabled  = "wifiDisabled";
    static const juce::Identifier notConnected  = "notConnected";
    static const juce::Identifier connectingAP  = "connectingAP";
    static const juce::Identifier missingPSK    = "missingPSK";
    static const juce::Identifier disconnecting = "disconnecting";
}
    
Wifi::ControlWidget::ControlWidget(std::function<void() > openWifiPage) :
Settings::ConnectionComponent(openWifiPage),
Locale::TextUser(localeClassKey)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}

/*
 * Checks if wifi is currently turned on.
 */
bool Wifi::ControlWidget::connectionEnabled()
{
    const Device::Reader deviceReader;
    return deviceReader.wifiDeviceEnabled();
}

/*
 * Determines if the loading spinner should be shown.
 */
bool Wifi::ControlWidget::shouldShowSpinner() 
{
    const Connection::Record::Reader recordReader;
    const Device::Reader deviceReader; 
    return recordReader.isConnecting()
            && !deviceReader.isDeviceStateChanging();
}

/*
 * Determines if the connection switch should be enabled.
 */
bool Wifi::ControlWidget::allowConnectionToggle()
{
    const Device::Reader deviceReader;
    return deviceReader.wifiDeviceExists() 
        && !deviceReader.isDeviceStateChanging();
}

/*
 * Determines if the connection page should be accessible.
 */
bool Wifi::ControlWidget::connectionPageAvailable() 
{
    const Device::Reader deviceReader;
    const Connection::Record::Reader recordReader;
    return deviceReader.wifiDeviceEnabled()
        && !deviceReader.isDeviceStateChanging()
        && !recordReader.isConnecting();
}

/*
 * Gets the wifi icon path.
 */
juce::String Wifi::ControlWidget::getIconAsset()
{
    const Theme::Image::ConfigFile imageConfig;
    return imageConfig.getWifiIconPath();
}

/**
 * Enable or disable the wifi radio.
 */
void Wifi::ControlWidget::enabledStateChanged(bool enabled)
{
    Device::Controller deviceController;
    deviceController.setEnabled(enabled);
}

/**
 * Sets the wifi button text based on the current wifi state.
 */
juce::String Wifi::ControlWidget::updateButtonText()
{
    const Device::Reader deviceReader;
    if(!deviceReader.wifiDeviceExists())
    {
        return localeText(TextKey::wifiNotFound);
    }
    if(!deviceReader.wifiDeviceEnabled())
    {
        return localeText(TextKey::wifiDisabled);
    }

    const Connection::Record::Reader recordReader;
    if(recordReader.getLatestEvent().getEventType()
            == Connection::EventType::connectionAuthFailed)
    {
        return localeText(TextKey::missingPSK);
    }

    const bool isConnected = recordReader.isConnected();
    const bool isConnecting = isConnected ? 
        false : recordReader.isConnecting();
    if(isConnected || isConnecting)
    {
        juce::String apName 
                = recordReader.getActiveAP().getSSID().toString();
        jassert(apName.isNotEmpty());
        return isConnected ? apName :
            (localeText(TextKey::connectingAP) + apName);
    }

    return localeText(TextKey::notConnected);
}

/*
 * Updates the widget whenever wireless networking is enabled.
 */
void Wifi::ControlWidget::wirelessEnabled()
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}

/*
 * Updates the widget whenever wireless networking is disabled.
 */
void Wifi::ControlWidget::wirelessDisabled()
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}

/*
 * Shows the loading spinner and updates connection text when a new connection 
 * starts.
 */
void Wifi::ControlWidget::startedConnecting(const AccessPoint connectingAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}

/*
 * Signals that a new Wifi connection was opened successfully.
 */
void Wifi::ControlWidget::connected(const AccessPoint connectedAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}

/*
 * Updates the widget when a connection disconnects, stopping the loading 
 * spinner and updating the connection text.
 */
void Wifi::ControlWidget::disconnected(const AccessPoint disconnectedAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}

/*
 * Updates the widget when a connection fails, stopping the loading spinner and
 * updating the connection text.
 */
void Wifi::ControlWidget::connectionAuthFailed(const AccessPoint connectingAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}
