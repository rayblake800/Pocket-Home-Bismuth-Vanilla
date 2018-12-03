#include "Utils.h"
#include "WifiIcon.h"
#include "ComponentConfigKeys.h"

/* Timer frequency in milliseconds. */
static const constexpr int frequency = 2000;

WifiIcon::WifiIcon() :
WindowFocusedTimer("WifiIcon"),
ConfigurableImageComponent(ComponentConfigKeys::wifiIconKey)
{
#    if JUCE_DEBUG
    setName("WifiIcon");
#    endif
    startTimer(100);
}

/*
 * Sets the timer to go off after a very short delay so that the icon will 
 * update to match the new connection state.
 */
void WifiIcon::wifiStateChanged(const WifiState state)
{
    startTimer(100);
}

/*
 * Sets the WiFi connection status image.
 */
void WifiIcon::setStatus(const WifiIconImage wifiState)
{
    setImageAssetIndex((int) wifiState);
}

/*
 * Enables or disables the WiFi checking timer based on component visibility.
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

/*
 * Checks the current WiFi connection state, and updates the WiFi icon.
 */
void WifiIcon::timerCallback()
{
    WifiStateManager wifiManager;
    switch (wifiManager.getWifiState())
    {
            //wifi disabled
        case WifiState::missingNetworkDevice:
        case WifiState::disabled:
        case WifiState::turningOn:
            stopTimer();
            setStatus(wifiOff);
            return;

            //wifi disconnected
        case WifiState::enabled:
        case WifiState::turningOff:
        case WifiState::connecting:
            stopTimer();
            setStatus(wifiStrength0);
            return;
    }
    //wifi connected
    WifiAccessPoint accessPoint = wifiManager.getActiveAP();
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
        // wifi off
        wifiState = wifiStrength0;
    }    
    setStatus(wifiState);
    startTimer(frequency);
}
