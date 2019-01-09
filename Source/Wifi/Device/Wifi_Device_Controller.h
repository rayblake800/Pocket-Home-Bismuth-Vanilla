#pragma once
/**
 * @file  Wifi_Device_Controller.h
 *
 * @brief  Sends basic commands to the Wifi device.
 */

#include "SharedResource_Handler.h"

namespace Wifi { namespace Device { class Controller; } }
namespace Wifi { namespace Device { class Tracker; } }

/**
 *  Controller objects are used to turn Wifi on and off, and to make the Wifi
 * device rescan visible access points. 
 */
class Wifi::Device::Controller : public SharedResource::Handler<Tracker>
{
public:
    Controller();

    virtual ~Controller() { }

    /**
     * @brief  Enables or disables the Wifi device.
     *
     * @param enableWifi  If true, Wifi will be enabled, if false, Wifi will be
     *                    disabled.
     */
    void setEnabled(const bool enableWifi);

    /**
     * @brief  Makes the Wifi device rescan nearby access points.
     */
    void scanAccessPoints();
};
