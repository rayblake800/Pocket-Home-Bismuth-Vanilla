#pragma once
#include "JuceHeader.h"
#include "Localized.h"
#include "DrawableImageButton.h"
#include "ScalingLabel.h"
#include "MainConfigFile.h"
#include "ComponentConfigFile.h"
#include "PageComponent.h"

/**
 * @file DateTimePage.h
 * 
 * @brief DateTimePage is a UI page component that allows the user to change
 * the clock display mode and adjust system time.
 */

class DateTimePage : public PageComponent, private juce::ComboBox::Listener,
private Localized
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

    //localized text keys;
    static const constexpr char * date_time_settings = "date_time_settings";
    static const constexpr char * select_clock_mode = "select_clock_mode";
    static const constexpr char * mode_24h = "mode_24h";
    static const constexpr char * mode_am_pm = "mode_am_pm";
    static const constexpr char * hide_clock = "hide_clock";
    static const constexpr char * set_system_clock = "set_system_clock";
    static const constexpr char * failed_launch = "failed_launch";
    static const constexpr char * failed_to_run = "failed_to_run";
    static const constexpr char * check_term_cmd = "check_term_cmd";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DateTimePage)
};