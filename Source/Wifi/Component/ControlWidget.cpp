#define WIFI_IMPLEMENTATION
#include "Wifi/Component/ControlWidget.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/Device/DeviceViewer.h"
#include "Wifi/Device/Controller.h"
#include "Wifi/Connection/RecordReader.h"
#include "Wifi/Connection/Event.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "Wifi::ControlWidget";

/* Localized text keys: */
static const juce::Identifier wifiNotFoundTextKey      = "wifiNotFound";
static const juce::Identifier wifiDisabledTextKey      = "wifiDisabled";
static const juce::Identifier notConnectedTextKey      = "notConnected";
static const juce::Identifier connectingAPTextKey      = "connectingAP";
static const juce::Identifier missingPSKTextKey        = "missingPSK";
static const juce::Identifier disconnectingTextKey     = "disconnecting";
    
Wifi::ControlWidget::ControlWidget(std::function<void() > openWifiPage) :
ConnectionSettingsComponent(openWifiPage, localeClassKey.toString()),
Locale::TextUser(localeClassKey)
{
    refresh();
}

/*
 * Checks if wifi is currently turned on.
 */
bool Wifi::ControlWidget::connectionEnabled()
{
    const DeviceViewer wifiDevice;
    return wifiDevice.wifiDeviceEnabled();
}

/*
 * Determines if the loading spinner should be shown.
 */
bool Wifi::ControlWidget::shouldShowSpinner() 
{
    const Connection::RecordReader connectionRecords;
    const DeviceViewer deviceViewer; 
    return connectionRecords.isConnecting()
            && !deviceViewer.isDeviceStateChanging();
}

/*
 * Determines if the connection switch should be enabled.
 */
bool Wifi::ControlWidget::allowConnectionToggle()
{
    const DeviceViewer deviceViewer;
    return deviceViewer.wifiDeviceExists() 
        && !deviceViewer.isDeviceStateChanging();
}

/*
 * Determines if the connection page should be accessible.
 */
bool Wifi::ControlWidget::connectionPageAvailable() 
{
    const DeviceViewer deviceViewer;
    const Connection::RecordReader connectionRecords;
    return deviceViewer.wifiDeviceEnabled()
        && !deviceViewer.isDeviceStateChanging()
        && !connectionRecords.isConnecting();
}


/*
 * Gets the wifi icon path.
 */
juce::String Wifi::ControlWidget::getIconAsset()
{
    //TODO: define in JSON file
    return "wifiIcon.svg";
}

/**
 * Enable or disable the wifi radio.
 */
void Wifi::ControlWidget::enabledStateChanged(bool enabled)
{
    Controller deviceController;
    deviceController.setEnabled(enabled);
}

/**
 * Sets the wifi button text based on the current wifi state.
 */
juce::String Wifi::ControlWidget::updateButtonText()
{
    const DeviceViewer deviceViewer;
    if(!deviceViewer.wifiDeviceExists())
    {
        return localeText(wifiNotFoundTextKey);
    }
    if(!deviceViewer.wifiDeviceEnabled())
    {
        return localeText(wifiDisabledTextKey);
    }

    const Connection::RecordReader connectionRecords;
    if(connectionRecords.getLastEvent().getEventType()
            == Connection::EventType::connectionAuthFailed)
    {
        return localeText(missingPSKTextKey);
    }

    const bool isConnected = connectionRecords.isConnected();
    const bool isConnecting = isConnected ? 
        false : connectionRecords.isConnecting();
    if(isConnected || isConnecting)
    {
        juce::String apName 
                = connectionRecords.getActiveAP().getSSID().toString();
        jassert(apName.isNotEmpty());
        return isConnected ? apName :
            (localeText(connectingAPTextKey) + apName);
    }

    return localeText(notConnectedTextKey);
}

/*
 * Updates the widget whenever wireless networking is enabled.
 */
void Wifi::ControlWidget::wirelessEnabled()
{
    refresh();
}

/*
 * Updates the widget whenever wireless networking is disabled.
 */
void Wifi::ControlWidget::wirelessDisabled()
{
    refresh();
}

/*
 * Shows the loading spinner and updates connection text when a new connection 
 * starts.
 */
void Wifi::ControlWidget::startedConnecting(const AccessPoint connectingAP)
{
    refresh();
}

/*
 * Signals that a new Wifi connection was opened successfully.
 */
void Wifi::ControlWidget::connected(const AccessPoint connectedAP)
{
    refresh();
}

/*
 * Updates the widget when a connection disconnects, stopping the loading 
 * spinner and updating the connection text.
 */
void Wifi::ControlWidget::disconnected(const AccessPoint disconnectedAP)
{
    refresh();
}

/*
 * Updates the widget when a connection fails, stopping the loading spinner and
 * updating the connection text.
 */
void Wifi::ControlWidget::connectionAuthFailed(const AccessPoint connectingAP)
{
    refresh();
}
