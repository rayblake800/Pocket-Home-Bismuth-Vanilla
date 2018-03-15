#include "PocketHomeApplication.h"
#include "WifiAccessPoint.h"
#include "Utils.h"
#include "WifiIcon.h"

WifiIcon::WifiIcon() :
WindowFocusedTimer("WifiIcon"),
ConfigurableImageComponent(ComponentConfigFile::wifiIconKey)
{

#    if JUCE_DEBUG
    setName("WifiIcon");
#    endif
    startTimer(1);
}

WifiIcon::~WifiIcon()
{
    stopTimer();
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
    if (isVisible())
    {
        if (!isTimerRunning())
        {
            startTimer(10);
        }
    }
    else
    {
        stopTimer();
    }
}

/**
 * Check WiFi state and update the image.
 */
void WifiIcon::timerCallback()
{
    WifiStatus* wifiStatus =  PocketHomeApplication::getInstance()
            ->getWifiStatus();
    WifiIconImage wifiState = wifiOff;
    if (wifiStatus != nullptr)
    {
        if (wifiStatus->isConnected())
        {
            WifiAccessPoint accessPoint = wifiStatus->getConnectedAccessPoint();
            if (!accessPoint.isNull())
            {
                // 0 to 100
                float sigStrength =
                        median<float>(0, accessPoint.getSignalStrength(), 99);
                wifiState = (WifiIconImage) (2 + (int) (sigStrength * 3 / 100));
            }
        }// wifi on but no connection
        else if (wifiStatus->isEnabled())
        {
            wifiState = wifiStrength0;
        }// wifi off
    }
    setStatus(wifiState);
    startTimer(frequency);
}