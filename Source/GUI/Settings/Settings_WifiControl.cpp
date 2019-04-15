#include "Settings_WifiControl.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_Device_Reader.h"
#include "Wifi_Device_Controller.h"
#include "Wifi_Connection_Record_Handler.h"
#include "Wifi_Connection_Event.h"
#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_AssetList.h"

// Localized object class key:
static const juce::Identifier localeClassKey = "Settings::WifiControl";

// Localized text keys:
namespace TextKey
{
    static const juce::Identifier wifiNotFound  = "wifiNotFound";
    static const juce::Identifier wifiDisabled  = "wifiDisabled";
    static const juce::Identifier notConnected  = "notConnected";
    static const juce::Identifier connectingAP  = "connectingAP";
    static const juce::Identifier missingPSK    = "missingPSK";
    static const juce::Identifier disconnecting = "disconnecting";
}

Settings::WifiControl::WifiControl(std::function<void()> openWifiPage) :
Settings::ConnectionComponent(openWifiPage),
Locale::TextUser(localeClassKey)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}


// Checks if Wifi is currently turned on.
bool Settings::WifiControl::connectionEnabled()
{
    const Wifi::Device::Reader deviceReader;
    return deviceReader.wifiDeviceEnabled();
}


// Determines if the loading spinner should be shown.
bool Settings::WifiControl::shouldShowSpinner()
{
    const Wifi::Connection::Record::Handler recordHandler;
    const Wifi::Device::Reader deviceReader;
    return recordHandler.isConnecting()
            && !deviceReader.isDeviceStateChanging();
}


// Determines if the connection switch should be enabled.
bool Settings::WifiControl::allowConnectionToggle()
{
    const Wifi::Device::Reader deviceReader;
    return deviceReader.wifiDeviceExists()
            && !deviceReader.isDeviceStateChanging();
}


// Determines if the connection page should be accessible.
bool Settings::WifiControl::connectionPageAvailable()
{
    const Wifi::Device::Reader deviceReader;
    const Wifi::Connection::Record::Handler recordHandler;
    return deviceReader.wifiDeviceEnabled()
            && !deviceReader.isDeviceStateChanging()
            && !recordHandler.isConnecting();
}


// Gets the Wifi icon path.
juce::String Settings::WifiControl::getIconAsset()
{
    const Theme::Image::ConfigFile imageConfig;
    return imageConfig.getWifiIconPath();
}


// Enables or disables the Wifi radio.
void Settings::WifiControl::enabledStateChanged(bool enabled)
{
    Wifi::Device::Controller deviceController;
    deviceController.setEnabled(enabled);
}


// Sets the Wifi button text based on the current wifi state.
juce::String Settings::WifiControl::updateButtonText()
{
    const Wifi::Device::Reader deviceReader;
    if (!deviceReader.wifiDeviceExists())
    {
        return localeText(TextKey::wifiNotFound);
    }
    if (!deviceReader.wifiDeviceEnabled())
    {
        return localeText(TextKey::wifiDisabled);
    }

    const Wifi::Connection::Record::Handler recordHandler;
    if (recordHandler.getLatestEvent().getEventType()
            == Wifi::Connection::EventType::connectionAuthFailed)
    {
        return localeText(TextKey::missingPSK);
    }

    const bool isConnected = recordHandler.isConnected();
    const bool isConnecting = isConnected ?
            false : recordHandler.isConnecting();
    if (isConnected || isConnecting)
    {
        juce::String apName = recordHandler.getActiveAP().getSSID().toString();
        jassert(apName.isNotEmpty());
        return isConnected ? apName :
                (localeText(TextKey::connectingAP) + apName);
    }

    return localeText(TextKey::notConnected);
}


// Updates the widget whenever wireless networking is enabled.
void Settings::WifiControl::wirelessEnabled()
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}


// Updates the widget whenever wireless networking is disabled.
void Settings::WifiControl::wirelessDisabled()
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}


// Shows the loading spinner and updates connection text when a new connection
// starts.
void Settings::WifiControl::startedConnecting
(const Wifi::AccessPoint connectingAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}


// Signals that a new Wifi connection was opened successfully.
void Settings::WifiControl::connected(const Wifi::AccessPoint connectedAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}


// Updates the widget when a connection disconnects, stopping the loading
// spinner and updating the connection text.
void Settings::WifiControl::disconnected
(const Wifi::AccessPoint disconnectedAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}


// Updates the widget when a connection fails, stopping the loading spinner and
// updating the connection text.
void Settings::WifiControl::connectionAuthFailed
(const Wifi::AccessPoint connectingAP)
{
    juce::MessageManager::callAsync([this]() { refresh(); });
}
