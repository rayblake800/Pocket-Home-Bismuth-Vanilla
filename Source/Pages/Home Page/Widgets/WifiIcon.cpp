#include "WifiAccessPoint.h"
#include "Utils.h"
#include "WifiIcon.h"

WifiIcon::WifiIcon(WifiStateManager& wifiManager) :
WindowFocusedTimer("WifiIcon"),
ConfigurableImageComponent(ComponentConfigFile::wifiIconKey),
wifiManager(wifiManager)
{

#    if JUCE_DEBUG
    setName("WifiIcon");
#    endif
    wifiManager.addListener(this);
    startTimer(100);
}

/**
 * When the wifi state changes, set the timer to go off after a very short
 * delay so that the icon will update.
 */
void WifiIcon::wifiStateChanged(WifiStateManager::WifiState state)
{
    startTimer(100);
}

/**
 * Set the WiFi connection status image.
 */
void WifiIcon::setStatus(WifiIconImage wifiState)
{
    setImageAssetIndex((int) wifiState);
}

/**
 * Enable/disable the WiFi checking timer based on component visibility.
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
 * Periodically checks the current WiFi connection state, and
 * updates the WiFi icon.
 */
void WifiIcon::timerCallback()
{
    switch (getWifiState())
    {
            //wifi disabled
        case WifiStateManager::missingNetworkInterface:
        case WifiStateManager::disabled:
        case WifiStateManager::turningOn:
        case WifiStateManager::noStateManager:
            stopTimer();
            setStatus(wifiOff);
            return;

            //wifi disconnected
        case WifiStateManager::enabled:
        case WifiStateManager::turningOff:
        case WifiStateManager::connecting:
            stopTimer();
            setStatus(wifiStrength0);
            return;
    }
    //wifi connected
    WifiAccessPoint accessPoint = wifiManager.getConnectedAP();
    WifiIconImage wifiState = wifiOff;
    if (!accessPoint.isNull())
    {
        // 0 to 100
        float sigStrength =
                median<float>(0, accessPoint.getSignalStrength(), 99);
        wifiState = (WifiIconImage) (2 + (int) (sigStrength * 3 / 100));
    }
    else
    {
        wifiState = wifiStrength0;
    }// wifi off
    setStatus(wifiState);
    startTimer(frequency);
}