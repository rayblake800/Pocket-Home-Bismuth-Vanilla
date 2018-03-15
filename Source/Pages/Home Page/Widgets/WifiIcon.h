/**
 * @File WifiIcon.h
 * 
 * WifiIcon tracks WiFi status and displays an image indicating connection
 * state.
 */

#pragma once
#include "WindowFocusedTimer.h"
#include "ConfigurableImageComponent.h"

class WifiIcon : public ConfigurableImageComponent, private WindowFocusedTimer {
public:
    WifiIcon();
    virtual ~WifiIcon();

private:
    //
    //defined in config.json

    /**
     * All tracked WiFi states.  Each corresponds to an image asset file
     * defined in components.json
     */
    enum WifiIconImage {
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
};