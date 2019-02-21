#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Signal_DeviceHandler.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "GLib_Signal_CallbackData.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug messages: */
static const constexpr char* dbgPrefix 
        = "Wifi::LibNM::Signal::DeviceHandler::";
#endif

/* Object property keys: */
static const constexpr char * activeConnectionProperty = "active-connection";

/* Object signal keys: */
static const constexpr char * stateChangeSignal = "state-changed";
static const constexpr char * addedAPSignal     = "access-point-added";
static const constexpr char * removedAPSignal   = "access-point-removed";

namespace NMSignal = Wifi::LibNM::Signal;

NMSignal::DeviceHandler::DeviceHandler() { }

/*
 * Subscribes to all relevant signals from a single GObject signal source.
 */
void NMSignal::DeviceHandler::connectAllSignals(const DeviceWifi source) 
{
    ASSERT_NM_CONTEXT;
    if(!source.isNull())
    {
        createConnection(stateChangeSignal, G_CALLBACK(stateChangeCallback),
                new GLib::Signal::CallbackData<DeviceWifi>(source, this));
        createConnection(addedAPSignal, G_CALLBACK(apAddedCallback),
                new GLib::Signal::CallbackData<DeviceWifi>(source, this));
        createConnection(removedAPSignal, G_CALLBACK(apRemovedCallback),
                new GLib::Signal::CallbackData<DeviceWifi>(source, this));
        createPropertyConnection(activeConnectionProperty, source);
    }
}

/*
 * Handles Wifi device state changes.
 */
void NMSignal::DeviceHandler::stateChanged(NMDeviceState newState,
        NMDeviceState oldState, NMDeviceStateReason reason) { }

/*
 * Handles newly detected Wifi access points.
 */
void NMSignal::DeviceHandler::accessPointAdded(AccessPoint addedAP) { }

/*
 * Signals that a saved access point is no longer visible.
 */
void NMSignal::DeviceHandler::accessPointRemoved() { }

/*
 * Handles changes to the active network connection.
 */
void NMSignal::DeviceHandler::activeConnectionChanged
(ActiveConnection activeConnection) { }
 
/*
 * Converts generic property change notifications into activeConnectionChanged 
 * calls.
 */
void NMSignal::DeviceHandler::propertyChanged(const DeviceWifi source,
        juce::String property)
{
    ASSERT_NM_CONTEXT;
    jassert(property == activeConnectionProperty);
    if(!source.isNull())
    {
        activeConnectionChanged(source.getActiveConnection());
    }
}

/*
 * The GCallback method called directly by LibNM code when sending state-changed 
 * signals.
 */
void NMSignal::DeviceHandler::stateChangeCallback(
        NMDevice* device,
        NMDeviceState newState,
        NMDeviceState oldState,
        NMDeviceStateReason reason,
        GLib::Signal::CallbackData<DeviceWifi>* data) 
{ 
    ASSERT_NM_CONTEXT;
    if(data != nullptr)
    {
        DeviceHandler* deviceHandler = dynamic_cast<DeviceHandler*>
                (data->getSignalHandler());
        if(deviceHandler != nullptr)
        {
            deviceHandler->stateChanged(newState, oldState, reason);
        }
    }
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-added signals.
 */
void NMSignal::DeviceHandler::apAddedCallback(
        NMDeviceWifi* device,
        NMAccessPoint* ap,
        GLib::Signal::CallbackData<DeviceWifi>* data) 
{ 
    ASSERT_NM_CONTEXT;
    if(device != nullptr && ap != nullptr && data != nullptr)
    {
        // Ignore access points with null SSIDs.
        // TODO: Find out why access points without SSIDs are showing up at all
        //       Are they hidden? Have the SSIDs just not loaded yet?
        if(nm_access_point_get_ssid(ap) == nullptr)
        {
            DBG(dbgPrefix << __func__ << ": Ignoring AP with null SSID.");
            return;
        }

        DeviceWifi wifiDevice = data->getSignalSource();
        DeviceHandler* deviceHandler = dynamic_cast<DeviceHandler*>
                (data->getSignalHandler());
        if(!wifiDevice.isNull() && deviceHandler != nullptr)
        {
            deviceHandler->accessPointAdded(wifiDevice.getAPLender()
                    ->borrowObject(G_OBJECT(ap)));
        }
    }
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-removed signals.
 */
void NMSignal::DeviceHandler::apRemovedCallback(
        NMDeviceWifi* device, 
        NMAccessPoint* ap,
        GLib::Signal::CallbackData<DeviceWifi>* data) 
{ 
    ASSERT_NM_CONTEXT;
    if(data != nullptr)
    {
        DeviceWifi wifiDevice = data->getSignalSource();
        DeviceHandler* deviceHandler = dynamic_cast<DeviceHandler*>
                (data->getSignalHandler());
        if(!wifiDevice.isNull() && deviceHandler != nullptr)
        {
            wifiDevice.getAPLender()->invalidateObject(G_OBJECT(ap));
            deviceHandler->accessPointRemoved();
        }
    }
}
