#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Signal_DeviceModule.h
 *
 * @brief  Handles signals from the Wifi device object, updating Wifi connection
 *         state and visible access points. 
 */

#include "Wifi_LibNM_Signal_DeviceHandler.h"
#include "Wifi_Module.h"
#include <nm-device-wifi.h>

namespace Wifi 
{ 
    class Resource;
    namespace Signal { class DeviceModule; } 
    namespace LibNM 
    { 
        class DeviceWifi;
        class ActiveConnection;
    } 
}

/**
 * @brief  Receives and handles all LibNM::DeviceWifi signals for the Wifi 
 *         module.
 */
class Wifi::Signal::DeviceModule : public LibNM::Signal::DeviceHandler,
    public Wifi::Module
{
public:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    DeviceModule(Resource& parentResource);

    virtual ~DeviceModule() { }

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
     * @brief  Handles Wifi device state changes.
     *
     * @param newState  The new device state value.
     *
     * @param oldState  The previous device state value.
     *
     * @param reason    The reason for the change in device state.
     */
    virtual void stateChanged(NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason) override;

    /**
     * @brief  Updates the access point list whenever a new access point is
     *         detected.
     *
     * @param addedAP  The new access point's LibNM object.
     */
    virtual void accessPointAdded(LibNM::AccessPoint addedAP) override;

    /**
     * @brief  Updates the access point list whenever a previously seen access
     *         point is lost.
     */
    virtual void accessPointRemoved() override;

    /**
     * @brief  Updates the connection record when the active network connection
     *         changes.
     * 
     * @param activeConnection   The new active Wifi connection. If the device 
     *                           has disconnected, this will be a null object.
     */
    virtual void activeConnectionChanged
    (LibNM::ActiveConnection activeConnection) override;
};
