#pragma once
#include "JuceHeader.h"
#include "Locale/TextUser.h"
#include "DrawableImageButton.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

/**
 * @file DateTimePage.h
 * 
 * @brief DateTimePage is a UI page component that allows the user to change
 * the clock display mode and adjust system time.
 */

class DateTimePage : public PageComponent, public Locale::TextUser,
private juce::ComboBox::Listener
{
public:
    DateTimePage();

    virtual ~DateTimePage() { }

private:
    /**
     * Runs reconfigureCommand in the terminal to update system time when
     * the user presses the reconfigure button.
     * 
     * @param button
     */
    void pageButtonClicked(juce::Button* button) override;

    /**
     * Changes the clock mode saved in the ComponentConfigFile when the
     * user selects a new mode with the setClockMode combo box.
     * 
     * @param comboBox
     */
    void comboBoxChanged(juce::ComboBox* comboBox) override;

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
