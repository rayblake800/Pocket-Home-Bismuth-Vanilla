#pragma once
#include "WindowFocusedTimer.h"
#include "WifiStateManager.h"
#include "ConfigurableImageComponent.h"

/**
 * @file  WifiIcon.h
 * 
 * @brief  Tracks WiFi status and displays an image indicating connection
 *         state.
 */

class WifiIcon : public ConfigurableImageComponent, private WindowFocusedTimer,
private WifiStateManager::Listener
{
public:
    WifiIcon();
    
    virtual ~WifiIcon() { }

private:

    /**
     * @brief  Sets the timer to go off after a very short delay so that the 
     *         icon will update to match the new connection state.
     * 
     * @param state  The new wifi connection state sent.
     */
    virtual void wifiStateChanged(const WifiState state) override;

    /**
     * All wifi state icons.  Each should correspond to an image asset file 
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
     * @brief  Sets the WiFi connection status image.
     * 
     * @param wifiState  The wifi icon that matches the current connection
     *                   state.
     */
    void setStatus(const WifiIconImage wifiState);

    /**
     * @brief  Enables or disables the WiFi checking timer based on component 
     *         visibility.
     */
    void visibilityChanged() override;

    /**
     * @brief  Checks the current WiFi connection state, and updates the WiFi 
     *         icon.
     */
    virtual void timerCallback() override;
    
};
