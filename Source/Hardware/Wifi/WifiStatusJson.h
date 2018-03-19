/**
 * @file WifiStatusJson.h
 * 
 * TODO: documentation
 */
#pragma once

#include "WifiStateManager.h"

class WifiStatusJson : public WifiStateManager::NetworkInterface
{
public:
    WifiStatusJson();
    virtual ~WifiStatusJson();

protected:
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
    WifiAccessPoint connectedAP;
    WifiAccessPoint waitingToConnect;
    bool enabled = false;
    bool connected = false;
    bool disconnecting = false;
    bool turningOn = false;
    bool turningOff = false;

    CriticalSection wifiLock;
};
