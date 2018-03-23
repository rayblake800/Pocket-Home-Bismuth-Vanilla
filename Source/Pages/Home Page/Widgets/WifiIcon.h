/**
 * @File WifiIcon.h
 * 
 * WifiIcon tracks WiFi status and displays an image indicating connection
 * state.
 */

#pragma once
#include "WindowFocusedTimer.h"
#include "WifiStateManager.h"
#include "ConfigurableImageComponent.h"

class WifiIcon : public ConfigurableImageComponent, private WindowFocusedTimer,
private WifiStateManager::Listener
{
public:
    WifiIcon(WifiStateManager& wifiManager,ComponentConfigFile& config);
    virtual ~WifiIcon();

private:

    /**
     * When the wifi state changes, set the timer to go off after a very short
     * delay so that the icon state will update.
     */
    void wifiStateChanged(WifiStateManager::WifiState state) override;

    /**
     * All tracked WiFi states.  Each corresponds to an image asset file
     * defined in components.json
     */
    enum WifiIconImage
    {
        wifiOff,
        wifiStrength0,
        wifiStrength1,
        wifiStrength2,
        wifiStrength3
    };

    /**
     * Set the WiFi connection status image.
     * @param wifiState the last discovered state of the WiFi connection.
     */
    void setStatus(WifiIconImage wifiState);

    /**
     * Enable/disable the WiFi checking timer based on component visibility
     */
    void visibilityChanged() override;

    /**
     * periodically checks the current WiFi connection state, and
     * updates the WiFi icon.
     */
    void timerCallback();
    //timer frequency in ms
    const static int frequency = 2000;
    
    WifiStateManager& wifiManager;
};