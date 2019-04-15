#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Device_Module.h
 *
 * @brief  Tracks whether the Wifi device is enabled, notifying listeners
 *         whenever Wifi is enabled or disabled.
 */

#include "Wifi_Module.h"


namespace Wifi
{
    namespace Device { class Module; }
    class Resource;
}

/**
 * @brief  Tracks whether a Wifi device managed by NetworkManager is known to
 *         exist, and whether it is currently enabled.
 *
 *  Whenever this status changes, the Module notifies all Device::Listener
 * objects of the change.
 */
class Wifi::Device::Module : public Wifi::Module
{
public:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    Module(Resource& parentResource);

    virtual ~Module() { }

    /**
     * @brief  Checks if a Wifi device managed by NetworkManager exists.
     *
     * @return  Whether a managed Wifi device was found.
     */
    bool wifiDeviceExists() const;

    /**
     * @brief  Checks if the wifi device is enabled.
     *
     * @return  Whether a Wifi device managed by NetworkManager is enabled.
     */
    bool wifiDeviceEnabled() const;

    /**
     * @brief  Checks if wifi is currently being enabled or disabled.
     *
     * @return  Whether the wifi device is changing its enabled state.
     */
    bool isDeviceStateChanging() const;

    /**
     * @brief  Connects to NetworkManager to update the Wifi device state,
     *         optionally notifying all Listener objects if the state changes.
     *
     * @param notifyListeners  Whether the module should notify Listener
     *                         objects if the state changes.
     */
    void updateDeviceState(const bool notifyListeners = true);

    /**
     * @brief  Updates the Module's saved wifi device state, optionally
     *         notifying all Listener objects if the state changes.
     *
     * @param exists   Indicates if a managed Wifi device is present.
     *
     * @param enabled  Indicates if the Wifi device is enabled.
     *
     * @param notifyListeners  Whether the module should notify DeviceListeners
     *                         if the state changes.
     */
    void updateDeviceState(const bool exists, const bool enabled,
            const bool notifyListeners = true);

    /**
     * @brief  Notifies the device module that Wifi is about to be enabled or
     *         disabled.
     */
    void signalDeviceStateChanging();

private:
    // Tracks whether a managed Wifi device was found.
    bool deviceExists = false;

    // Tracks whether the Wifi device is enabled.
    bool deviceEnabled = false;

    // Tracks whether Wifi is being enabled or disabled.
    bool stateChanging = false;
};
