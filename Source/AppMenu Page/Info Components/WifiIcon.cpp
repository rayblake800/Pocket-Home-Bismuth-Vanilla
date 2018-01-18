/*
  ==============================================================================

    WifiIcon.cpp
    Created: 29 Dec 2017 12:02:41pm
    Author:  Anthony Brown

  ==============================================================================
 */

#include "WifiIcon.h"
#include "../../Wifi/WifiStatus.h"
#include "../../PocketHomeApplication.h"

WifiIcon::WifiIcon() :
ConfigurableImageComponent(ComponentConfigFile::wifiIconKey)
{

    wifiTimer = new WifiTimer(this);
    wifiTimer->startTimer(1);
}

WifiIcon::~WifiIcon()
{
    wifiTimer->removeTimer();
}

/**
 * Set the WiFi connection status image.
 */
void WifiIcon::setStatus(WifiIconImage wifiState)
{
    setImageAssetIndex((int) wifiState);
}

/**
 * Enable/disable the WiFi checking timer based on component visibility
 */
void WifiIcon::visibilityChanged()
{
    if (wifiTimer != nullptr)
    {
        if (isVisible())
        {
            if (!wifiTimer->isTimerRunning())
            {
                wifiTimer->startTimer(10);
            }
        } else
        {
            wifiTimer->stopTimer();
        }
    }
}

WifiIcon::WifiTimer::WifiTimer(WifiIcon * wifiIcon) : wifiIcon(wifiIcon)
{
}

WifiIcon::WifiTimer::~WifiTimer()
{
    removeTimer();
}

void WifiIcon::WifiTimer::removeTimer()
{
    wifiIcon = nullptr;
    stopTimer();
}

/**
 * Check WiFi state and update the image.
 */
void WifiIcon::WifiTimer::timerCallback()
{
    if (wifiIcon != nullptr)
    {
        const WifiStatus& wifiStatus = PocketHomeApplication::getInstance()
                ->getWifiStatus();
        WifiIconImage wifiState = WIFI_OFF;
        ScopedPointer<WifiAccessPoint> accessPoint =
                wifiStatus.connectedAccessPoint();
        if (wifiStatus.isConnected() && accessPoint != nullptr)
        {
            // 0 to 100
            float sigStrength = std::max(0,
                    std::min(99, accessPoint->signalStrength));
            wifiState = (WifiIconImage) (2 + (int) (sigStrength * 3 / 100));
        }// wifi on but no connection
        else if (wifiStatus.isEnabled())
        {
            wifiState = WIFI_STRENGTH_0;
        }// wifi off
        wifiIcon->setStatus(wifiState);
        startTimer(frequency);
    } 
    else
    {
        stopTimer();
    }
}