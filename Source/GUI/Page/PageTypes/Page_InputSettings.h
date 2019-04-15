#pragma once
/**
 * @file  Page_InputSettings.h
 *
 * @brief  Sets cursor visibility and opens screen calibration.
 */

#include "Page_Component.h"
#include "Locale_TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "Settings_CursorModePicker.h"
#include "Settings_CalibrationButton.h"

namespace Page { class InputSettings; }

/**
 * @brief  A Page class that provides control components to the user for
 *         changing application input settings.
 */
class Page::InputSettings : public Page::Component, public Locale::TextUser
{
public:
    /**
     * @brief  Loads the page layout on construction.
     */
    InputSettings();

    virtual ~InputSettings() { }

private:
    // Page title label:
    Widgets::BoundedLabel titleLabel;

    // Sets cursor visibility:
    Widgets::BoundedLabel cursorModeLabel;
    Settings::CursorModePicker cursorModePicker;

    // Launches calibration application:
    Settings::CalibrationButton calibrateButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSettings)
};
