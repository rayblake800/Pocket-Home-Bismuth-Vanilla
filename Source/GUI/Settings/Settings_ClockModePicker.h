#pragma once
/**
 * @file  Settings_ClockModePicker.h
 *
 * @brief  Provides a combo box for selecting the clock mode.
 */

#include "Locale_TextUser.h"
#include "JuceHeader.h"

namespace Settings { class ClockModePicker; }

/**
 * @brief  Allows the user to choose between hiding the clock, showing twelve
 *         hour time, and showing twenty four hour time.
 */
class Settings::ClockModePicker : public juce::ComboBox,
        public Locale::TextUser, private juce::ComboBox::Listener
{
public:
    /**
     * @brief  Initializes the component with the current saved clock mode.
     */
    ClockModePicker();

    virtual ~ClockModePicker() { }

private:
    /**
     * @brief  Updates the clock mode when the user selects a new mode.
     *
     * @param comboBox  This combo box.
     */
    virtual void comboBoxChanged(juce::ComboBox* comboBox);
};

