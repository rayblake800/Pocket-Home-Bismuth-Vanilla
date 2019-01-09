#pragma once
/**
 * @file  Wifi_Device_Tracker.h
 *
 * @brief  Tracks whether the Wifi device is enabled, notifying listeners
 *         whenever Wifi is enabled or disabled. 
 */

#include "SharedResource_Resource.h"
#include "LibNM/OwnedObjects/Client.h"
#include "WindowFocus/WindowFocus.h"

namespace Wifi { namespace Device { class Tracker; } }

/**
 *  Wifi::Device::Tracker tracks whether a Wifi device managed by NetworkManager
 * is known to exist, and whether it is currently enabled. Whenever this status
 * changes, the Tracker notifies all Wifi::Device::Listener objects of the 
 * change. 
 * 
 *  Tracker is a singleton SharedResource object, and may only be accessed by 
 * its Handler classes, Device::Listener and Device::SignalHandler. A 
 * SignalHandler instance is required to keep the Tracker updated when the Wifi 
 * device state changes.
 */
class Wifi::Device::Tracker : public SharedResource::Resource
{
public:
    /* SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Checks the initial Wifi device state.
     */
    Tracker();

    virtual ~Tracker() { }

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

    /**
     * @brief  Notifies the device tracker that Wifi is about to be enabled or
     *         disabled.
     */
    void signalDeviceStateChanging();

private:
    /* Tracks whether a managed Wifi device was found.  */
    bool deviceExists = false;

    /* Tracks whether the Wifi device is enabled. */
    bool deviceEnabled = false;

    /* Tracks whether Wifi is being enabled or disabled. */
    bool stateChanging = false;
};
