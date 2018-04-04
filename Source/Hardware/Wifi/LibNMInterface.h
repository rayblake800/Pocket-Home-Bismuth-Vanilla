
/**
 * @file LibNMInterface.h
 * 
 * Connects to the libnm NetworkManager to track and control wifi device wlan0.
 * As a WifiStateManager::NetworkInterface, only the WifiStateManager should
 * directly interact with this class.
 */
#pragma once

#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include "WindowFocus.h"
#include "WifiEventHandler.h"
#include "WifiStateManager.h"

class LibNMInterface : public WifiStateManager::NetworkInterface,
private WindowFocus::Listener, private WifiEventHandler
{
public:
    LibNMInterface();

    virtual ~LibNMInterface();

protected:
    /**
     * Check the NMDevice state to see if wifi is enabled.
     * 
     * @return true iff the wifi device is currently turned on.
     */
    bool isWifiEnabled() override;

    /**
     * Check the NMDevice state to see if wifi is connecting.
     * 
     * @return true iff the wifi device is currently connecting to an access 
     *          point.
     */
    bool isWifiConnecting() override;

    /**
     * Check the NMDevice state to see if wifi is connected.
     * 
     * @return true iff the wifi device is currently connected to an access 
     *          point.
     */
    bool isWifiConnected() override;

    /**
     * Request information on the connected access point from the NMDevice.
     * 
     * @return a WifiAccessPoint object representing the access point connected
     *          to the wifi device, or WifiAccessPoint() if not connected.
     */
    WifiAccessPoint getConnectedAP() override;

    /**
     * Request information on the connecting access point from the NMDevice.
     * 
     * @return a WifiAccessPoint object representing the access point 
     *          connecting to the wifi device, or WifiAccessPoint() if not 
     *          connecting.
     */
    WifiAccessPoint getConnectingAP() override;

    /**
     * Request information on all wifi access points detected by the NMDevice.
     * 
     * @return a list of all visible access points within range of the wifi
     *          device, or an empty list if wifi is disabled.
     */
    Array<WifiAccessPoint> getVisibleAPs() override;

    /**
     * Asynchronously attempt to connect to a wifi access point.
     * 
     * @param toConnect  The access point that should be connected to. 
     * 
     * @param psk        The access point's security key, or the empty string 
     *                    if toConnect is unsecured.
     */
    void connectToAccessPoint(const WifiAccessPoint& toConnect,
            String psk = String()) override;

    /**
     * Asynchronously checks the wifi device list, connection state, and 
     * active and pending connections.
     */
    void updateAllWifiData();

    /**
     * Asynchronously close the currently active connection, if one exists.
     */
    void disconnect() override;

    /**
     * Asynchronously turns on the wifi radio.
     */
    void enableWifi() override;

    /**
     * Asynchronously turns off the wifi radio.
     */
    void disableWifi() override;

private:
    /**
     * Attaches LibNMInterface callback functions to the network manager client
     * and wifi device in order to receive updates when wifi actions occur.
     */
    void nmSignalSubscribe();


    /**
     * Called whenever the application window gains focus in order to subscribe
     * to wifi signals.
     */
    void windowFocusGained() override;

    /**
     * Called whenever the application window loses focus in order to
     * un-subscribe from wifi signals.
     */
    void windowFocusLost() override;

    /**
     * Generate a unique hash string identifying a wifi access point.
     * Adapted from network-manager-applet src/utils/utils.c 
     * 
     * @param ap
     */
    static char* hashAP(NMAccessPoint* ap);

    /**
     * Checks the security on a libnm access point object.
     * 
     * @param  ap
     * 
     * @return true iff ap is a secure access point.
     */
    static bool resolveAPSecurity(NMAccessPoint* ap);

    /**
     * Creates a WifiAccessPoint from a libnm access point object.
     * 
     * @param  ap
     * 
     * @return WifiAccessPoint data read from a NMAccessPoint
     */
    static WifiAccessPoint createNMWifiAccessPoint(NMAccessPoint* ap);


    /**
     * Closes a specific wifi connection on wifi device wlan0.  This should
     * only be called from the GLib main event loop.
     * 
     * @param conn  the connection to close
     */
    void removeNMConnection(NMActiveConnection* conn = nullptr);

    /**
     * Checks the validity of a potential WEP access point key string.
     * 
     * @param  key
     * 
     * @return true iff key has the correct format for a WEP key.
     */
    static bool isValidWEPKeyFormat(String key);

    /**
     * Checks the validity of a potential WEP passphrase.
     * 
     * @param phrase
     * 
     * @return true iff phrase has the correct format for a WEP passphrase.
     */
    static bool isValidWEPPassphraseFormat(String phrase);
    
    /**
     * Requests the current connecting access point from the network manager.
     * This should only be called from the GLib main event loop.
     * 
     * @return the current connecting access point
     */
    NMAccessPoint* getConnectingNmAp();
        
    /**
     * Requests the current connected access point from the network manager.
     * This should only be called from the GLib main event loop.
     * 
     * @return the current connected access point
     */
    NMAccessPoint* getConnectedNmAp();
        
    /**
     * Requests the current list of visible access points from the network 
     * manager.  This should only be called from the GLib main event loop.
     * 
     * @return the current list of visible access points
     */
    Array<NMAccessPoint*> getVisibleNmAps();

    /**
     *######################## LibNM Callbacks #################################
     * These functions run asynchronously on the main GLib event loop in order
     * to update wifi data.  Only these functions should interact with GLib
     * sources to do anything other than adding new signal sources/handlers
     * or removing signal sources/handlers.  All of these functions will
     * acquire the wifiLock. 
     */

    /**
     * This callback runs after libnm attempts to open a network connection. It
     * updates the connecting access point, and catches any immediate connection
     * errors.  If it encounters an error, it will inform the Wifi state manager
     * with signalConnectionFailed()
     * 
     * @param client       The network manager client object.
     * 
     * @param active       The attempted wifi connection.
     * 
     * @param path         The device path for the wifi access point.
     * 
     * @param err          This will hold error data if the connection attempt 
     *                      failed.
     * 
     * @param nmInterface  A pointer to the LibNMInterface object.
     */
    static void handleConnectionAttempt(
            NMClient* client,
            NMActiveConnection* active,
            const char* path,
            GError* err,
            LibNMInterface* nmInterface);

    /**
     * This callback runs when the network manager client signals that wifi
     * was enabled or disabled. Depending on which event occurs, this will 
     * inform the Wifi state manager with either signalWifiEnabled() or 
     * signalWifiDisabled().
     * 
     * @param client       The NMClient signaling the change.
     * 
     * @param nmInterface   A pointer to the LibNMInterface object.
     */
    static void handleWifiEnabledChange(
            NMClient* client,
            LibNMInterface* nmInterface);

    /**
     * This callback runs when device wlan0 signals that its state has changed.
     * Depending on what state it enters, the nmInterface may send an update to
     * the WifiStateManager.
     * 
     * @param nmDevice     Device wlan0, the source of the signal.
     * 
     * @param nmInterface  A pointer to the LibNMInterface object.
     */
    static void handleStateChange(
            NMDevice* nmDevice,
            LibNMInterface* nmInterface);


    /**
     * This callback runs when device wlan0 signals that its active connection 
     * has changed. This will check the status of the connection, and call 
     * signalWifiConnected() or signalWifiDisconnected()
     * 
     * @param wifiDevice    The wifi device wlan0.
     * 
     * @param nmInterface   A pointer to the LibNMInterface object.
     */
    static void handleConnectionChange(
            NMDeviceWifi* wifiDevice,
            LibNMInterface* nmInterface);

    /**
     * This callback runs when device wlan0 signals that a new wifi access point
     * is visible or an old one has been lost.  This will update the 
     * nmInterface's record of visible access points.
     * 
     * @param wifiDevice    The wifi device wlan0.
     * 
     * @param nmInterface   A pointer to the LibNMInterface object.
     */
    static void handleApChange(
            NMDeviceWifi* wifiDevice,
            LibNMInterface* nmInterface);

    //Prevent concurrent access to the wifi data.  This should be locked
    //any time any of the data members listed beneath it is accessed.
    CriticalSection wifiLock;

    NMDeviceState lastNMState = NM_DEVICE_STATE_UNKNOWN;

    WifiAccessPoint connectedAP;
    WifiAccessPoint connectingAP;
    Array<WifiAccessPoint> visibleAPs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LibNMInterface)
};
