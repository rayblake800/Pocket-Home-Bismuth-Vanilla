#pragma once
#include "Wifi/Wifi.h"

/**
 * @file  Wifi/Listeners/UpdateInterfaces/DeviceUpdateInterface.h
 *
 * @brief  The interface shared by DeviceTracker and DeviceListener, used to
 *         notify DeviceListeners when wireless networking is enabled or
 *         disabled.
 */
namespace Wifi
{
    class DeviceUpdateInterface
    {
    public:
        /* Only the DeviceTracker may send update notifications.  */
        friend class DeviceTracker;

        DeviceUpdateInterface() { }

        virtual ~DeviceUpdateInterface() { }

    private:
        /**
         * @brief  Called whenever wireless networking is enabled. 
         *
         * Implement this to define how the DeviceListener should handle the
         * update.
         */
        virtual void wirelessEnabled() = 0;

        /**
         * @brief  Called whenever wireless networking is disabled. 
         *
         * Implement this to define how the DeviceListener should handle the
         * update.
         */
        virtual void wirelessDisabled() = 0;
    };
}
