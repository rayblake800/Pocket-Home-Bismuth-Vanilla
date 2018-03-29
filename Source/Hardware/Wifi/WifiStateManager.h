/**
 * @file WifiStateManager.h
 *
 * WifiStateManager tracks the current state of the Wifi network device,
 * provides information about nearby access points and any current connections,
 * notifies Listener objects whenever Wifi state changes, and provides an
 * interface for requesting Wifi connections or other changes in Wifi
 * state.
 *
 * WifiStateManager::WifiNetworkInterface is responsible for all direct
 * communication with the Wifi device.  In order for a WifiStateManager
 * object to function correctly, an object implementing WifiNetworkInterface
 * must be assigned to it with setNetworkInterface.
 */

#pragma once
#include "JuceHeader.h"
#include "WindowFocus.h"
#include "ResourceManager.h"
#include "WifiAccessPoint.h"

class WifiStateManager : private ResourceManager
{
public:
    class Listener;
    class NetworkInterface;

    WifiStateManager();

    virtual ~WifiStateManager() { }

    enum WifiState
    {
        //Wifi state unknown, WifiNetworkInterface needs to be assigned with 
        //setNetworkManager.
        missingNetworkInterface,
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
        //Wifi device is connected to an access point.
        connected,
        //Wifi device is closing its connection to an access point.
        disconnecting,
        //Wifi is disconnecting from one access point in order to connect to 
        //another.
        switchingConnection,
        //WifiStateManager not available.
        noStateManager
    };

#ifdef JUCE_DEBUG    

    /**
     * Convert a WifiState to a string for debugging.
     */
    static String wifiStateString(WifiState state)
    {
        switch (state)
        {
            case missingNetworkInterface:
                return "missingNetworkInterface";
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
            case connected:
                return "connected";
            case disconnecting:
                return "disconnecting";
            case switchingConnection:
                return "switchingConnection";
            case noStateManager:
                return "noStateManager";
        }
    }
#endif

    /**
     * Assigns a WifiNetworkInterface to this WifiStateManager. The state 
     * manager will take ownership of the network  interface, deleting it on 
     * destruction or when a new  interface is provided through 
     * setNetworkInterface.
     *
     * @param interface  Takes responsibility for communicating with the Wifi 
     *                    device.
     */
    void setNetworkInterface(NetworkInterface* interface);

    /**
     * Gets the current state of the wifi device.
     * 
     * @return the current state of the wifi device, or missingNetworkInterface
     *          if the WifiNetworkInterface hasn`t been set.
     */
    WifiState getWifiState();

    /**
     * Add a listener to the list of objects receiving updates whenever Wifi 
     * state changes.
     *
     * If the listener has already been added to another  WifiStateManager, it 
     * will be removed from that state manager  before it's added to this one.  
     * In debug builds this will also print a warning and fail an assertion, as 
     * there really  shouldn`t be more than one WifiStateManager.
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
     * Gets the connected wifi access point, if one exists.
     * 
     * @return the current connected Wifi access point, or a null 
     *          WifiAccessPoint if there is no connection.
     */
    WifiAccessPoint getConnectedAP();

    /**
     * Gets the connecting wifi access point, if one exists.
     * 
     * @return the current connecting WifiAccessPoint if one is found, or a 
     *          null WifiAccessPoint if wifi is not connecting.
     */
    WifiAccessPoint getConnectingAP();

    /**
     * Gets all access points visible to the wifi device.
     * 
     * @return all wifi access points currently detected. If wifi is disabled, 
     *          this will return an empty list.
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
     * @param psk        The wifi key for toConnect, or the empty string
     *                    if toConnect isn't secured.
     */
    void connectToAccessPoint(const WifiAccessPoint& toConnect,
            String psk = String());

    /**
     * If currently connected to or trying to connect to a particular access 
     * point, that connection will be closed or canceled.
     * 
     * @param toDisconnect  Specifies the access point that should not be 
     *                       connected.
     */
    void disconnect(const WifiAccessPoint& toDisconnect);

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



private:

    class WifiResource : public ResourceManager::SharedResource, public Timer,
    private WindowFocus::Listener
    {
    public:
        /**
         * @param timerLock  The static resource lock used to control access to
         *                    the WifiResource instance.  This object should
         *                    only use this in the timer callback, anywhere
         *                    else it will cause a deadlock.
         */
        WifiResource(CriticalSection& timerLock);

        /**
         * On destruction, the WifiResource removes all references to itself 
         * from its Listener objects, and destroys its WifiNetworkInterface.
         */
        virtual ~WifiResource();

        /**
         * Assigns a WifiNetworkInterface to this WifiStateManager. The state 
         * manager will take ownership of the network  interface, deleting it on 
         * destruction or when a new  interface is provided through 
         * setNetworkInterface. 
         * @see WifiStateManager::setNetworkInterface()
         */
        void setNetworkInterface(NetworkInterface* interface);

        /**
         * Gets the current state of the wifi device.
         * @see WifiStateManager::getWifiState()
         */
        WifiState getWifiState();

        /**
         * Updates the current wifi state and notify all listeners.
         */
        void setWifiState(WifiState state);

        /**
         * Add a listener to the list of objects receiving updates whenever 
         * Wifi state changes.  
         * @see WifiStateManager::addListener()
         */
        void addListener(WifiStateManager::Listener* listener);

        /**
         * Searches the list of registered listeners for a particular one, and 
         * removes it if it's found. 
         * @see WifiStateManager::removeListener()
         */
        void removeListener(WifiStateManager::Listener* listener);

        /**
         * Gets the connected wifi access point, if one exists. 
         * @see WifiStateManager::getConnectedAP()
         */
        WifiAccessPoint getConnectedAP();

        /**
         * Gets the connecting wifi access point, if one exists. 
         * @see WifiStateManager::getConnectingAP()
         */
        WifiAccessPoint getConnectingAP();

        /**
         * Gets all access points visible to the wifi device. 
         * @see WifiStateManager::getVisibleAPs()
         */
        Array<WifiAccessPoint> getVisibleAPs();

        /**
         * Checks if wifi is enabled. 
         * @see WifiStateManager::isEnabled()
         */
        bool isEnabled();

        /**
         * Checks if the wifi device is connected to an access point. 
         * @see WifiStateManager::isConnected()
         */
        bool isConnected();

        /**
         * Attempts to open a connection to a wifi access point. This will fail 
         * if wifi is disabled, the access point is invalid, or the psk is 
         * wrong. 
         * @see WifiStateManager::connectToAccessPoint()
         */
        void connectToAccessPoint(const WifiAccessPoint& toConnect, String psk);

        /**
         * If currently connected to or trying to connect to a particular access 
         * point, that connection will be closed or canceled.
         * @see WifiStateManager::disconnect()
         */
        void disconnect(const WifiAccessPoint& toDisconnect);

        /**
         * If wifi is currently disabled, this will enable it.  
         * @see WifiStateManager::enableWifi()
         */
        void enableWifi();

        /**
         * If wifi is currently enabled, this will disable it. 
         * @see WifiStateManager::disableWifi()
         */
        void disableWifi();

        /**
         * Checks the validity of the current wifi state, possibly changing the
         * state to missingNetworkInterface if necessary.
         * 
         * @return true iff the current wifi state is noStateManager or 
         * missingNetworkInterface
         */
        bool invalidWifiState();

        //Milliseconds to wait before assuming that enabling or
        //disabling wifi has failed.
        static const constexpr int wifiEnabledChangeTimeout = 10000;

        //Milliseconds to wait before assuming that connecting to
        //or disconnecting from a wifi access point has failed.
        static const constexpr int wifiConnectionTimeout = 35000;

        WifiAccessPoint getPendingAp()
        {
            return pendingConnection.ap;
        }

        String getPendingPsk()
        {
            return pendingConnection.psk;
        }

        void setPendingConnection(WifiAccessPoint ap, String psk)
        {
            pendingConnection.ap = ap;
            pendingConnection.psk = psk;
        }

        WifiAccessPoint getCancelledConnection()
        {
            return cancelledConnection;
        }

        void setCancelledConnection(WifiAccessPoint ap)
        {
            cancelledConnection = ap;
        }

    private:
        /**
         * Whenever a wifi operation is attempted, the timer is set to
         * the appropriate timeout period. If the timer goes off before 
         * the WifiNetworkInterface responds, the WifiStateManager will
         * assume that the wifi operation failed.
         * This method acquires the WifiStateManager's stateLock.
         */
        void timerCallback() override;

        /**
         * When window focus is lost, the timer will be canceled.  
         */
        void windowFocusLost() override;

        /**
         * When window focus is gained, if the network interface has been assigned,
         * the wifi state will be updated.
         */
        void windowFocusGained() override;

        /**
         * holds any pending connection data
         */
        struct
        {
            WifiAccessPoint ap;
            String psk;
        } pendingConnection;

        CriticalSection& timerLock;

        //Holds the last canceled connection, so the state manager knows to 
        //immediately disconnect if the networkInterface announces a connection to
        //this access point.
        WifiAccessPoint cancelledConnection;

        //Holds the last known state of the wifi device.
        WifiState wifiState = missingNetworkInterface;

        //Holds the NetworkInterface responsible for communicating with the wifi
        //device
        ScopedPointer<NetworkInterface> networkInterface = nullptr;

        //All objects tracking the current wifi state.
        Array<WifiStateManager::Listener*> listeners;
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
        friend class WifiResource;

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

    /**
     * WifiNetworkInterface is an abstract interface for communication between 
     * the WifiStateManager and whatever object handles direct interaction with
     * the wifi device.
     */
    class NetworkInterface
    {
    public:
        friend class WifiStateManager;

        NetworkInterface() { }

        virtual ~NetworkInterface() { }

    protected:

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
         * Gets the connected wifi access point, if one exists.
         * 
         * @return the current connected WifiAccessPoint if one is found, or a 
         *          null WifiAccessPoint if not connected.
         */
        virtual WifiAccessPoint getConnectedAP() = 0;

        /**
         * Gets the connecting wifi access point, if one exists.
         * 
         * @return the current connecting WifiAccessPoint if one is found, or a
         *          null WifiAccessPoint if not connecting.
         */
        virtual WifiAccessPoint getConnectingAP() = 0;

        /**
         * Scans for and returns nearby wifi access points.
         * 
         * @return the list of all wifi access points close enough to detect.
         */
        virtual Array<WifiAccessPoint> getVisibleAPs() = 0;


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
         * If a wifi access point is currently connected, attempt to disconnect
         * from that access point.
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
        void signalWifiConnected(const WifiAccessPoint& connectedAP);

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
         * @return true if the state manager is missing or in an invalid state.
         */
        bool invalidWifiState();
    private:
        WifiStateManager::WifiResource* wifiResource = nullptr;
        CriticalSection* wifiLock = nullptr;
    };

private:

    /**
     * Checks the validity of the current wifi state, possibly changing the
     * state to missingNetworkInterface if necessary.
     * 
     * @return true iff the current wifi state is noStateManager or 
     * missingNetworkInterface
     */
    bool invalidWifiState();

    //ResourceManager shared object and lock;
    static ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static CriticalSection stateLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiStateManager)
};

