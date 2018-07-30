#pragma once
#include "ConfigurableImageButton.h"
#include "Localized.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

/**
 * @file InputSettingsPage.h
 * 
 * The input settings page sets cursor visibility, opens screen calibration,
 * and fixes button mapping.
 */

class InputSettingsPage : public PageComponent,
private juce::ComboBox::Listener,
private Localized
{
public:
    InputSettingsPage();

    virtual ~InputSettingsPage() { }

    /**
     * Re-applies the Xmodmap file or runs Xinput Calibrator, depending on 
     * which button was pressed.
     * 
     * @param button
     */
    void pageButtonClicked(juce::Button* button) override;

    /**
     * Changes the cursor visibility settings.
     * 
     * @param box
     */
    void comboBoxChanged(juce::ComboBox* box) override;

private:
    //Title of the page
    ScalingLabel title;

    //Set cursor visibility
    ScalingLabel cursorVisible;
    juce::ComboBox chooseMode;

    //Button for calibrating
    juce::TextButton calibrating;
    //calibration command
    static const constexpr char * calibrationCommand = "xinput_calibrator";

    //Button for the FN key (xmodmap)
    juce::TextButton fnmapping;
    //Command to fix key mappings
    static const constexpr char * keyFixCommand = "xmodmap ${HOME}/.Xmodmap";

    //localized text keys;
    static const constexpr char * input_settings = "input_settings";
    static const constexpr char * calibrate_screen = "calibrate_screen";
    static const constexpr char * remap_keybord = "remap_keybord";
    static const constexpr char * select_cursor_visible
            = "select_cursor_visible";
    static const constexpr char * not_visible = "not_visible";
    static const constexpr char * visible = "visible";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSettingsPage)
};
