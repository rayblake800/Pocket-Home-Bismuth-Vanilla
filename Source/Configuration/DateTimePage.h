#ifndef DATETIME_H
#define DATETIME_H

#include "../../JuceLibraryCode/JuceHeader.h"

class DateTimePage : public Component, public Button::Listener,
private ComboBox::Listener {
public:
    DateTimePage();
    ~DateTimePage();
    void buttonClicked(Button*) override;
    void paint(Graphics&) override;
    void resized() override;
    void comboBoxChanged(ComboBox*) override;

private:
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
    
    ScopedPointer<ImageButton> backButton;
    Label titleLabel;
    Label clockModeLabel;
    ComboBox setClockMode;

    //Button for setting system time
    TextButton reconfigureBtn;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DateTimePage)
};

#endif