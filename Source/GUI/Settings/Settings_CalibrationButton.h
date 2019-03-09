#pragma once
/**
 * @file  Settings_CalibrationButton.h
 *
 * @brief  Allows the user to calibrate touchscreen input.
 */

#include "Locale_TextUser.h"
#include "JuceHeader.h"

namespace Settings { class CalibrationButton; }

/**
 * @brief  Launches a touchscreen calibration application when clicked.
 */
class Settings::CalibrationButton : public juce::TextButton, 
        public Locale::TextUser, private juce::Button::Listener
{
public:
    /**
     * @brief  Loads localized button text and starts listening for click
     *         events on construction.
     */
    CalibrationButton();

    virtual ~CalibrationButton() { }

private:
    /**
     * @brief  Launches the calibration application when the button is clicked.
     *
     * @param button  This button component.
     */
    virtual void buttonClicked(juce::Button* button) override;
};
