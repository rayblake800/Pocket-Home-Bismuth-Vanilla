#pragma once
/**
 * @file  AppMenu_SettingsComponents.h
 *
 * @brief  Provides a set of UI components used to control AppMenu settings.
 */

#include "Locale/TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "Widgets_Counter.h"
#include "AppMenu_ConfigFile.h"
#include "JuceHeader.h"

namespace AppMenu { class SettingsComponents; }

/**
 * @brief  Creates the UI components used to set the AppMenu settings, and 
 *         controls their behavior.
 *
 *  The SettingsComponents object provides UI components that allow the user to 
 * select the AppMenu format, and change the maximum number of applications the 
 * AppMenu shows on screen. It updates these components to reflect the current
 * settings, and updates the settings with changes made through its components.
 *
 *  The SettingsComponents object does not manage the layout of its components,
 * nor does it add them to a parent Component. It provides methods to access 
 * each of its Component objects so that their layout can be managed elsewhere.
 */
class AppMenu::SettingsComponents final : public Locale::TextUser,
    private juce::ComboBox::Listener
{
public:
    /**
     * @brief  Initializes all components to match the current AppMenu settings.
     */
    SettingsComponents();

    virtual ~SettingsComponents() { }

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

    /**
     * @brief  Gets the label used to identify the AppMenu format selection
     *         component.
     *
     * @return  The format selection label.
     */
    juce::Component* getMenuFormatLabel();

    /**
     * @brief  Gets the ComboBox component used to set the AppMenu format.
     *
     * @return   The AppMenu::Format selection ComboBox.
     */
    juce::Component* getMenuFormatPicker();

    /**
     * @brief   Gets the label used to identify the AppMenu column count
     *          component.
     *
     * @return  The column count label.
     */
    juce::Component* getColumnCountLabel();

    /**
     * @brief   Gets the counter used to set the number of application columns
     *          shown by the AppMenu.
     *
     * @return  The AppMenu column counter.
     */
    juce::Component* getColumnCounter();

    /**
     * @brief   Gets the label used to identify the AppMenu row count
     *          component.
     *
     * @return  The row count label.
     */
    juce::Component* getRowCountLabel();

    /**
     * @brief   Gets the counter used to set the number of application rows
     *          shown by the AppMenu.
     *
     * @return  The AppMenu column counter.
     */
    juce::Component* getRowCounter();

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

    /* Saves and loads AppMenu settings, and ensures AppMenu settings are always
     * available while this object exists: */
    ConfigFile formatConfig;

    /* Used to select the AppMenu format: */
    Widgets::BoundedLabel menuFormatLabel;
    juce::ComboBox menuFormatPicker;

    /* Used to select the maximum column count for AppMenu grids: */
    Widgets::BoundedLabel columnCountLabel;
    Widgets::Counter columnCounter;

    /* Used to select the maximum row count for AppMenu grids: */
    Widgets::BoundedLabel rowCountLabel;
    Widgets::Counter rowCounter;
};
