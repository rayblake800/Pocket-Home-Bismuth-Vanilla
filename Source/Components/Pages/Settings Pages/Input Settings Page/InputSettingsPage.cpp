#include "MainConfigFile.h"
#include "MainConfigKeys.h"
#include "SystemCommands.h"
#include "InputSettingsPage.h"

InputSettingsPage::InputSettingsPage() :
Localized("InputSettingsPage"),
PageComponent("InputSettingsPage"),
title("settings", localeText(input_settings)),
chooseMode("chooseMode"),
calibrating(localeText(calibrate_screen)),
cursorVisible("cursorVisible", localeText(select_cursor_visible))
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("InputSettingsPage");
#    endif
    setBackButton(PageComponent::leftBackButton);
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(30,
        {
            RowItem(&title, 10)
        }),
        Row(20,
        {
            RowItem(&cursorVisible, 50),
            RowItem(&chooseMode, 20)
        }),
        Row(20),
        Row(20,
        {
            RowItem(&calibrating, 10)
        }),
        Row(20)
    });
    layout.setYMarginFraction(0.03);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(3);
    setLayout(layout);

    title.setJustificationType(Justification::centred);
    //ComboBox
    chooseMode.addItem(localeText(not_visible), 1);
    chooseMode.addItem(localeText(visible), 2);
    chooseMode.addListener(this);
    MainConfigFile mainConfig;
    if (mainConfig.getConfigValue<bool>(MainConfigKeys::showCursorKey))
    {
        chooseMode.setSelectedId(2);
    }
    else
    {
        chooseMode.setSelectedId(1);
    }
    calibrating.addListener(this);
    addAndShowLayoutComponents();
}

/*
 * Re-applies the Xmodmap file or runs Xinput Calibrator, depending on 
 * which button was pressed.
 */
void InputSettingsPage::pageButtonClicked(juce::Button* button)
{
    using namespace juce;
    if (button == &calibrating)
    {
        SystemCommands systemCommands;
        systemCommands.runActionCommand
            (SystemCommands::ActionCommand::calibrate);
    }
}

/*
 * Changes the cursor visibility settings.
 */
void InputSettingsPage::comboBoxChanged(juce::ComboBox* box)
{
    MainConfigFile mainConfig;
    if (box != &chooseMode) return;
    bool cursorVisible = (box->getSelectedId() == 2);
    mainConfig.setConfigValue<bool>(MainConfigKeys::showCursorKey,
                                    cursorVisible);
}

