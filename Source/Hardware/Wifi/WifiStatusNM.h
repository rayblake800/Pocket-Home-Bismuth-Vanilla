
/**
 * @file WifiStatusNM.h
 * 
 * Connects to NetWorkManager to track and control wifi.
 */
#pragma once
#ifdef LINUX

#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include "WifiStateManager.h"

class WifiStatusNM : public WifiStateManager::NetworkInterface,
private Thread, private WindowFocus::Listener
{
public:
    WifiStatusNM();
    virtual ~WifiStatusNM();

    /**
     */
    bool isWifiDeviceEnabled() override;

    /**
     */
    bool isWifiConnecting() override;

    /**
     */
    bool isWifiConnected() override;

    /**
     */
    WifiAccessPoint getConnectedAP() override;

    /**
     */
    WifiAccessPoint getConnectingAP() override;

    /**
     */
    Array<WifiAccessPoint> getVisibleAPs() override;


    /**
     */
    void connectToAccessPoint(WifiAccessPoint toConnect,
            String psk = String()) override;

    /**
     */
    void disconnect() override;

    /**
     */
    void enableWifi() override;

    /**
     */
    void disableWifi() override;

private:

    /**
     * Attempt to open a connection to the network manager.
     * @return true iff the connection was made and nmClient was initialized.
     */
    bool connectToNetworkManager();

    /**
     * Wifi thread loop:
     * While the wifi status thread is running, regularly check the network
     * manager for wifi network changes.
     */
    void run() override;

    /**
     * Generate a unique hash string identifying a wifi access point.
     * Adapted from network-manager-applet src/utils/utils.c 
     * @param ap
     */
    char* hashAP(NMAccessPoint* ap);

    /**
     * @param ap
     * @return true iff ap is a secure access point.
     */
    bool resolveAPSecurity(NMAccessPoint* ap);

    /**
     * @param ap
     * @return WifiAccessPoint data read from a NMAccessPoint
     */
    WifiAccessPoint createNMWifiAccessPoint(NMAccessPoint* ap);

    /**
     * @param wdev
     * @return WifiAccessPoint data of the access point connected to wdev, or
     * WifiAccessPoint::null() if no connection is found.
     */
    WifiAccessPoint getNMConnectedAP(NMDeviceWifi *wdev);

    /**
     * Close a specific wifi connection
     * @param conn the connection to close
     */
    void removeNMConnection(NMActiveConnection* conn = nullptr);

    /**
     * @param key
     * @return true iff key has the correct format for a WEP key.
     */
    bool isValidWEPKeyFormat(String key);

    /**
     * @param phrase
     * @return true iff phrase has the correct format for a WEP passphrase.
     */
    bool isValidWEPPassphraseFormat(String phrase);


    //##############  NetworkManager callback functions ###########################
    // These are called by the NetworkManager after wifi state changes.
    static void handle_add_and_activate_finish(NMClient *client,
            NMActiveConnection *active,
            const char* path,
            GError *err,
            gpointer user_data);
    static void handle_wireless_enabled(WifiStatusNM *wifiStatus);
    static void handle_wireless_connected(WifiStatusNM *wifiStatus);
    static void handle_active_access_point(WifiStatusNM *wifiStatus);
    static void handle_changed_access_points(WifiStatusNM *wifiStatus);

    CriticalSection wifiLock;
    Array<WifiStatus::Listener*> listeners;
    WifiAccessPoint connectedAP;
    bool enabled = false;
    bool connected = false;
    bool connecting = false;
    NMClient* nmClient = nullptr;
    NMDevice* nmDevice = nullptr;
    GMainContext *context;
};
#endif // LINUX
