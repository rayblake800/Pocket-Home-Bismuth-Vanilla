#include "DateTimePage.h"

DateTimePage::DateTimePage() :
Localized("DateTimePage"),
PageComponent("DateTimePage"),
titleLabel("dateTimeTitle", localeText(date_time_settings)),
setClockMode("setClockMode"),
reconfigureBtn(localeText(set_system_clock)),
clockModeLabel("modeLabel", localeText(select_clock_mode))
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("DateTimePage");
#    endif
    setBackButton(PageComponent::leftBackButton);
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout(
    {
        Row(30, { RowItem(&titleLabel, 20) }),
        Row(20,{
                RowItem(&clockModeLabel, 30),
                RowItem(&setClockMode, 10)
            }),
        Row(50)
    });
    layout.setYMarginFraction(0.05);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(3);
    setLayout(layout);
    
    reconfigureBtn.addListener(this);
    titleLabel.setJustificationType(Justification::centred);

    setClockMode.addItem(localeText(mode_24h), 1);
    setClockMode.addItem(localeText(mode_am_pm), 2);
    setClockMode.addItem(localeText(hide_clock), 3);
    setClockMode.addListener(this);
    MainConfigFile mainConfig;
    if (mainConfig.getConfigValue<bool>(MainConfigFile::showClockKey))
    {
        if (mainConfig.getConfigValue<bool>
            (MainConfigFile::use24HrModeKey))
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

/**
 * Runs reconfigureCommand in the terminal to update system time when
 * the user presses the reconfigure button.
 */
void DateTimePage::pageButtonClicked(juce::Button* button)
{
    using namespace juce;
    if (button == &reconfigureBtn)
    {
        MainConfigFile mainConfig;
        String configureTime = mainConfig.getConfigValue<String>
                (MainConfigFile::termLaunchCommandKey)
                + reconfigureCommand;
        int ret = system(configureTime.toRawUTF8());
        if (ret != 0)
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon,
                    localeText(failed_launch),
                    localeText(failed_to_run)
                    + "\n" + configureTime + "\n"
                    + localeText(check_term_cmd));
        }
    }
}

/**
 * Changes the clock mode saved in the ComponentConfigFile when the
 * user selects a new mode with the setClockMode combo box.
 */
void DateTimePage::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox != &setClockMode)
    {
        DBG("DateTimePage::" << __func__ << ": responding to ComboBox "
                << comboBox->getName() << ", this should not happen!");
        return;
    }
    bool showClock = (comboBox->getSelectedId() != 3);
    bool use24HrMode = (comboBox->getSelectedId() == 1);
    MainConfigFile mainConfig;
    if (showClock)
    {
        mainConfig.setConfigValue<bool>
                (MainConfigFile::use24HrModeKey, use24HrMode);
    }
    mainConfig.setConfigValue<bool>
            (MainConfigFile::showClockKey, showClock);
}

