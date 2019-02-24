#pragma once
#include "JuceHeader.h"
#include "Locale/TextUser.h"
#include "DrawableImageButton.h"
#include "ScalingLabel.h"
#include "Page_Component.h"

/**
 * @file DateTimePage.h
 * 
 * @brief DateTimePage is a UI page component that allows the user to change
 * the clock display mode and adjust system time.
 */

class DateTimePage : public Page::Component, public Locale::TextUser
{
public:
    DateTimePage();

    virtual ~DateTimePage() { }

private:
    class PageListener : public juce::Button::Listener,
            public juce::ComboBox::Listener
    {
    public:
        PageListener(DateTimePage& dateTimePage) 
                : dateTimePage(dateTimePage) { }
    private:
        /**
         * Runs reconfigureCommand in the terminal to update system time when
         * the user presses the reconfigure button.
         * 
         * @param button
         */
        void buttonClicked(juce::Button* button) override;

        /**
         * Changes the clock mode saved in the ComponentConfigFile when the
         * user selects a new mode with the setClockMode combo box.
         * 
         * @param comboBox
         */
        void comboBoxChanged(juce::ComboBox* comboBox) override;

        DateTimePage& dateTimePage;
    };
    PageListener pageListener;

    //page title label
    ScalingLabel titleLabel;
    
    //set the clock mode
    ScalingLabel clockModeLabel;
    juce::ComboBox setClockMode;

    //Button for setting system time
    juce::TextButton reconfigureBtn;

    //Command for changing system time
    static const constexpr char * reconfigureCommand
            = " 'sudo dpkg-reconfigure tzdata ; exit'";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DateTimePage)
};
