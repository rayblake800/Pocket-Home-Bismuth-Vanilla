/**
 * @file WifiStatusJson.h
 * 
 * TODO: documentation
 */
#pragma once

#include "WifiStatus.h"

class WifiStatusJson : public WifiStatus {
public:
    WifiStatusJson();
    ~WifiStatusJson() override;

    Array<WifiAccessPoint> nearbyAccessPoints() override;
    WifiAccessPoint getConnectedAccessPoint() const override;
    bool isEnabled() const override;
    bool isConnected() const override;
        
    /**
     * @return true iff wifi is currently being enabled.
     */
    bool isTurningOn() const override;
    
    /**
     * @return true iff wifi is currently being disabled.
     */
    isTurningOff() const override;
    
    /**
     * @return true iff wifi is attempting to connect to an access point.
     */
    bool isConnecting() const override;
    
    /**
     * @return true iff wifi is attempting to disconnect from an access point. 
     */
    bool isDisconnecting() const override;

    void enableWifi() override;
    void disableWifi() override;
    void setConnectedAccessPoint(const WifiAccessPoint& ap,
            String psk = String()) override;
    void disconnect() override;

private:

    void run() override {
    }


    WifiAccessPoint connectedAP;
    WifiAccessPoint waitingToConnect;
    String waitingPsk;
    bool enabled = false;
    bool connected = false;
    bool connecting = false;
    bool disconnecting = false;
    bool turningOn = false;
    bool turningOff = false;
    
    CriticalSection wifiLock;
};
