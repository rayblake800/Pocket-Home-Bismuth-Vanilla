#pragma once
/**
 * @file  Settings_CursorModePicker.h
 *
 * @brief  Provides a switch for changing mouse cursor visibility.
 */

#include "Locale_TextUser.h"
#include "Widgets_Switch.h"

namespace Settings { class CursorModePicker; }

/**
 * @brief  Allows the user to choose between showing or hiding the mouse cursor.
 */
class Settings::CursorModePicker : public Widgets::Switch,
    private juce::Button::Listener
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
     * @param cursorSwitc  This switch.
     */
    void buttonClicked(juce::Button* cursorSwitch);
};

