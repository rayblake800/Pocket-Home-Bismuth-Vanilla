#pragma once
/**
 * @file  Wifi_Device_UpdateInterface.h
 *
 * @brief  The interface shared by Device::Tracker and Device::Listener, used to
 *         notify DeviceListeners when wireless networking is enabled or
 *         disabled.
 */

namespace Wifi { namespace Device { class UpdateInterface; } }

class Wifi::Device::UpdateInterface
{
public:
    /* Only the Tracker may send update notifications.  */
    friend class Tracker;

    UpdateInterface() { }

    virtual ~UpdateInterface() { }

private:
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
