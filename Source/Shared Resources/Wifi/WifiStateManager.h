#pragma once
#include "JuceHeader.h"
#include "WindowFocus.h"
#include "ResourceManager.h"
#include "WindowFocusedTimer.h"
#include "WifiAccessPoint.h"

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
 *
 * WifiStateManager::NetworkInterface is responsible for all direct
 * communication with the Wifi device.
 */


class WifiStateManager : private ResourceManager
{
public:
    class Listener;

    WifiStateManager
    (std::function<ResourceManager::SharedResource*(CriticalSection&)> 
            createWifiResource = [](CriticalSection& c)
            {
                return nullptr;
            });

    virtual ~WifiStateManager() { }

    enum WifiState
    {
        //no wifi device found
        missingNetworkDevice,
        //Wifi device is disabled.
        disabled,
        //Wifi device is being turned on.
        turningOn,
        //Wifi is turned on but not connected.
        enabled,
        //Wifi device is being turned on.
        turningOff,
        //Wifi device is connecting to an access point.
        connecting,
        //Connecting access point needs a password.
        missingPassword,
        //Wifi device is connected to an access point.
        connected,
        //Wifi device is closing its connection to an access point.
        disconnecting
    };

#ifdef JUCE_DEBUG    

    /**
     * Convert a WifiState to a string for debugging.
     */
    static String wifiStateString(WifiState state)
    {
        switch (state)
        {
            case missingNetworkDevice:
                return "missingNetworkDevice";
            case disabled:
                return "disabled";
            case turningOn:
                return "turningOn";
            case enabled:
                return "enabled";
            case turningOff:
                return "turningOff";
            case connecting:
                return "connecting";
            case missingPassword:
                return "missingPassword";
            case connected:
                return "connected";
            case disconnecting:
                return "disconnecting";
        }
    }
#endif

    /**
     * Gets the current state of the wifi device.
     * 
     * @return the current state of the wifi device
     */
    WifiState getWifiState();

    /**
     * Add a listener to the list of objects receiving updates whenever Wifi 
     * state changes.
     *
     * @param listener  This object will be subscribed to state updates.
     */
    void addListener(Listener* listener);

    /**
     * Searches the list of registered listeners for a particular one, and
     * removes it if it's found.
     * 
     * @param listener  This object will be unsubscribed from state updates.
     */
    void removeListener(Listener* listener);

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
    Array<WifiAccessPoint> getVisibleAPs();

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
            String psk = String());

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
     * Describes the current network state of a WifiAccessPoint object.
     */
    enum AccessPointState
    {
        // WifiAccessPoint contains no access point data.
        nullAP,
        // The access point is not visible to the wifi device.
        missingAP,
        // The access point is visible but not being used.
        disconnectedAP,
        // A connection is being created through this access point.
        connectingAP,
        // An active connection exists using this access point.
        connectedAP,
        // A connection using this access point is being closed.
        disconnectingAP,
        // Connecting failed because the security key was invalid.
        invalidSecurityAP
    };
    
    /**
     * Finds the current network state of an access point object.
     * 
     * @param accessPoint  The access point to check.
     * 
     * @return  the access point's current state. 
     */
    AccessPointState getAPState(const WifiAccessPoint& accessPoint);

    class NetworkInterface : public ResourceManager::SharedResource, 
    public Timer
    {
    public:
        /**
         * @param wifiLock    The static resource lock used to control access to
         *                    the NetworkInterface.  This should only be used
         *                    for methods triggered by the timer or by wifi
	 *                    device signals.  In any other case, it is assumed
	 *                    that the caller acquired the lock already.
         */
        NetworkInterface(CriticalSection& wifiLock);

        virtual ~NetworkInterface() { }

        /**
         * Gets the current state of the wifi device.
         * 
         * @see WifiStateManager::getWifiState()
         */
        WifiState getWifiState();

        /**
         * Updates the current wifi state and notifies all listeners. 
	 * This method acquires the wifi lock.
         */
        void setWifiState(WifiState state);

        /**
         * Add a listener to the list of objects receiving updates whenever 
         * Wifi state changes.  
         * 
         * @see WifiStateManager::addListener()
         */
        void addListener(WifiStateManager::Listener* listener);

        /**
         * Searches the list of registered listeners for a particular one, and 
         * removes it if it's found. 
         * 
         * @see WifiStateManager::removeListener()
         */
        void removeListener(WifiStateManager::Listener* listener);

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
        virtual Array<WifiAccessPoint> getVisibleAPs() = 0;

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
                String psk = String()) = 0;

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

        //Milliseconds to wait before assuming that enabling or
        //disabling wifi has failed.
        static const constexpr int wifiEnabledChangeTimeout = 10000;

        //Milliseconds to wait before assuming that connecting to
        //or disconnecting from a wifi access point has failed.  This will
        //be reset whenever wifi enters a new connection step.
        static const constexpr int wifiConnectionTimeout = 5000;
    
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
         *
         * @param connectedAP the newly connected access point
         */
        void signalWifiConnected(WifiAccessPoint connectedAP);

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

    private:
        /**
         * Whenever a wifi operation is attempted, the timer is set to
         * the appropriate timeout period. If the timer goes off before 
         * the WifiNetworkInterface responds, the WifiStateManager will
         * assume that the wifi operation failed.
         * This method acquires the WifiStateManager's stateLock.
         */
        void timerCallback() override;
        
        WifiState wifiState = missingNetworkDevice;
        CriticalSection& wifiLock;
        //All objects tracking the current wifi state.
        Array<WifiStateManager::Listener*> listeners;
        //Listeners waiting for a pending notification.
        Array<WifiStateManager::Listener*> notifyQueue;
    };

public:

    /**
     * Listener objects can be added to a WifiStateManager to receive wifi state
     * updates. Once added to a WifiStateManager, Listener objects can get the 
     * current wifi state at any time until they're removed from the state 
     * manager.
     */
    class Listener
    {
    public:
        friend class NetworkInterface;

        Listener() { }

        /**
         * On destruction, listeners will remove themselves from their 
         * WifiStateManager if necessary.
         */
        virtual ~Listener();

    private:

        /**
         * When added to a WifiStateManager, it will call this method whenever 
         * the wifi state changes.
         *
         * @param state new wifi device state
         */
        virtual void wifiStateChanged(WifiState state) { };
    };

private:

    //ResourceManager shared object and lock;
    static ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static CriticalSection stateLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiStateManager)
};

