#pragma once
/**
 * @file  Wifi_Device_Listener.h
 *
 * @brief  Provides a basis for classes that should receive updates when
 *         wireless networking is enabled or disabled.
 */

#ifndef WIFI_IMPLEMENTATION
    #define DEVICE_LISTENER_IMPLEMENTATION
    #define WIFI_IMPLEMENTATION
#endif
#include "Wifi_Device_UpdateInterface.h"
#ifdef DEVICE_LISTENER_IMPLEMENTATION
    #undef DEVICE_LISTENER_IMPLEMENTATION
    #undef WIFI_IMPLEMENTATION
#endif
#include "SharedResource_Modular_Handler.h"

namespace Wifi
{
    class Resource;
    namespace Device
    {
        class Listener;
        class Module;
    }
}

/**
 * @brief  Checks if a Wifi device is present and enabled, and receives signals
 *         whenever Wifi is enabled or disabled.
 */
class Wifi::Device::Listener :
    public SharedResource::Modular::Handler<Resource, Module>,
    public UpdateInterface
{
public:
    Listener();

    virtual ~Listener() { }

    /**
     * @brief  Checks if a Wifi device managed by NetworkManager exists.
     *
     * @return  Whether a managed Wifi device was found.
     */
    bool wifiDeviceExists() const;

    /**
     * @brief  Checks if the managed Wifi device is currently enabled.
     *
     * @return  Whether wireless networking is enabled.
     */
    bool wifiDeviceEnabled() const;

private:
    /**
     * @brief  Called whenever wireless networking is enabled.
     *
     * Override this to define how the DeviceListener should handle the update.
     */
    virtual void wirelessEnabled() override { }

    /**
     * @brief  Called whenever wireless networking is disabled.
     *
     * Override this to define how the DeviceListener should handle the update.
     */
    virtual void wirelessDisabled() override { }
};
