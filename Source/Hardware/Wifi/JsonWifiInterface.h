/**
 * @file JsonWifiInterface.h
 * 
 * Simulates interaction with a wifi device using fake access point data.  
 * Access point data is loaded from the wifi.json file in the pocket-home assets
 * folder.  As a WifiStateManager::NetworkInterface, only the WifiStateManager
 * should directly interact with this class.
 */
#pragma once

#include "WifiStateManager.h"

class JsonWifiInterface : public WifiStateManager::NetworkInterface
{
public:
    JsonWifiInterface(CriticalSection& wifiLock);
    virtual ~JsonWifiInterface();

protected:
    bool wifiDeviceFound() override;
    
    /**
     * Checks if the simulated wifi device is enabled.
     * 
     * @return true iff simulated wifi was enabled with the enableWifi() 
     *          method.
     */
    bool isWifiEnabled() override;

    /**
     * Checks if a connection event is currently being simulated.
     * 
     * @return true iff connectToAccessPoint() was called but the connection
     *          has not yet completed or failed.
     */
    bool isWifiConnecting() override;
    
    /**
     * Checks if a simulated connection has been created.
     * 
     * @return true iff a connected WifiAccessPoint has been set.
     */
    bool isWifiConnected() override;
        
    /**
     * Checks if the simulated connection requires a psk
     */
    bool isPSKNeeded() override;

    /**
     * Returns the connected access point.
     * 
     * @return the access point, or WifiAccessPoint() if not connected.
     */
    WifiAccessPoint getConnectedAP() override;

    /**
     * Returns the connecting access point.
     * 
     * @return the access point, or WifiAccessPoint() if wifi is not 
     *          connecting.
     */
    WifiAccessPoint getConnectingAP() override;

    /**
     * Returns the entire list of access points defined in wifi.json.
     * 
     * @return the list of fake access points, or the empty list if the 
     *          simulated wifi device is disabled.
     */
    Array<WifiAccessPoint> getVisibleAPs() override;


    /**
     * Triggers a wifi connection event.  This will set a timer to simulate
     * the connection's success or failure after a randomized delay of nine to
     * twenty-four seconds.
     * 
     * @param toConnect  If this has the SSID "MyFi", a successful connection
     *                    will be simulated.  For all other access points, a
     *                    failed connection will be simulated.
     * 
     * @param psk        The security key for WifiAccessPoint toConnect, or the
     *                    empty string if the connection is not secured. This
     *                    value will be noted in debug output, but otherwise
     *                    is ignored.
     */
    void connectToAccessPoint(const WifiAccessPoint& toConnect,
            String psk = String()) override;

    /**
     * If a connection is pending, cancel it.
     */
    void stopConnecting() override;
    
    /**
     * Triggers a simulated wifi disconnection event.  If a simulated connection
     * exists, after a randomized delay of no more than six seconds, a 
     * disconnection event will trigger.
     */
    void disconnect() override;

    /**
     * Turns on the simulated wifi device.  This will trigger a wifi enabled
     * event after a randomized delay of no more than six seconds.
     */
    void enableWifi() override;

    /**
     * Turns off the simulated wifi device.  This will trigger a wifi disabled
     * event after a randomized delay of no more than six seconds.
     */
    void disableWifi() override;

private:
    //Simulated wifi state variables.
    WifiAccessPoint connectedAP;
    WifiAccessPoint waitingToConnect;
    bool enabled = false;
    bool connected = false;
    bool disconnecting = false;
    bool turningOn = false;
    bool turningOff = false;
    
    //Disallows concurrent modification
    CriticalSection& wifiLock;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JsonWifiInterface)
};
