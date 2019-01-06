#pragma once
/**
 * @file  InputSettingsPage.h
 * 
 * @brief   Sets cursor visibility and opens screen calibration.
 */

#include "Theme_Image_Component.h"
#include "DrawableImageButton.h"
#include "Locale/TextUser.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

class InputSettingsPage : public PageComponent, public Locale::TextUser,
private juce::ComboBox::Listener
{
public:
    InputSettingsPage();

    virtual ~InputSettingsPage() { }

    /**
     * @brief  Runs the display calibration command when the calibration button
     *         is clicked.
     * 
     * @param button  The calibration button.
     */
    void pageButtonClicked(juce::Button* button) override;

    /**
     * @brief  Changes the cursor visibility settings.
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSettingsPage)
};
