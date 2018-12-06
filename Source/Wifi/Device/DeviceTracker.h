#pragma once
/**
 * @file  Wifi/State/DeviceTracker.h
 *
 * @brief  Tracks whether the Wifi device is enabled, notifying listeners
 *         whenever Wifi is enabled or disabled. 
 */

#include "SharedResource/Resource.h"
#include "LibNM/NMObjects/Client.h"
#include "WindowFocus/WindowFocus.h"

namespace Wifi { class DeviceTracker; }

/**
 *  DeviceTracker tracks whether a Wifi device managed by NetworkManager is
 * known to exist, and whether it is currently enabled. Whenever this status
 * changes, the DeviceTracker notifies all DeviceListeners of the change. 
 * 
 *  DeviceTracker is a singleton SharedResource object, and may only be accessed
 * by its Handler classes, DeviceListener and DeviceSignalHandler. A 
 * DeviceSignalHandler instance is required to keep the DeviceTracker updated
 * when the Wifi device state changes.
 */
class Wifi::DeviceTracker : public SharedResource::Resource
{
public:
    /* SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Checks the initial Wifi device state.
     */
    DeviceTracker();

    virtual ~DeviceTracker() { }

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
     * @brief  Connects to NetworkManager to update the Wifi device state,
     *         notifying all DeviceListeners if the state changes.
     */
    void updateDeviceState(const bool notifyListeners = true);

    /**
     * @brief  Updates the DeviceTracker's saved wifi device state, notifying
     *         all DeviceListeners if the state changes.
     *
     * @param exists   Indicates if a managed Wifi device is present.
     *
     * @param enabled  Indicates if the Wifi device is enabled.
     */
    void updateDeviceState(const bool exists, const bool enabled);

private:
    /* Tracks whether a managed Wifi device was found.  */
    bool deviceExists;

    /* Tracks whether the Wifi device is enabled. */
    bool deviceEnabled;
};
