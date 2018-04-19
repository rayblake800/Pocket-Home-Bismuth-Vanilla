/**
 * @file WifiEventHandler
 * 
 * @brief WifiEventHandler interacts with LibNM and the GLib event loop
 *         to simplify the use of LibNM functionality.
 */
#pragma once
#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include <nm-remote-connection.h>
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
    WifiAccessPoint::Ptr findConnectedAP();
    
        
    /**
     * Gets the current connecting access point from the network manager wifi
     * device.
     * 
     * @return the current connecting access point, or the void access point if
     *         none is found.
     */
    WifiAccessPoint::Ptr findConnectingAP();
    
    /**
     * Returns the list of visible wifi access points. If multiple access points
     * share a connection, this will only return the one with the best signal
     * strength.  Hidden access points with no saved connection will also not be
     * included in the list.
     */
    Array<WifiAccessPoint::Ptr> findVisibleAPs();
    
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
     */
    void initConnection(WifiAccessPoint::Ptr toConnect, String psk);
    
    /**
     * Shuts down the active wifi connection.
     */
    void closeActiveConnection();
    
    /**
     * Shuts down any wifi connection currently being activated.
     */
    void closeActivatingConnection();
    
    /**
     * A callback function to run whenever initConnection successfully starts
     * opening a connection.
     * 
     * @param connectingAP
     */
    virtual void connectingCallback(WifiAccessPoint::Ptr connectingAP) = 0;
    
    /**
     * A callback function to run whenever initConnection fails to open a new
     * connection.
     */
    virtual void connectionFailureCallback() = 0;
    
    /**
     * A callback function to run whenever a connection fails due to invalid
     * security settings.
     */
    virtual void invalidSecurityCallback() = 0;
    
    /**
     * A callback function to run whenever the wifi device is enabled
     * or disabled.
     * 
     * @param isEnabled  Indicates if the wifi device is on or off.
     */
    virtual void wifiEnablementChangeCallback(bool isEnabled) = 0;
    
    /**
     * A callback function to run whenever the list of wifi access points is 
     * updated.
     * 
     * @param visibleAPs  The updated visible access point list.
     */
    virtual void apUpdateCallback(Array<WifiAccessPoint::Ptr> visibleAPs) = 0;
    
    /**
     * A callback function to run whenever the wifi device state changes.
     * 
     * @param newState     The updated wifi device state.
     * 
     * @param stateReason  The reason for the device state change.
     */
    virtual void stateUpdateCallback
    (NMDeviceState newState, NMDeviceStateReason stateReason) = 0;
    
        
    /**
     * A callback function to run whenever the wifi connection changes.
     * 
     * @param connected  The newly connected access point, or the void access
     *                   point if wifi just disconnected.
     */
    virtual void connectionUpdateCallback (WifiAccessPoint::Ptr connected) = 0;
    
    
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
    /**
     * Loads the list of all saved network connections as a GSList* of 
     * NMRemoteConnection* objects.
     * 
     * @return  the list of saved connections. The list object will need to be
     *          freed with g_slist_free.
     */
    GSList* getSavedConnections();
    
    //Internal signal handlers:
    
    static void handleConnectionAttempt(
        NMClient *client,
        NMActiveConnection *active,
        const char* path,
        GError *err,
        LibNMHandler* nmHandler);
    
    static void handleKnownConnectionAttempt(
        NMClient *client,
        NMActiveConnection *active,
        GError *err,
        LibNMHandler* nmHandler);
    
    static void handleWifiEnabledChange(LibNMHandler* nmHandler);
    
    static void handleStateChange(LibNMHandler* nmHandler);
    
    static void handleApAdded(LibNMHandler* nmHandler);
    
    static void handleApRemoved(LibNMHandler* nmHandler);
    
    static void handleConnectionChange(LibNMHandler* nmHandler);
    
    static void handleClientRemoved(LibNMHandler* self, GObject* client);
    
    static void handleDeviceRemoved(LibNMHandler* self, GObject* device);
    
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

    NMClient* nmClient = nullptr;
    NMDevice* nmDevice = nullptr;
    
    NMActiveConnection* activatingConn = nullptr;
    
    Array<NMRemoteConnection*> savedConnections;
    
    Array<WifiAccessPoint::Ptr, CriticalSection> visibleAPs;

    Array<gulong, CriticalSection> clientSignalHandlers;
    Array<gulong, CriticalSection> deviceSignalHandlers;
    Array<gulong, CriticalSection> wifiDeviceSignalHandlers;
};