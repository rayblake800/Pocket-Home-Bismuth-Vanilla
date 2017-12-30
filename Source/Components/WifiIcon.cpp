/*
  ==============================================================================

    WifiIcon.cpp
    Created: 29 Dec 2017 12:02:41pm
    Author:  anthony

  ==============================================================================
 */

#include "WifiIcon.h"
#include "../Wifi/WifiStatus.h"
#include "../Main.h"

WifiIcon::WifiIcon() :
VectorImageButton(ConfigFile::getInstance()->
getComponentSettings(ConfigFile::WIFI), "Wifi") {
    setInterceptsMouseClicks(false, false);
    setWantsKeyboardFocus(false);

    wifiTimer = new WifiTimer(this);
    wifiTimer->startTimer(1);
}

WifiIcon::~WifiIcon() {
    wifiTimer->removeTimer();
}

void WifiIcon::setStatus(WifiIconImage wifiState) {
    ((VectorImageButton*)this)->setImage((int) wifiState);
}

void WifiIcon::visibilityChanged() {
    if (wifiTimer != nullptr) {
        if (isVisible()) {
            if (!wifiTimer->isTimerRunning()) {
                wifiTimer->startTimer(10);
            }
        } else {
            wifiTimer->stopTimer();
        }
    }
}

WifiIcon::WifiTimer::WifiTimer(WifiIcon * wifiIcon) : wifiIcon(wifiIcon) {
}

WifiIcon::WifiTimer::~WifiTimer() {
    removeTimer();
}

void WifiIcon::WifiTimer::removeTimer() {
    wifiIcon = nullptr;
    stopTimer();
}

void WifiIcon::WifiTimer::timerCallback() {
    if (wifiIcon != nullptr) {
        const WifiStatus& wifiStatus = getWifiStatus();
        WifiIconImage wifiState = WIFI_OFF;
        ScopedPointer<WifiAccessPoint> accessPoint =
                wifiStatus.connectedAccessPoint();
        if (wifiStatus.isConnected() && accessPoint != nullptr) {
            // 0 to 100
            float sigStrength = std::max(0,
                    std::min(99, accessPoint->signalStrength));
            wifiState = (WifiIconImage) (2 + (int) (sigStrength * 3 / 100));
        }// wifi on but no connection
        else if (getWifiStatus().isEnabled()) {
            wifiState = WIFI_STRENGTH_0;
        }// wifi off
        wifiIcon->setStatus(wifiState);
    } else {
        stopTimer();
    }
}