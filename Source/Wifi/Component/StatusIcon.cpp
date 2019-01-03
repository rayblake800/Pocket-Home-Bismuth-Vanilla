#define WIFI_IMPLEMENTATION
#include "Wifi/Component/StatusIcon.h"
#include "ComponentConfigKeys.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/Connection/RecordReader.h"
#include "Wifi/Device/DeviceViewer.h"

Wifi::StatusIcon::StatusIcon() :
ConfigurableImageComponent(ComponentConfigKeys::wifiIconKey)
{
#    if JUCE_DEBUG
    setName("Wifi::StatusIcon");
#    endif
    const Connection::RecordReader connectionRecords;
    if(connectionRecords.isConnected())
    {
        AccessPoint connectedAP = connectionRecords.getActiveAP();
        setTrackedAccessPoint(connectedAP);
        signalStrengthUpdate(connectedAP);
        return;
    }

    const DeviceViewer deviceViewer;
    setIcon(deviceViewer.wifiDeviceEnabled() ? 
            APIcon::wifiStrength0 : APIcon::wifiOff);
}

/*
 * Sets the displayed WiFi connection icon.
 */
void Wifi::StatusIcon::setIcon(const APIcon selectedIcon)
{
    setImageAssetIndex((int) selectedIcon);
}


/*
 * Updates the selected icon when the active connection's signal strength 
 * changes.
 */
void Wifi::StatusIcon::signalStrengthUpdate(const AccessPoint updatedAP) 
{
    const int minStrengthIdx = (int) APIcon::wifiStrength0;
    const int maxStrengthIdx = (int) APIcon::wifiStrength3;
    const int range = maxStrengthIdx - minStrengthIdx;
    const int strengthLevel = updatedAP.getSignalStrength() * range / 100;
    const int index = minStrengthIdx + strengthLevel;
    juce::MessageManager::callAsync([this, index]
    {
        setImageAssetIndex(index);
    });
}

/*
 * Changes the tracked AccessPoint and selected status icon when a new Wifi 
 * connection is activated.
 */
void Wifi::StatusIcon::connected(const AccessPoint connectedAP)
{
    setTrackedAccessPoint(connectedAP);
    signalStrengthUpdate(connectedAP);
}

/*
 * Stops tracking signal strength changes and updates the status icon when the 
 * Wifi connection is lost.
 */
void Wifi::StatusIcon::disconnected(const AccessPoint disconnectedAP)
{
    ignoreAllUpdates();
    juce::MessageManager::callAsync([this]
    {
        setIcon(APIcon::wifiStrength0);
    });
}

/*
 * Updates the selected icon when Wifi is turned on.
 */
void Wifi::StatusIcon::wirelessEnabled()
{
    juce::MessageManager::callAsync([this]
    {
        setIcon(APIcon::wifiStrength0);
    });
}

/*
 * Updates the selected icon and stops tracking signal strength when Wifi is 
 * turned off.
 */
void Wifi::StatusIcon::wirelessDisabled()
{
    ignoreAllUpdates();
    juce::MessageManager::callAsync([this]
    {
        setIcon(APIcon::wifiOff);
    });
}
