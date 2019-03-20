#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_LibNM_Signal_DeviceHandler.h
 *
 * @brief  A basis for classes that should be updated when the list of visible
 *         Wifi access points or the state of the active Wifi Device change.
 */

#include "GLib_Signal_Handler.h"
#include <nm-device-wifi.h>

namespace Wifi 
{ 
    namespace LibNM 
    { 
        namespace Signal { class DeviceHandler; } 
        class DeviceWifi;
        class AccessPoint;
        class ActiveConnection;
    } 
}

/**
 * @brief  Handles signals from the active LibNM::DeviceWifi object, receiving 
 *         updates when Wifi connection state or the list of visible access 
 *         points change. 
 */
class Wifi::LibNM::Signal::DeviceHandler : 
        public GLib::Signal::Handler<DeviceWifi>
{
public:
    DeviceHandler();

    virtual ~DeviceHandler() { }

protected:
    /**
     * @brief  Subscribes to all relevant signals from a single GObject 
     *         signal source.
     * 
     * @param source  A DeviceWifi object this signal handler should track.
     */
    virtual void connectAllSignals(const DeviceWifi source) override;

    /**
     * @brief  Handles Wifi device state changes.
     *
     *  This takes no action by default. Subclasses should override this method
     * to define how Wifi device state changes should be handled.
     *
     * @param newState  The new device state value.
     *
     * @param oldState  The previous device state value.
     *
     * @param reason    The reason for the change in device state.
     */
    virtual void stateChanged(NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason);

    /**
     * @brief  Handles newly detected Wifi access points.
     *
     *  This takes no action by default. Subclasses should override this method
     * to define how new access points should be handled.
     *
     * @param addedAP  The new access point's LibNM object.
     */
    virtual void accessPointAdded(LibNM::AccessPoint addedAP);

    /**
     * @brief  Signals that a saved access point is no longer visible.
     *
     *  This takes no action by default. Subclasses should override this method
     * to define how removed access points should be handled.
     */
    virtual void accessPointRemoved();

    /**
     * @brief  Handles changes to the active network connection.
     *
     *  This takes no action by default. Subclasses should override this method
     * to define how new active connections should be handled.
     * 
     * @param activeConnection   The new active Wifi connection. If the device 
     *                           has disconnected, this will be a null object.
     */
    virtual void activeConnectionChanged(ActiveConnection activeConnection);
        
    /**
     * @brief  Converts generic property change notifications into 
     *         activeConnectionChanged calls.
     * 
     * @param source    The GObject that emitted the signal. This should be a
     *                  NMDeviceWifi object.
     * 
     * @param property  This should be the active connection property, 
     *                  "active-connection"
     */
    virtual void propertyChanged(const DeviceWifi source,
            juce::String property) override;

private:
    /**
     * @brief  The GCallback method called directly by LibNM code when sending 
     *         state-changed signals.  
     *
     * This will use the signal data to call the listener object's stateChanged 
     * method.
     * 
     * @param device    The GObject sending the state change signal.
     *
     * @param newState  The new device state.
     *
     * @param oldState  The previous device state.
     *
     * @param reason    The reason for the change in state.
     *
     * @param data      A data object set up to handle signals from this device.
     */
    static void stateChangeCallback(
            NMDevice* device,
            NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason,
            GLib::Signal::CallbackData<DeviceWifi>* data);

    /**
     * @brief  The GCallback method called directly by LibNM code when sending 
     *         access-point-added signals.  
     *
     * This will use the signal data to call the listener object's 
     * accessPointAdded method
     * 
     * @param device    The GObject sending the signal.
     *
     * @param ap        An access point object representing a newly discovered
     *                  access point.
     *
     * @param data      A data object set up to handle signals from this device.
     */
    static void apAddedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            GLib::Signal::CallbackData<DeviceWifi>* data);

    /**
     * @brief  The GCallback method called directly by LibNM code when sending 
     *         access-point-removed signals.  
     *
     * This will use the signal data to call the listener object's 
     * accessPointRemoved method.
     * 
     * @param device    The GObject sending the signal.
     *
     * @param ap        An access point object representing a wifi access point
     *                  that is no longer visible.
     *
     * @param data      A data object set up to handle signals from this device.
     */
    static void apRemovedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            GLib::Signal::CallbackData<DeviceWifi>* data);
};
