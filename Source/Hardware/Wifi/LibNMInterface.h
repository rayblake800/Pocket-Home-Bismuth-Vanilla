
/**
 * @file LibNMInterface.h
 * 
 * Connects to the libnm NetworkManager to track and control wifi device wlan0.
 * As a WifiStateManager::NetworkInterface, only the WifiStateManager should
 * directly interact with this class.
 */
#pragma once
#ifdef LINUX

#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include "WindowFocus.h"
#include "WifiStateManager.h"

class LibNMInterface : public WifiStateManager::NetworkInterface,
private Thread, private WindowFocus::Listener
{
public:
    LibNMInterface();

    virtual ~LibNMInterface() { }

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
     * Attempt to connect to a wifi access point.
     * 
     * @param toConnect  The access point that should be connected to. 
     * 
     * @param psk       The access point's security key, or the empty string if
     *                   toConnect is unsecured.
     */
    void connectToAccessPoint(const WifiAccessPoint& toConnect,
            String psk = String()) override;

    /**
     * Close the currently active connection, if one exists.
     */
    void disconnect() override;

    /**
     * Turns on the wifi radio.
     */
    void enableWifi() override;

    /**
     * Turns off the wifi radio.
     */
    void disableWifi() override;

private:

    /**
     * This callback runs after libnm attempts to open a network connection. It
     * primarily serves to catch immediate connection errors.  If it encounters
     * an error, it will inform the Wifi state manager with 
     * signalConnectionFailed()
     * 
     * @param client     The network manager client object.
     * 
     * @param active     The attempted wifi connection.
     * 
     * @param path       The device path for the wifi access point.
     * 
     * @param err        This will hold error data if the connection attempt 
     *                    failed.
     * 
     * @param user_data  A pointer to the LibNMInterface object. This can be
     *                    directly typecast to LibNMInterface*
     */
    static void handleConnectionAttempt(NMClient* client,
            NMActiveConnection* active,
            const char* path,
            GError* err,
            gpointer user_data);

    /**
     * This callback runs when libnm detects that wifi was enabled or disabled.
     * Depending on which event occurs, this will inform the Wifi state manager
     * with either signalWifiEnabled() or signalWifiDisabled().
     * 
     * @param wifiStatus   A pointer to the LibNMInterface object.
     */
    static void handleWifiEnabledChange(LibNMInterface* wifiStatus);

    /**
     * This callback runs when libnm signals that a connection attempt is 
     * finished, or the active connection has changed. This will check the 
     * status of the connection, and call signalWifiConnected(), 
     * signalWifiDisconnected(), or signalConnectionFailed() depending on the 
     * results.
     * 
     * @param wifiStatus   A pointer to the LibNMInterface object.
     */
    static void handleConnectionEvent(LibNMInterface* wifiStatus);

    /**
     * Attempt to open a connection to the network manager, and find wifi 
     * network device wlan0.
     * 
     * @return true iff nmClient and nmDevice were successfully initialized.
     */
    bool connectToNetworkManager();

    /**
     * Wifi thread loop:
     * While the wifi status thread is running, regularly check the network
     * manager for wifi network changes.
     */
    void run() override;

    /**
     * called whenever the application window gains focus.
     */
    void windowFocusGained() override;

    /**
     * called whenever the application window loses focus.
     */
    void windowFocusLost() override;

    /**
     * Generate a unique hash string identifying a wifi access point.
     * Adapted from network-manager-applet src/utils/utils.c 
     * 
     * @param ap
     */
    char* hashAP(NMAccessPoint* ap);

    /**
     * Checks the security on a libnm access point object.
     * 
     * @param  ap
     * 
     * @return true iff ap is a secure access point.
     */
    bool resolveAPSecurity(NMAccessPoint* ap);

    /**
     * Creates a WifiAccessPoint from a libnm access point object.
     * 
     * @param  ap
     * 
     * @return WifiAccessPoint data read from a NMAccessPoint
     */
    WifiAccessPoint createNMWifiAccessPoint(NMAccessPoint* ap);


    /**
     * Closes a specific wifi connection on wifi device wlan0.
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
    bool isValidWEPKeyFormat(String key);

    /**
     * Checks the validity of a potential WEP passphrase.
     * 
     * @param phrase
     * 
     * @return true iff phrase has the correct format for a WEP passphrase.
     */
    bool isValidWEPPassphraseFormat(String phrase);

    //Prevent concurrent access to the wifi device.
    CriticalSection wifiLock;
    
    //libnm network manager objects
    NMClient* nmClient = nullptr;
    NMDevice* nmDevice = nullptr;
    GMainContext *context;
};
#endif // LINUX
