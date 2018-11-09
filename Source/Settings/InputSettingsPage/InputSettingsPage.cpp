#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "SystemCommands.h"
#include "InputSettingsPage.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "InputSettingsPage";

/* Localized text value keys: */
static const juce::Identifier inputSettingsTextKey = "inputSettings";
static const juce::Identifier calibrateTextKey     = "calibrate";
static const juce::Identifier showCursorTextKey    = "showCursor";
static const juce::Identifier notVisibleTextKey    = "notVisible";
static const juce::Identifier visibleTextKey       = "visible";

InputSettingsPage::InputSettingsPage() :
Locale::TextUser(localeClassKey),
PageComponent("InputSettingsPage"),
title("settings", localeText(inputSettingsTextKey)),
chooseMode("chooseMode"),
calibrating(localeText(calibrateTextKey)),
cursorVisible("cursorVisible", localeText(showCursorTextKey))
{
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

    title.setJustificationType(juce::Justification::centred);
    //ComboBox
    chooseMode.addItem(localeText(notVisibleTextKey), 1);
    chooseMode.addItem(localeText(visibleTextKey), 2);
    chooseMode.addListener(this);
    Config::MainFile mainConfig;
    if (mainConfig.getConfigValue<bool>(Config::MainKeys::showCursorKey))
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
    Config::MainFile mainConfig;
    if (box != &chooseMode) return;
    bool cursorVisible = (box->getSelectedId() == 2);
    mainConfig.setConfigValue<bool>(Config::MainKeys::showCursorKey,
                                    cursorVisible);
}

