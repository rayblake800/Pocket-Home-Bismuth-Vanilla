#pragma once
/**
 * @file  Wifi/Control/Controller.h
 *
 * @brief  Sends basic commands to the Wifi device.
 */

#include "SharedResource/Handler.h"

namespace Wifi { class Controller; }
namespace Wifi { class DeviceTracker; }

/**
 *  Controller objects are used to turn Wifi on and off, and to make the Wifi
 * device rescan visible access points. 
 */
class Wifi::Controller : public SharedResource::Handler<DeviceTracker>
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
