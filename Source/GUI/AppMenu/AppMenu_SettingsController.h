#pragma once
/**
 * @file  AppMenu_SettingsController.h
 *
 * @brief  Manages a set of UI components used to control AppMenu settings.
 */

#include "Locale_TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "Widgets_Counter.h"
#include "AppMenu_ConfigFile.h"
#include "JuceHeader.h"

namespace AppMenu { class SettingsController; }

/**
 * @brief  Configures a set of UI components so they can be used to change
 *         AppMenu behavior.
 *
 *  The SettingsController object controls UI components that allow the user to
 * select the AppMenu format, and change the maximum number of applications the
 * AppMenu shows on screen. It updates these components to reflect the current
 * settings, and updates the settings with changes made through its components.
 */
class AppMenu::SettingsController final : public Locale::TextUser,
    private juce::ComboBox::Listener
{
public:
    /**
     * @brief  Connects the controller to the components it will control.
     *
     * @param menuFormatLabel   The label used to identify the menu format
     *                          selection box.
     *
     * @param menuFormatPicker  The component used to select the active AppMenu
     *                          format.
     *
     * @param columnCountLabel  The label used to identify the column count
     *                          editor.
     *
     * @param columnCounter     The component used to select the maximum number
     *                          of menu columns to display at once.
     *
     * @param rowCountLabel     The label used to identify the row count editor.
     *
     * @param rowCounter        The component used to select the maximum number
     *                          of menu rows to display at once.
     */
    SettingsController(Widgets::BoundedLabel& menuFormatLabel,
            juce::ComboBox& menuFormatPicker,
            Widgets::BoundedLabel& columnCountLabel,
            Widgets::Counter& columnCounter,
            Widgets::BoundedLabel& rowCountLabel,
            Widgets::Counter& rowCounter);

    virtual ~SettingsController() { }

    /**
     * @brief  Updates all settings components to match the current AppMenu
     *         settings.
     */
    void updateForCurrentSettings();

    /**
     * @brief  Saves the settings selected by the SettingsComponents so they
     *         will be applied to the AppMenu.
     */
    void applySettingsChanges();

private:
    /**
     * @brief  Updates counter components when the format selection box changes.
     *
     * @param box  The AppMenu::Format selection ComboBox.
     */
    void comboBoxChanged(juce::ComboBox* box) final override;

    /**
     * @brief  Updates the column and row counters for the current format shown
     *         by the menuFormatPicker.
     *
     *  This sets the counters to whatever value is currently saved for the
     * selected format, and shows or hides the column counter and its label
     * depending on whether the selected format uses it.
     */
    void updateCountersForSelectedFormat();

    // Tracks whether the controller has already loaded initial menu text and
    // set itself as the selection box listener:
    bool initialized = false;

    // Saves and loads AppMenu settings, and ensures AppMenu settings are
    // always available while this object exists:
    ConfigFile formatConfig;

    // Used to select the AppMenu format:
    Widgets::BoundedLabel& menuFormatLabel;
    juce::ComboBox& menuFormatPicker;

    // Used to select the maximum column count for AppMenu grids:
    Widgets::BoundedLabel& columnCountLabel;
    Widgets::Counter& columnCounter;

    // Used to select the maximum row count for AppMenu grids:
    Widgets::BoundedLabel& rowCountLabel;
    Widgets::Counter& rowCounter;
};
