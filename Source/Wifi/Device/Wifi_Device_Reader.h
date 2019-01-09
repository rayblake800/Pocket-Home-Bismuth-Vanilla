#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Device_Reader.h
 *
 * @brief  Checks the status of the system's Wifi device.
 */

#include "SharedResource_Handler.h"

namespace Wifi { namespace Device { class Reader; } }
namespace Wifi { namespace Device { class Tracker; } }

class Wifi::Device::Reader : public SharedResource::Handler<Tracker>
{
public:
    Reader();

    virtual ~Reader() { }
    
    /**
     * @brief  Checks if the wifi device is enabled. 
     *
     * @return  Whether a Wifi device managed by NetworkManager is enabled. 
     */
    bool wifiDeviceEnabled() const;

    /**
     * @brief  Checks if a Wifi device managed by NetworkManager exists.
     *
     * @return  Whether a managed Wifi device was found. 
     */
    bool wifiDeviceExists() const;

    /**
     * @brief  Checks if wifi is currently being enabled or disabled.
     *
     * @return  Whether the wifi device is changing its enabled state. 
     */
    bool isDeviceStateChanging() const;
};
