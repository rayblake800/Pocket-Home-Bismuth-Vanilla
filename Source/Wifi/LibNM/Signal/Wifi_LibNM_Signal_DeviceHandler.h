#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_LibNM_Signal_DeviceHandler.h
 *
 * @brief  Handles signals from the Wifi device object, updating Wifi connection
 *         state and visible access points. 
 */

#include "GLib_Signal_Handler.h"
#include "Wifi_LibNM_Signal_APHandler.h"
#include <nm-device-wifi.h>

namespace Wifi 
{ 
    namespace LibNM 
    { 
        namespace Signal { class DeviceHandler; } 
        class DeviceWifi;
        class ActiveConnection;
    } 
}

class Wifi::LibNM::Signal::DeviceHandler : 
        public GLib::Signal::Handler<DeviceWifi>
{
public:
    DeviceHandler();

    virtual ~DeviceHandler() { }

    /**
     * @brief  Starts tracking the LibNM thread's DeviceWifi object.
     *
     * Signals will not be received until the signal handler is connected.
     */
    void connect();

    /**
     * @brief  Stops tracking the LibNM thread's DeviceWifi object.
     * 
     * Signals will not be received after the signal handler is disconnected 
     * until it is connected again.
     */
    void disconnect();

private:
   /**
    * @brief  Subscribes to all relevant signals from a single GObject 
    *         signal source.
    * 
    * @param source  A NMDeviceWifi GObject this signal handler should 
    *                track.
    */
    virtual void connectAllSignals(DeviceWifi& source) override;

   /**
    * @brief  Handles Wifi device state changes.
    *
    * @param newState  The new device state value.
    *
    * @param oldState  The previous device state value.
    *
    * @param reason    The reason for the change in device state.
    */
    void stateChanged(NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason);

    /**
     * @brief  Updates the access point list whenever a new access point is
     *         detected.
     *
     * @param addedAP  The new access point's LibNM object.
     */
    void accessPointAdded(LibNM::AccessPoint addedAP);

    /**
     * @brief  Updates the access point list whenever a previously seen access
     *         point is lost.
     */
    void accessPointRemoved();

   /**
    * @brief  Updates the connection record when the active network connection
    *         changes.
    * 
    * @param activeConnection   The new active Wifi connection. If the device 
    *                           has disconnected, this will be a null object.
    */
    void activeConnectionChanged(ActiveConnection activeConnection);
        
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
    virtual void propertyChanged(DeviceWifi& source,
            juce::String property) override;

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

    /* Tracks all access point signal strengths. */
    APHandler apHandler;
};
