
/**
 * @file LibNMInterface.h
 * 
 * Connects to the libnm NetworkManager to track and control wifi device.
 * As a WifiStateManager::NetworkInterface, only the WifiStateManager should
 * directly interact with this class.
 */
#pragma once

#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include "WindowFocus.h"
#include "LibNMHandler.h"
#include "WifiStateManager.h"

class LibNMInterface : public WifiStateManager::NetworkInterface, 
public LibNMHandler, private WindowFocus::Listener 
{
public:
    LibNMInterface(CriticalSection& wifiLock);

    virtual ~LibNMInterface() { }

protected:
    /**
     * Check if the network manager found a valid wifi device.
     */
    bool wifiDeviceFound() override;
    
    /**
     * Check the NMDevice state to see if wifi is enabled.
     */
    bool isWifiEnabled() override;

    /**
     * Check the NMDevice state to see if wifi is connecting to an access point.
     */
    bool isWifiConnecting() override;

    /**
     * Check the NMDevice state to see if wifi is connected to an access point.
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
     * @return  a WifiAccessPoint object representing the access point 
     *          connecting to the wifi device, or WifiAccessPoint() if not 
     *          connecting.
     */
    WifiAccessPoint getConnectingAP() override;

    /**
     * Request information on all wifi access points detected by the NMDevice.
     * 
     * @return  a list of all visible access points within range of the wifi
     *          device, or an empty list if wifi is disabled.
     */
    Array<WifiAccessPoint> getVisibleAPs() override;

    /**
     * Begin opening a connection to a wifi access point.
     * 
     * @param toConnect  The access point that should be connected to. 
     * 
     * @param psk        The access point's security key. This will be ignored
     *                   if the access point is unsecured.
     */
    void connectToAccessPoint(const WifiAccessPoint& toConnect,
            String psk = String()) override;


    /**
     * Checks the wifi device list, connection state, and active and pending 
     * connections.
     */
    void updateAllWifiData();

    /**
     * Close the currently active connection, if one exists.
     */
    void disconnect() override;
    
    /**
     * Cancel any activating wifi connection.
     */
    void stopConnecting() override;

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
    void setNMCallbacks();


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
    
    //Prevent concurrent access to the wifi data.  This should be locked
    //any time any of the data members listed beneath it is accessed.
    CriticalSection& wifiLock;

    NMDeviceState lastNMState = NM_DEVICE_STATE_UNKNOWN;

    WifiAccessPoint connectedAP;
    WifiAccessPoint connectingAP;
    Array<WifiAccessPoint> visibleAPs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LibNMInterface)
};
