#pragma once
/**
 * @file  Settings_CursorModePicker.h
 *
 * @brief  Provides a combo box for changing mouse cursor visibility.
 */

#include "Locale_TextUser.h"
#include "JuceHeader.h"

namespace Settings { class CursorModePicker; }

/**
 * @brief  Allows the user to choose between showing or hiding the mouse cursor.
 */
class Settings::CursorModePicker : public juce::ComboBox, 
        public Locale::TextUser, private juce::ComboBox::Listener
{
public:
    /**
     * @brief  Initializes the component with the current cursor visibility
     *         setting.
     */
    CursorModePicker();

    virtual ~CursorModePicker() { }

private:
    /**
     * @brief  Updates the cursor visibility mode when the user selects a new 
     *         mode.
     *
     * @param comboBox  This combo box.
     */
    virtual void comboBoxChanged(juce::ComboBox* comboBox);
};

