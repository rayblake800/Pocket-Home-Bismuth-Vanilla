/**
 * @file  Wifi_Device_Reader.h
 *
 * @brief  Checks the status of the system's Wifi device.
 */

#include "SharedResource_Modular_Handler.h"

namespace Wifi 
{ 
    class Resource;
    namespace Device 
    { 
        class Reader;
        class Module;
    }
}

/**
 * @brief  Connects to the Device::Module object to check the state of the
 *         primary Wifi device.
 */
class Wifi::Device::Reader : 
    public SharedResource::Modular::Handler<Resource, Module>
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
