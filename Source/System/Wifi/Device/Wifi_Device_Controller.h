#pragma once
/**
 * @file  Wifi_Device_Controller.h
 *
 * @brief  Sends basic commands to the Wifi device.
 */

#include "SharedResource_Modular_Handler.h"

namespace Wifi 
{ 
    namespace Device 
    { 
        class Controller; 
        class Module;
    } 
    class Resource;
}

/**
 * @brief  Accesses the wifi device module to turn Wifi on and off, and to make 
 *         the Wifi * device rescan visible access points. 
 */
class Wifi::Device::Controller : 
    public SharedResource::Modular::Handler<Resource, Module>
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
