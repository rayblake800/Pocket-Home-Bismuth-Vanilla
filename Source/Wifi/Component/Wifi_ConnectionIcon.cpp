#define WIFI_IMPLEMENTATION
#include "Wifi_ConnectionIcon.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_Device_Reader.h"
#include "Wifi_Connection_Record_Reader.h"

Wifi::ConnectionIcon::ConnectionIcon()
{
#    if JUCE_DEBUG
    setName("Wifi::ConnectionIcon");
#    endif
    const Connection::Record::Reader connectionRecords;
    if(connectionRecords.isConnected())
    {
        AccessPoint connectedAP = connectionRecords.getActiveAP();
        setTrackedAccessPoint(connectedAP);
        setSignalStrengthImage(connectedAP.getSignalStrength());
        return;
    }

    const Device::Reader deviceReader;
    if(deviceReader.wifiDeviceEnabled())
    {
        setSignalStrengthImage(0);
    }
    else
    {
        setWifiDisabledImage();
    }
}

/*
 * Updates the selected icon when the active connection's signal strength 
 * changes.
 */
void Wifi::ConnectionIcon::signalStrengthUpdate(const AccessPoint updatedAP) 
{
    setSignalStrengthImage(updatedAP.getSignalStrength());
}

/*
 * Changes the tracked AccessPoint and selected status icon when a new Wifi 
 * connection is activated.
 */
void Wifi::ConnectionIcon::connected(const AccessPoint connectedAP)
{
    setTrackedAccessPoint(connectedAP);
    setSignalStrengthImage(connectedAP.getSignalStrength());
}

/*
 * Stops tracking signal strength changes and updates the status icon when the 
 * Wifi connection is lost.
 */
void Wifi::ConnectionIcon::disconnected(const AccessPoint disconnectedAP)
{
    ignoreAllUpdates();
    setSignalStrengthImage(0);
}

/*
 * Updates the selected icon when Wifi is turned on.
 */
void Wifi::ConnectionIcon::wirelessEnabled()
{
    Connection::Record::Reader connectionRecord;
    if(connectionRecord.isConnected())
    {
        connected(connectionRecord.getActiveAP());
    }
    else
    {
        setSignalStrengthImage(0);
    }
}

/*
 * Updates the selected icon and stops tracking signal strength when Wifi is 
 * turned off.
 */
void Wifi::ConnectionIcon::wirelessDisabled()
{
    ignoreAllUpdates();
    setWifiDisabledImage();
}
