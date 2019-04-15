#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Device_UpdateInterface.h
 *
 * @brief  The interface shared by Device::Module and Device::Listener, used to
 *         notify all Listener objects when wireless networking is enabled or
 *         disabled.
 */

namespace Wifi
{
    namespace Device
    {
        class UpdateInterface;
        class Module;
    }
}

/**
 * @brief  An interface used to send updates when wireless networking is
 *         enabled or disabled.
 */
class Wifi::Device::UpdateInterface
{
public:
    UpdateInterface() { }

    virtual ~UpdateInterface() { }

private:
    // Only the Module may send update notifications.
    friend class Module;

    /**
     * @brief  Called whenever wireless networking is enabled.
     *
     * Implement this to define how the Device::Listener should handle the
     * update.
     */
    virtual void wirelessEnabled() = 0;

    /**
     * @brief  Called whenever wireless networking is disabled.
     *
     * Implement this to define how the Device::Listener should handle the
     * update.
     */
    virtual void wirelessDisabled() = 0;
};
