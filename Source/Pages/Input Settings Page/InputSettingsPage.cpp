#include "MainConfigFile.h"
#include "AppLauncher.h"
#include "InputSettingsPage.h"

InputSettingsPage::InputSettingsPage(PageFactoryInterface* pageFactory) :
Localized("InputSettingsPage"),
PageComponent("InputSettingsPage",{
    {3,
        {
            {&title, 1}
        }},
    {2,
        {
            {&cursorVisible, 5},
            {&chooseMode, 2}
        }},
    {4,
        {
            {nullptr, 1}
        }},
    {2,
        {
            {&calibrating, 1}
        }},
    {2,
        {
            {&fnmapping, 1}
        }}
}, pageFactory),
title("settings", localeText(input_settings)),
chooseMode("chooseMode"),
calibrating(localeText(calibrate_screen)),
fnmapping(localeText(remap_keybord)),
cursorVisible("cursorVisible", localeText(select_cursor_visible))
{

#    if JUCE_DEBUG
    setName("InputSettingsPage");
#    endif
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

