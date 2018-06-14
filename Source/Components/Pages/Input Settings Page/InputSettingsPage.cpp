#include "MainConfigFile.h"
#include "AppLauncher.h"
#include "InputSettingsPage.h"

InputSettingsPage::InputSettingsPage() :
Localized("InputSettingsPage"),
PageComponent("InputSettingsPage"),
title("settings", localeText(input_settings)),
chooseMode("chooseMode"),
calibrating(localeText(calibrate_screen)),
fnmapping(localeText(remap_keybord)),
cursorVisible("cursorVisible", localeText(select_cursor_visible))
{

#    if JUCE_DEBUG
    setName("InputSettingsPage");
#    endif
    using RowItem = RelativeLayoutManager::ComponentLayout;
    RelativeLayoutManager::Layout layout(
    {
        {
	    .weight = 30, .rowItems = 
            {
                RowItem(&title, 10)
	    }
        },
        {
	    .weight = 20, .rowItems = 
            {
                RowItem(&cursorVisible, 50),
                RowItem(&chooseMode, 20)
	    }
        },
        {   .weight = 40, .rowItems = {} },
        {
	    .weight = 20, .rowItems = 
            {
                RowItem(&calibrating, 10)
	    }
        },
        {
            .weight = 20, .rowItems = 
            {
                RowItem(&fnmapping, 10)
	    }
        }
    });
    layout.setYMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout); 
    
    title.setJustificationType(Justification::centred);
    //ComboBox
    chooseMode.addItem(localeText(not_visible), 1);
    chooseMode.addItem(localeText(visible), 2);
    chooseMode.addListener(this);
    MainConfigFile mainConfig;
    if (mainConfig.getConfigValue<bool>(MainConfigFile::showCursorKey))
    {
        chooseMode.setSelectedId(2);
    }
    else
    {
        chooseMode.setSelectedId(1);
    }
    calibrating.addListener(this);
    fnmapping.addListener(this);
    addAndShowLayoutComponents();
}

/**
 * Re-applies the Xmodmap file or runs Xinput Calibrator, depending on 
 * which button was pressed.
 */
void InputSettingsPage::pageButtonClicked(Button* button)
{
    if (button == &calibrating)
    {
        AppLauncher launcher;
        launcher.startOrFocusApp("XInput Calibrator", calibrationCommand);
    }
    if (button == &fnmapping)
    {
        ScopedPointer<ChildProcess> launchApp = new ChildProcess();
        launchApp->start(keyFixCommand);
        launchApp->waitForProcessToFinish(-1);
    }
}

/**
 * Changes the cursor visibility settings.
 */
void InputSettingsPage::comboBoxChanged(ComboBox* box)
{
    MainConfigFile mainConfig;
    if (box != &chooseMode) return;
    bool cursorVisible = (box->getSelectedId() == 2);
    mainConfig.setConfigValue<bool>(MainConfigFile::showCursorKey,
                                    cursorVisible);
}

