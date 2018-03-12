/**
 * @file WifiStatusJson.h
 * 
 * TODO: documentation
 */
#pragma once

#include "../Utils.h"
#include "WifiStatus.h"

class WifiStatusJson : public WifiStatus {
public:
  WifiStatusJson();
  ~WifiStatusJson() override;
  
  Array<WifiAccessPoint> nearbyAccessPoints() override;
  WifiAccessPoint connectedAccessPoint() const override;
  bool isEnabled() const override;
  bool isConnected() const override;

  void addListener(Listener* listener) override;
  void clearListeners() override;

  void enableWifi() override;
  void disableWifi() override;
  void setConnectedAccessPoint(const WifiAccessPoint& ap,
          String psk = String()) override;
  void disconnect() override;

private:
  Array<Listener*> listeners;
  WifiAccessPoint connectedAP;
  bool enabled = false;
  bool connected = false;
};
