#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi/Device/DeviceViewer.h
 *
 * @brief  Checks the status of the system's Wifi device.
 */

#include "SharedResource/Handler.h"

namespace Wifi { class DeviceViewer; }
namespace Wifi { class DeviceTracker; }

class Wifi::DeviceViewer : public SharedResource::Handler<DeviceTracker>
{
public:
    DeviceViewer();

    virtual ~DeviceViewer() { }
    
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
