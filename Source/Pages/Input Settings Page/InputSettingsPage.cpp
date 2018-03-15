#include "MainConfigFile.h"
#include "AppLauncher.h"
#include "InputSettingsPage.h"

InputSettingsPage::InputSettingsPage() :
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
    {2,
        {
            {&calibrating, 1}
        }},
    {2,
        {
            {&fnmapping, 1}
        }}
}, true),
title("settings", "Input settings"),
chooseMode("chooseMode"),
calibrating("Calibrate the screen"),
fnmapping("Remap keyboard (FN key fix)"),
cursorVisible("cursorVisible", "Select the visibility of the cursor:")
{
    
#if JUCE_DEBUG
    setName("InputSettingsPage");
#endif
    title.setJustificationType(Justification::centred);
    //ComboBox
    chooseMode.addItem("Not visible", 1);
    chooseMode.addItem("Visible", 2);
    chooseMode.addListener(this);
    MainConfigFile config;
    if (config.getConfigValue<bool>(MainConfigFile::showCursorKey))
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

InputSettingsPage::~InputSettingsPage() { }

void InputSettingsPage::pageButtonClicked(Button* button)
{
    if (button == &calibrating)
    {
        AppLauncher launcher;
        launcher.startOrFocusApp("XInput Calibrator", "xinput_calibrator");
    }
}

void InputSettingsPage::comboBoxChanged(ComboBox* c)
{
    if (c != &chooseMode) return;
    MainConfigFile config;
    bool cursorVisible = (c->getSelectedId() == 2);
    config.setConfigValue<bool>(MainConfigFile::showCursorKey, cursorVisible);
}

