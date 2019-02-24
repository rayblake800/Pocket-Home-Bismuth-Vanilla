#pragma once
/**
 * @file  Wifi_SignalIcon.h
 *
 * @brief  Displays the signal strength of a Wifi access point or connection.
 */

#include "Theme_Image_Component.h"

namespace Wifi { class SignalIcon; }

/**
 * @brief  An image Component representing the signal strength of a Wifi access
 *         point or connection.
 */
class Wifi::SignalIcon : public Theme::Image::Component<>
{
public:
    /**
     * @brief  Initializes the icon with the image assets defined in the image
     *         configuration file.
     */
    SignalIcon();

    virtual ~SignalIcon() { }

    /**
     * @brief  Updates the Wifi signal icon to represent a new signal strength.
     *
     * @param signalStrength  The signal strength, as a number between zero and
     *                        100, inclusive.
     */
    void setSignalStrengthImage(const unsigned int signalStrength);

    /**
     * @brief  Updates the Wifi signal icon to show that Wifi is disabled.
     */
    void setWifiDisabledImage();

private:
    /**
     * All wifi state icons.  Each should correspond to an image asset file 
     * defined in components.json
     */
    enum class APIcon
    {
        wifiOff,
        wifiStrength0,
        wifiStrength1,
        wifiStrength2,
        wifiStrength3
    };
};
