#pragma once
/**
 * @file  Info_SignalIcon.h
 *
 * @brief  Displays a network signal strength icon.
 */

#include "Theme_Image_Component.h"

namespace Info { class SignalIcon; }

/**
 * @brief  An image Component representing a network signal strength.
 */
class Info::SignalIcon : public Theme::Image::Component<>
{
public:
    /**
     * @brief  Initializes the icon with the image assets defined in the image
     *         configuration file.
     */
    SignalIcon();

    virtual ~SignalIcon() { }

    /**
     * @brief  Updates the signal icon to represent a new signal strength.
     *
     * @param signalStrength  The signal strength, as a number between zero and
     *                        100, inclusive.
     */
    void setSignalStrengthImage(const unsigned int signalStrength);

    /**
     * @brief  Updates the signal icon to show that the signal is disabled.
     */
    void setDisabledImage();
};
