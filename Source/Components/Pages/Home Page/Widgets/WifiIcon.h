#pragma once
#include "WindowFocusedTimer.h"
#include "WifiStateManager.h"
#include "ConfigurableImageComponent.h"

/**
 * @File WifiIcon.h
 * 
 * WifiIcon tracks WiFi status and displays an image indicating connection
 * state.
 */

class WifiIcon : public ConfigurableImageComponent, private WindowFocusedTimer,
private WifiStateManager::Listener
{
public:
    WifiIcon();
    
    virtual ~WifiIcon() { }

private:

    /**
     * When the wifi state changes, set the timer to go off after a very short
     * delay so that the icon will update.
     * 
     * @param state   New wifi state sent by the wifi manager.
     */
    virtual void wifiStateChanged(WifiState state) override;

    /**
     * All wifi icon states.  Each corresponds to an image asset file
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
     * 
     * @param wifiState   The wifi icon that matches the current connection
     *                     state.
     */
    void setStatus(WifiIconImage wifiState);

    /**
     * Enable/disable the WiFi checking timer based on component visibility.
     */
    void visibilityChanged() override;

    /**
     * Periodically checks the current WiFi connection state, and
     * updates the WiFi icon.
     */
    void timerCallback();
    
    //timer frequency in ms
    const static int frequency = 2000;
};
