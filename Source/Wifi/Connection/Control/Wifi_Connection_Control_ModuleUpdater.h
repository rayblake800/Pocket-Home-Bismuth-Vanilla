#pragma once
/**
 * @file  Wifi_Connection_Control_ModuleUpdater.h
 *
 * @brief  An interface for passing Wifi event updates to the 
 *         Wifi::Connection::Control::Module object instance.
 */

#include "SharedResource_Modular_Handler.h"
#include <nm-device.h>

namespace Wifi 
{ 
    class Resource;
    namespace Connection 
    { 
        namespace Control
        {
            class Module;
            class ModuleUpdater;
        }
    }
    namespace LibNM { class AccessPoint; }
}

class Wifi::Connection::Control::ModuleUpdater :
        public SharedResource::Modular::Handler<Resource, Module>
{
public:
    ModuleUpdater();

    virtual ~ModuleUpdater() { }

    /**
     * @brief  Notifies the Control::Module that a new access point was spotted,
     *         just in case it's needed to establish a connection.
     *
     * @param addedAP  An access point object that was just added.
     */
    void signalAPAdded(LibNM::AccessPoint newAP);

    /**
     * @brief  Notifies the Control::Module that wireless networking was
     *         disabled.
     */
    void signalWifiDisabled();

    /**
     * @brief  Notifies the Control::Module that the Wifi device state has
     *         changed.
     *
     * @param newState  The new Wifi device state.
     *
     * @param oldState  The last state of the Wifi device before the change.
     *
     * @param reason    The reason for the change in state.
     */
    void signalWifiStateChange(
            NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason);
};
