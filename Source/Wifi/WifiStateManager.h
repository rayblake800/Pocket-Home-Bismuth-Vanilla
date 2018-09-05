#pragma once
#include "JuceHeader.h"
#include "WindowFocus.h"
#include "ResourceManager.h"
#include "WindowFocusedTimer.h"
#include "WifiAccessPoint.h"
#include "NetworkInterface.h"
#include "WifiState.h"
#include "AccessPointState.h"

/**
 * @file WifiStateManager.h
 *
 * @brief Tracks wifi state and controls the wifi device.
 * 
 * WifiStateManager tracks the current state of the wifi device, provides 
 * information about nearby access points and any current connections,
 * notifies Listener objects whenever Wifi state changes, and provides an
 * interface for requesting Wifi connections or other changes in Wifi
 * state.
 */


class WifiStateManager : private ResourceManager<NetworkInterface>
{
public:
    /**
     * @param createWifiResource  The function used to initialize the
     *                            NetworkInterface.  The default value should
     *                            only be used when it is completely certain 
     *                            that the NetworkInterface does not need to be
     *                            initialized.
     */
    WifiStateManager
    (std::function<NetworkInterface*(juce::ReadWriteLock&)> 
            createWifiResource = [](juce::ReadWriteLock& c)
            {
                return nullptr;
            });

    virtual ~WifiStateManager() { }

    /**
     * Gets the current state of the wifi device.
     * 
     * @return  The WifiState that best describes the current wifi device state.
     */
    WifiState getWifiState();

    /**
     * Add a listener to the list of objects receiving updates whenever Wifi 
     * state changes.
     *
     * @param listener  This object will be subscribed to state updates.
     */
    void addListener(NetworkInterface::Listener* listener);

    /**
     * Searches the list of registered listeners for a particular one, and
     * removes it if it's found.
     * 
     * @param listener  This object will be unsubscribed from state updates.
     */
    void removeListener(NetworkInterface::Listener* listener);

    /**
     * Gets the connected or connecting wifi access point.
     * 
     * @return the current active WifiAccessPoint if one is found, or a 
     *          null WifiAccessPoint if not connected or connecting.
     */
    WifiAccessPoint getActiveAP();

    /**
     * Gets all access points visible to the wifi device.
     * 
     * @return all wifi access points currently detected. If wifi is disabled, 
     *         this will return an empty list.
     */
    juce::Array<WifiAccessPoint> getVisibleAPs();

    /**
     * Checks if wifi is enabled.
     * 
     * @return true iff the wifi device is currently enabled.
     */
    bool isEnabled();

    /**
     * Checks if the wifi device is connected to an access point.
     * 
     * @return true iff the wifi device is currently connected.
     */
    bool isConnected();

    /**
     * Attempts to open a connection to a wifi access point. This will fail if 
     * wifi is disabled, the access point is invalid, or the psk is wrong.
     *
     * @param toConnect  Specifies which access point should be connected to.
     *
     * @param psk        The wifi key for toConnect. This parameter will be
     *                   ignored if toConnect isn't secured.
     */
    void connectToAccessPoint(const WifiAccessPoint& toConnect,
            juce::String psk = juce::String());

    /**
     * Closes any active or activating wifi connection.
     */
    void disconnect();
            
    /**
     * If wifi is currently disabled, this will enable it.  Otherwise, nothing 
     * will happen.
     */
    void enableWifi();
   
    /**
     * If wifi is currently enabled, this will disable it. Otherwise, nothing 
     * will happen.
     */
    void disableWifi();
    
    /**
     * Finds the current network state of an access point object.
     * 
     * @param accessPoint  The access point to check.
     * 
     * @return  the access point's current state. 
     */
    AccessPointState getAPState(const WifiAccessPoint& accessPoint);
        
    /**
     * Finds the last time a connection was active using a specific access
     * point.
     * 
     * @param accessPoint  A wifi access point object.
     * 
     * @return  The last time a connection compatible with this access point
     *          was active, or the Unix epoch time if no compatible
     *          compatible connection has a saved timestamp.
     */
    juce::Time lastConnectionTime(const WifiAccessPoint& accessPoint);

private:
    /**
     * Checks if a saved pointer to the NetworkInterface is still valid.
     *
     * When NetworkInterface methods need to run asynchronously, they should
     * create a WifiStateManager, use this method to check the saved "this"
     * pointer for validity, and ensure the created WifiStateManager instance
     * doesn't go out of scope until the function exits.  This verifies that the
     * saved instance is still valid, and ensures that it remains valid for the
     * duration of the asynchronous function.
     *
     * @param savedAddress  A saved pointer to the network interface.
     *
     * @return  True if the saved address still points to the singular network
     *          interface object, false if the interface has been destroyed
     *          since the address was saved.
     */
    bool validNetworkInterface(const NetworkInterface* savedAddress);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiStateManager)
};

