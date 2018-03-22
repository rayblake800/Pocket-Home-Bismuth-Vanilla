#include "DateTimePage.h"

const Colour DateTimePage::bgColour = Colour(0xffd23c6d);
const String DateTimePage::pageTitle = "Date and time settings";

const String DateTimePage::clockModeLabelText
        = "Select the display mode for the clock:";
const String DateTimePage::clockMode24h = "24h mode";
const String DateTimePage::clockModeAmPm = "AM/PM mode";
const String DateTimePage::clockModeNoShow = "Don't show clock";

const String DateTimePage::reconfigureBtnText = "Reconfigure system clock";
const String DateTimePage::reconfigureCommand
        = " 'sudo dpkg-reconfigure tzdata ; exit'";
const String DateTimePage::reconfErrorTitle = "Failed to launch terminal:";
const String DateTimePage::reconfErrorPreCmd = "Running '";
const String DateTimePage::reconfErrorPostCmd
        = "' failed.\nIs the terminal launch command set correctly?";

DateTimePage::DateTimePage(PageComponent::PageFactoryInterface& pageFactory,
        MainConfigFile& mainConfig, ComponentConfigFile& componentConfig) :
PageComponent(pageFactory, "DateTimePage",{
    {3,
        {
            {&titleLabel, 1}
        }},
    {2,
        {
            {&clockModeLabel, 3},
            {&setClockMode, 1}
        }},
    {3,
        {
            {nullptr, 1}
        }},
    {2,
        {
            {&reconfigureBtn, 1}
        }},
    {1,
        {
            {nullptr, 1}
        }}
}, true),
mainConfig(mainConfig),
componentConfig(componentConfig),
titleLabel("dateTimeTitleLabel", pageTitle),
setClockMode("setClockMode"),
reconfigureBtn(reconfigureBtnText),
clockModeLabel("clockModeLabel", clockModeLabelText)
{

#    if JUCE_DEBUG
    setName("DateTimePage");
#    endif
    addAndShowLayoutComponents();
    reconfigureBtn.addListener(this);
    titleLabel.setJustificationType(Justification::centred);

    setClockMode.addItem(clockMode24h, 1);
    setClockMode.addItem(clockModeAmPm, 2);
    setClockMode.addItem(clockModeNoShow, 3);
    setClockMode.addListener(this);
    if (componentConfig.getConfigValue<bool>(ComponentConfigFile::showClockKey))
    {
        if (componentConfig.getConfigValue<bool>
            (ComponentConfigFile::use24HrModeKey))
        {
            setClockMode.setSelectedId(1,
                    NotificationType::dontSendNotification);
        }
        else
        {
            setClockMode.setSelectedId(2,
                    NotificationType::dontSendNotification);
        }
    }
    else
    {
        setClockMode.setSelectedId(3, NotificationType::dontSendNotification);
    }
}

void
DateTimePage::pageButtonClicked(Button* button)
{
    if (button == &reconfigureBtn)
    {
        String configureTime = mainConfig.getConfigValue<String>
                (MainConfigFile::termLaunchCommandKey)
                + reconfigureCommand;
        int ret = system(configureTime.toRawUTF8());
        if (ret != 0)
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon,
                    reconfErrorTitle,
                    reconfErrorPreCmd + configureTime + reconfErrorPostCmd);
        }
    }
}

void DateTimePage::comboBoxChanged(ComboBox* c)
{
    if (c != &setClockMode) return;
    bool showClock = (c->getSelectedId() != 3);
    bool use24HrMode = (c->getSelectedId() == 1);
    if (showClock)
    {
        componentConfig.setConfigValue<bool>
                (ComponentConfigFile::use24HrModeKey, use24HrMode);
    }
    componentConfig.setConfigValue<bool>
            (ComponentConfigFile::showClockKey, showClock);
}

