/**
 * @file DateTimePage.h
 * 
 * DateTimePage is a UI page component that allows the user to change
 * the clock display mode and adjust system time.
 * 
 * TODO:documentation
 */
#pragma once
#include "JuceHeader.h"
#include "DrawableImageButton.h"
#include "ScalingLabel.h"
#include "MainConfigFile.h"
#include "ComponentConfigFile.h"
#include "PageComponent.h"

class DateTimePage : public PageComponent, private ComboBox::Listener
{
public:
    DateTimePage(PageComponent::PageFactoryInterface& pageFactory,
            MainConfigFile& mainConfig, ComponentConfigFile& componentConfig);

    ~DateTimePage() { }

private:
    /**
     * 
     * @param button
     */
    void pageButtonClicked(Button* button) override;

    /**
     * 
     * @param comboBox
     */
    void comboBoxChanged(ComboBox* comboBox) override;

    static const Colour bgColour;
    static const String pageTitle;

    static const String clockModeLabelText;
    static const String clockMode24h;
    static const String clockModeAmPm;
    static const String clockModeNoShow;

    static const String reconfigureBtnText;
    static const String reconfigureCommand;
    static const String reconfErrorTitle;
    static const String reconfErrorPreCmd;
    static const String reconfErrorPostCmd;

    MainConfigFile& mainConfig;
    ComponentConfigFile& componentConfig;

    ScalingLabel titleLabel;
    ScalingLabel clockModeLabel;
    ComboBox setClockMode;

    //Button for setting system time
    TextButton reconfigureBtn;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DateTimePage)
};