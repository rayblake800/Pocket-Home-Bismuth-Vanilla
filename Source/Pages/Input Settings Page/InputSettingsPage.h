/**
 * @file InputSettingsPage.h
 * 
 * The input settings page sets cursor visibility, opens screen calibration,
 * and fixes button mapping.
 */
#pragma once
#include "ConfigurableImageButton.h"
#include "Localized.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

class InputSettingsPage : public PageComponent, private ComboBox::Listener,
private Localized
{
public:
    /**
     * @param pageFactory      This will be needed to launch the key binding 
     *                          page, once it is implemented.
     */
    InputSettingsPage(PageFactoryInterface* pageFactory);

    virtual ~InputSettingsPage() { }

    /**
     * Re-applies the Xmodmap file or runs Xinput Calibrator, depending on 
     * which button was pressed.
     * 
     * @param button
     */
    void pageButtonClicked(Button* button) override;

    /**
     * Changes the cursor visibility settings.
     * 
     * @param box
     */
    void comboBoxChanged(ComboBox* box) override;

private:
    //Title of the page
    ScalingLabel title;

    //Set cursor visibility
    ScalingLabel cursorVisible;
    ComboBox chooseMode;

    //Button for calibrating
    TextButton calibrating;
    //calibration command
    static const constexpr char * calibrationCommand = "xinput_calibrator";

    //Button for the FN key (xmodmap)
    TextButton fnmapping;
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
