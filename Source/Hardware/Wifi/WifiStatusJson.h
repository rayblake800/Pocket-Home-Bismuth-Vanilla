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

    void enableWifi() override;
    void disableWifi() override;
    void setConnectedAccessPoint(const WifiAccessPoint& ap,
            String psk = String()) override;
    void disconnect() override;

private:

    void run() override {
    }


    WifiAccessPoint connectedAP;
    bool enabled = false;
    bool connected = false;
};
