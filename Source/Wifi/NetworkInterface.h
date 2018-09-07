#pragma once

#include "SharedResource.h"
#include "ResourceHandler.h"
#include "WifiState.h"
#include "AccessPointState.h"
#include "WifiAccessPoint.h"

/**
 * @file NetworkInterface
 *
 * @brief  Defines an interface for communication between WifiStateManager and
 *         the system wifi libraries.
 */
class NetworkInterface : public SharedResource, public juce::Timer
{
    /* Only the WifiStateManager may directly interact with the 
       NetworkInterface. */
    friend class WifiStateManager;

private:

    NetworkInterface();

public:
    virtual ~NetworkInterface() { }
    
protected:
    /**
     * Listener objects receive wifi state updates.
     */
    class Listener : public ResourceHandler<NetworkInterface>
    {
    public:
        friend class NetworkInterface;

        Listener();

        virtual ~Listener();

    private:
        /**
         * If this Listener has been added to the NetworkInterface, this method
         * will be called whenever the wifi state changes. Listeners should not
         * interact with WifiStateManagers within this function.
         *
         * @param state  The new wifi device state.
         */
        virtual void wifiStateChanged(WifiState state) { };
        
        /**
         * If this Listener has been added to the WifiStateManager, the state
         * manager will call this method whenever a new wifi access point is
         * detected. Listeners should not interact with WifiStateManagers 
         * within this function.
         *
         * @param addedAP  The newly detected access point object.
         */
        virtual void accessPointAdded(const WifiAccessPoint& addedAP) { };
        
        /**
         * If this Listener has been added to the WifiStateManager, the state
         * manager will call this method whenever a wifi access point is no
         * longer detected.  Listeners should not interact with
         * WifiStateManagers within this function.
         *
         * @param removedAP  The access point that is no longer visible.
         */
        virtual void accessPointRemoved(const WifiAccessPoint& removedAP) { };
    };



    /**
     * Gets the current state of the wifi device.
     * 
     * @return  The WifiState that best describes the current wifi device state.
     */
    WifiState getWifiState();

    /**
     * Updates the current wifi state and notifies all listeners. 
     * This method acquires the wifi lock.
     *
     * @param state  The new state of the wifi device.
     */
    void setWifiState(WifiState state);

    /**
     * Gets the connected or connecting wifi access point.
     * 
     * @return the current active WifiAccessPoint if one is found, or a 
     *          null WifiAccessPoint if not connected or connecting.
     */
    virtual WifiAccessPoint getActiveAP() = 0;

    /**
     * Scans for and returns nearby wifi access points.
     * 
     * @return the list of all wifi access points close enough to detect.
     */
    virtual juce::Array<WifiAccessPoint> getVisibleAPs() = 0;

    /**
     * Accesses the wifi device to check if wifi is enabled.
     * 
     * @return true iff wifi is enabled
     */
    virtual bool isWifiEnabled() = 0;

    /**
     * Accesses the wifi device to check if it's attempting to create a wifi
     * connection.
     * 
     * @return true iff currently connecting.
     */
    virtual bool isWifiConnecting() = 0;
           
    /**
     * Accesses the wifi device to check if an active wifi connection
     * exists.
     * 
     * @return true iff a connection is found.
     */
    virtual bool isWifiConnected() = 0;
    
    /**
     * @return true iff a wifi device was successfully found. 
     */
    virtual bool wifiDeviceFound() = 0;

    /**
     * Attempts to open a connection to a wifi access point. This will fail 
     * if wifi is disabled, the access point is invalid, or the psk is 
     * wrong.
     *
     * @param toConnect  Specifies which access point should be connected 
     *                    to.
     *
     * @param psk        The wifi key for toConnect, or the empty string if
     *                    toConnect isn't secured.
     */
    virtual void connectToAccessPoint(const WifiAccessPoint& toConnect,
            juce::String psk = juce::String()) = 0;

    /**
     * Closes any active or activating wifi connection.
     */
    virtual void disconnect() = 0;

    /**
     * If wifi is currently disabled, this will enable it.  Otherwise,
     * nothing will happen.
     */
    virtual void enableWifi() = 0;

    /**
     * If wifi is currently enabled, this will disable it.  Otherwise,
     * nothing will happen.
     */
    virtual void disableWifi() = 0;
    
    /**
     * Finds the current network state of an access point object.
     * 
     * @param accessPoint  The access point to check.
     * 
     * @return  the access point's current state. 
     */
    virtual AccessPointState getAPState
    (const WifiAccessPoint& accessPoint) = 0;
    
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
    virtual juce::Time lastConnectionTime
    (const WifiAccessPoint& accessPoint) = 0;

    //Milliseconds to wait before assuming that enabling or
    //disabling wifi has failed.
    static const constexpr int wifiEnabledChangeTimeout = 5000;

    //Milliseconds to wait before assuming that connecting to
    //or disconnecting from a wifi access point has failed.  This will
    //be reset whenever wifi enters a new connection step.
    static const constexpr int wifiConnectionTimeout = 10000;

protected:           
    /**
     * This method queries the wifi device to ensure that the current 
     * tracked wifi state actually matches the device state.  If necessary,
     * this will update the WifiStateManager's current state.  The
     * NetworkInterface should call this method whenever it has reason
     * to believe the wifi state may be out of date.  
     */
    void confirmWifiState();

    /**
     * Whenever the wifi device detects a connection being established, the 
     * NetworkInterface should call this to notify its WifiStateManager. 
     * 
     * This is intended to notify the state manager of unexpected
     * connections, so it's not necessary to call this for connections
     * opened by the state manager. Calls for known connection attempts will
     * just be ignored.
     */
    void signalWifiConnecting();

    /**
     * Whenever the wifi device establishes a new connection, the 
     * NetworkInterface should call this to notify its WifiStateManager.
     */
    void signalWifiConnected();

    /**
     * Whenever the wifi device fails to connect to an access point, the 
     * NetworkInterface should call this to notify its WifiStateManager.
     */
    void signalConnectionFailed();

    /**
     * Whenever the wifi device disconnects from a wifi access point, the 
     * NetworkInterface should call this to notify its WifiStateManager.
     */
    void signalWifiDisconnected();

    /**
     * Whenever the wifi device is turned on, the NetworkInterface should 
     * call this to notify its WifiStateManager.
     */
    void signalWifiEnabled();

    /**
     * Whenever the wifi device is turned off, the NetworkInterface should 
     * call this to notify its WifiStateManager.
     */
    void signalWifiDisabled();
    
    /**
     * When wifi is connecting and a missing psk is required, the 
     * NetworkInterface should call this to notify its WifiStateManager.
     */
    void signalPskNeeded();
    
    /**
     * Whenever a new wifi access point is detected, the NetworkInterface
     * should call this to notify its WifiStateManager.
     * 
     * @param addedAP  The newly detected access point.
     */
    void signalAPAdded(const WifiAccessPoint& addedAP);
    
    /**
     * Whenever a wifi access point loses visibility, the NetworkInterface
     * should call this to notify its WifiStateManager.
     * 
     * @param removedAP  The access point that is no longer visible.
     */
    void signalAPRemoved(const WifiAccessPoint& removedAP);
   
private:
    /**
     * Whenever a wifi operation is attempted, the timer is set to
     * the appropriate timeout period. If the timer goes off before 
     * the NetworkInterface responds, it will assume that the wifi operation 
     * failed.
     */
    void timerCallback() override;
    
    // SharedResource type key.
    static const juce::Identifier resourceKey;
    // Last registered Wifi state.
    WifiState wifiState = missingNetworkDevice;
};

