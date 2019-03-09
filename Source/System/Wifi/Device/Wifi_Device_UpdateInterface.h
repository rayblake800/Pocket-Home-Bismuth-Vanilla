#pragma once
/**
 * @file  Wifi_Device_UpdateInterface.h
 *
 * @brief  The interface shared by Device::Module and Device::Listener, used to
 *         notify all Listener objects when wireless networking is enabled or
 *         disabled.
 */

namespace Wifi { namespace Device { class UpdateInterface; } }
namespace Wifi { namespace Device { class Module; } }

class Wifi::Device::UpdateInterface
{
public:
    UpdateInterface() { }

    virtual ~UpdateInterface() { }

private:
    /* Only the Module may send update notifications.  */
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
