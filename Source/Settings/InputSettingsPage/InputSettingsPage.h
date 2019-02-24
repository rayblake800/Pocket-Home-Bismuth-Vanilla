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
#include "Page_Component.h"

class InputSettingsPage : public Page::Component, public Locale::TextUser
{
public:
    InputSettingsPage();

    virtual ~InputSettingsPage() { }

private:
    class PageListener : public juce::Button::Listener, 
            public juce::ComboBox::Listener
    {
    public:
        PageListener(InputSettingsPage& settingsPage) : 
            settingsPage(settingsPage) { }

    private:
        /**
         * @brief  Runs the display calibration command when the calibration 
         *         button is clicked.
         * 
         * @param button  The calibration button.
         */
        void buttonClicked(juce::Button* button) override;

        /**
         * @brief  Changes the cursor visibility settings.
         * 
         * @param box  The cursor visibility selection component.
         */
        void comboBoxChanged(juce::ComboBox* box) override;

        InputSettingsPage& settingsPage;
    };
    PageListener pageListener;

    //Title of the page
    ScalingLabel title;

    //Set cursor visibility
    ScalingLabel cursorVisible;
    juce::ComboBox chooseMode;

    //Button for calibrating
    juce::TextButton calibrating;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSettingsPage)
};
