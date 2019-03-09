#define WIFI_IMPLEMENTATION
#include "Wifi_ConnectionIcon.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_Device_Reader.h"
#include "Wifi_Connection_Record_Reader.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Wifi::ConnectionIcon::";
#endif

Wifi::ConnectionIcon::ConnectionIcon()
{
#if JUCE_DEBUG
    setName("Wifi::ConnectionIcon");
#endif
    const Connection::Record::Reader connectionRecords;
    if(connectionRecords.isConnected())
    {
        AccessPoint connectedAP = connectionRecords.getActiveAP();
        setTrackedAccessPoint(connectedAP);
        setSignalStrengthImage(connectedAP.getSignalStrength());
        DBG(dbgPrefix << __func__ << ": Initializing with connected AP "
                << connectedAP.getSSID().toString());
        return;
    }

    const Device::Reader deviceReader;
    if(deviceReader.wifiDeviceEnabled())
    {
        DBG(dbgPrefix << __func__ << ": Initializing with wifi disconnected.");
        setSignalStrengthImage(0);
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Initializing with wifi disabled.");
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
    DBG(dbgPrefix << __func__ << ": Now tracking connected AP "
            << connectedAP.getSSID().toString());
}

/*
 * Stops tracking signal strength changes and updates the status icon when the 
 * Wifi connection is lost.
 */
void Wifi::ConnectionIcon::disconnected(const AccessPoint disconnectedAP)
{
    ignoreAllUpdates();
    setSignalStrengthImage(0);
    DBG(dbgPrefix << __func__ << ": Removing old tracked AP");
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
