/**
 * @file WifiEventHandler
 * 
 * @brief WifiEventHandler interacts with LibNM and the GLib event loop
 *         to simplify the use of LibNM functionality.
 */
#pragma once
#include <NetworkManager.h>
#include <map>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include "JuceHeader.h"
#include "WifiAccessPoint.h"
#include "GLibSignalHandler.h"

class LibNMHandler
{
public:
    /**
     * Initializes LibNM functionality.
     */
    LibNMHandler();

    /**
     * Shuts LibNM functionality and removes all callback functions.
     */
    virtual ~LibNMHandler();

protected:
    /**
     * Checks if the network manager and the wifi device are available
     * 
     * @return true iff nmClient, nmDevice, and nmWifiDevice initialized
     *          successfully.
     */
    bool isWifiAvailable();
    
    /**
     * @return true iff the wifi device is enabled. 
     */
    bool checkWifiEnabled();
    
    /**
     * @return true iff the wifi device is connecting to an access point. 
     */
    bool checkWifiConnecting();
    
    /**
     * @return true iff the wifi device is connected to an access point. 
     */
    bool checkWifiConnected();
    
    /**
     * Gets the current connected access point from the network manager wifi
     * device.
     * 
     * @return the current connected access point, or the void access point if
     *         none is found.
     */
    WifiAccessPoint findConnectedAP();
    
        
    /**
     * Gets the current connecting access point from the network manager wifi
     * device.
     * 
     * @return the current connecting access point, or the void access point if
     *         none is found.
     */
    WifiAccessPoint findConnectingAP();
    
    /**
     * Updates and returns the list of visible wifi access points.
     */
    Array<WifiAccessPoint> updatedVisibleAPs();
    
    /**
     * Turns the wifi device on or off.
     * 
     * @param wifiEnabled  If true, wifi will be enabled.  If false, wifi will
     *                     be disabled.
     */
    void setWifiEnabled(bool wifiEnabled);
    
    /**
     * Send a request to the wifi device to scan for new access points.
     */
    void requestScan();
    
    /**
     * @return  the current state of the wifi device. 
     */
    NMDeviceState findWifiState();

    /**
     * Attempts to open a connection to an access point.
     * 
     * @param toConnect            The access point to connect to.
     *
     * @param psk                  The access point security key. This parameter
     *                             will be ignored if the access point is
     *                             unsecured.
     * 
     * @param connectingCallback   A callback function to run if connecting
     *                             starts successfully. 
     *
     *  @param failureCallback     A callback function to run if connecting
     *                             fails immediately.
     */
    void initConnection(const WifiAccessPoint& toConnect,
            String psk,
            std::function<void(WifiAccessPoint) > connectingCallback,
            std::function<void() > failureCallback);
    
    /**
     * Shuts down the active wifi connection.
     */
    void closeActiveConnection();
    
    /**
     * Shuts down any wifi connection currently being activated.
     */
    void closeActivatingConnection();
    
    /**
     * Register a callback function to run whenever the wifi device is enabled
     * or disabled.
     * 
     * @param callback  The callback function, which will be passed true if wifi
     *                  is enabled and false if its disabled.
     */
    void setWifiEnablementChangeCallback
    (std::function<void(bool)> callback)
    {
        enabledChangeCallback = callback;
    }
    
    /**
     * Register a callback function to run whenever the list of wifi access
     * points is updated.
     * 
     * @param callback  An AP update callback function, which will be passed the
     *                  updated array of all visible wifi access points.
     */
    void setAPUpdateCallback
    (std::function<void(Array<WifiAccessPoint>)> callback)
    {
        apUpdateCallback = callback;
    }
    
    /**
     * Register a callback function to run whenever the wifi device state
     * changes.
     * 
     * @param callback  A state update callback function, which will be called
     *                  and passed the new wifi state whenever wifi state
     *                  changes.
     */
    void setStateUpdateCallback
    (std::function<void(NMDeviceState)> callback)
    {
        stateUpdateCallback = callback;
    }
    
        
    /**
     * Register a callback function to run whenever the wifi connection
     * changes.
     * 
     * @param callback  A connection update callback function, which will be 
     *                  called and passed the new active wifi connection
     *                  whenever the active connection changes.
     */
    void setConnectionUpdateCallback
    (std::function<void(WifiAccessPoint)> callback)
    {
        connectionUpdateCallback = callback;
    }
    
    /**
     * Attach all signal handlers to the wifi thread, so that they are run
     * whenever the appropriate signals occur.
     */
    void connectSignalHandlers();
    
    /**
     * Remove all signal handlers from the wifi thread, so that they don't 
     * register wifi updates.
     */
    void disconnectSignalHandlers();

private:
    //Internal signal handlers:
    static void handleWifiEnabledChange
    (NMClient* client, LibNMHandler* nmHandler);
    
    static void handleStateChange
    (NMDevice* device, LibNMHandler* nmHandler);
    
    static void handleApAdded
    (NMDeviceWifi* wifiDevice, LibNMHandler* nmHandler);
    
    static void handleApRemoved
    (NMDeviceWifi* wifiDevice, LibNMHandler* nmHandler);
    
    static void handleConnectionChange
    (NMDeviceWifi* wifiDevice, LibNMHandler* nmHandler);
    
    /**
     * Checks if a connection belongs to the wifi device.  This function should
     * be called from the GLib event thread.
     * 
     * @param connection  A connection obtained from the NMClient
     * 
     * @return            True iff the connection is a wifi connection. 
     */
    bool isWifiConnection(NMActiveConnection* connection);
    
    /**
     * Scans for all visible NMAccessPoints and build the data structure 
     * mapping WifiAccessPoint objects to their NMAccessPoints.
     */
    void buildAPMap();
    
    /**
     * Connects a signal handler to the network manager client.
     * 
     * @param signal         The string value of some signal the nmClient
     *                       could send.
     * 
     * @param signalHandler  A callback function to run when the signal is
     *                       sent. This should take the NMClient* as its first
     *                       parameter and the callbackData as its second
     *                       parameter.
     * 
     * @param callbackData   User data to pass to the signalHandler function.
     * 
     * @return  an ID for this signal handler, which can be used to disconnect
     *           the signal handler.  This ID should not be used after the 
     *           signal handler is disconnected.
     */
    gulong nmClientSignalConnect(
            const char* signal,
            GCallback signalHandler,
            gpointer callbackData);

    /**
     * Connects a signal handler to the generic wlan0 device.
     * 
     * @param signal         The string value of some signal the nmDevice
     *                       could send.
     * 
     * @param signalHandler  A callback function to run when the signal is
     *                       sent. This should take the NMDevice* as its first
     *                       parameter and the callbackData as its second
     *                       parameter.
     * 
     * @param callbackData   User data to pass to the signalHandler function.
     * 
     * @return  an ID for this signal handler, which can be used to disconnect
     *           the signal handler.  This ID should not be used after the 
     *           signal handler is disconnected.
     */
    gulong nmDeviceSignalConnect(
            const char* signal,
            GCallback signalHandler,
            gpointer callbackData);


    /**
     * Connects a signal handler to the wifi device wlan0.
     * 
     * @param signal         The string value of some signal the NMDeviceWifi
     *                       could send.
     * 
     * @param signalHandler  A callback function to run when the signal is
     *                       sent. This should take the NMDeviceWifi* as its 
     *                       first parameter and the callbackData as its second
     *                       parameter.
     * 
     * @param callbackData  User data to pass to the signalHandler function.
     * 
     * @return  an ID for this signal handler, which can be used to disconnect
     *          the signal handler.  This ID should not be used after the 
     *          signal handler is disconnected.
     */
    gulong nmWifiDeviceSignalConnect(
            const char* signal,
            GCallback signalHandler,
            gpointer callbackData);

    /**
     * Disconnects a signal handler from the network manager client.
     * 
     * @toDisconnect   A signal handler ID previously returned by 
     *                 nmClientSignalConnect
     */
    void nmClientSignalDisconnect(gulong toDisconnect);

    /**
     * Disconnects a signal handler from the generic wlan0 device
     * 
     * @toDisconnect   A signal handler ID previously returned by 
     *                 nmDeviceSignalConnect
     */
    void nmDeviceSignalDisconnect(gulong toDisconnect);
    
    /**
     * Disconnects a signal handler from the wifi device wlan0
     * 
     * @toDisconnect   A signal handler ID previously returned by 
     *                 nmWifiDeviceSignalConnect
     */
    void nmWifiSignalDisconnect(gulong toDisconnect);

private:

    NMClient* nmClient = nullptr;
    NMDevice* nmDevice = nullptr;
    NMDeviceWifi* nmWifiDevice = nullptr;
    
     std::function<void(bool)> enabledChangeCallback;
    std::function<void(Array<WifiAccessPoint>)> apUpdateCallback;
    std::function<void(NMDeviceState)> stateUpdateCallback;
    std::function<void(WifiAccessPoint)> connectionUpdateCallback;
    
    std::map<WifiAccessPoint,Array<NMAccessPoint*>> accessPointMap;

    Array<gulong, CriticalSection> clientSignalHandlers;
    Array<gulong, CriticalSection> deviceSignalHandlers;
    Array<gulong, CriticalSection> wifiDeviceSignalHandlers;
};