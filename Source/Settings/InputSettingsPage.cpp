#include "../PocketHomeApplication.h"
#include "InputSettingsPage.h"
#include "../AppMenu Page/AppLauncher.h"

InputSettingsPage::InputSettingsPage() :
backButton(ComponentConfigFile::pageLeftKey),
bg_color(0xffd23c6d),
title("settings", "Input settings"),
choosemode("choosemode"),
calibrating("Calibrate the screen"),
fnmapping("Remap keyboard (FN key fix)"),
cursorVisible("cursorvisible", "Select the visibility of the cursor:")
{
    std::vector<GridLayoutManager::ComponentLayoutParams> layoutParams = {
        {&title,0,1},
        {&cursorVisible,2,5},
        {&choosemode,2,2},
        {&calibrating,3,1},
        {&fnmapping,4,1}
    };
    title.setJustificationType(Justification::centred);
    layoutManager.addComponents(layoutParams,this);
    //Back button
    backButton.addListener(this);
    //ComboBox
    choosemode.addItem("Not visible", 1);
    choosemode.addItem("Visible", 2);
    choosemode.addListener(this);
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    if (config.getConfigBool(MainConfigFile::showCursorKey))
    {
        choosemode.setSelectedId(2);
    } else
    {
        choosemode.setSelectedId(1);
    }
    calibrating.addListener(this);
    fnmapping.addListener(this);
}

InputSettingsPage::~InputSettingsPage()
{
}

void InputSettingsPage::buttonClicked(Button* but)
{
    if (but == &backButton)
        PocketHomeApplication::getInstance()->getMainStack().popPage
            (PageStackComponent::kTransitionTranslateHorizontal);
    else if (but == &calibrating)
    {
        AppLauncher launcher;
        launcher.startOrFocusApp("XInput Calibrator","xinput_calibrator");
    }
}

void InputSettingsPage::comboBoxChanged(ComboBox* c)
{
    if (c != &choosemode) return;
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    bool cursorVisible = (c->getSelectedId() == 2);
    config.setConfigBool(MainConfigFile::showCursorKey, cursorVisible);

    LookAndFeel& laf = getLookAndFeel();
    PokeLookAndFeel* mc = (PokeLookAndFeel*) & laf;
    mc->setCursorVisible(cursorVisible);
}

void InputSettingsPage::paint(Graphics& g)
{
    auto bounds = getLocalBounds();
    g.fillAll(bg_color);
}

void InputSettingsPage::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    backButton.applyConfigBounds();
    bounds.reduce(backButton.getWidth(),bounds.getHeight()/15);
    layoutManager.layoutComponents(bounds,bounds.getWidth()/20,bounds.getHeight()/14);
}
