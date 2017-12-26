/*
  ==============================================================================

    AppMenuPage.cpp
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */

#include "AppMenuPage.h"
#include "Main.h"

AppMenuPage::AppMenuPage(LauncherComponent * launcherComponent) {
    Rectangle<int> appMenuBorder = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    appMenu = new AppMenuComponent(getConfigJSON(), appMenuBorder);
    addAndMakeVisible(appMenu);
    setWantsKeyboardFocus(true);
}

AppMenuPage::~AppMenuPage() {
}

void AppMenuPage::hideLaunchSpinner() {
    appMenu->hideLaunchSpinner();
}

bool AppMenuPage::keyPressed(const KeyPress& key) {
    //don't interrupt animation
    if (Desktop::getInstance().getAnimator().isAnimating(appMenu))return false;
    int keyCode = key.getKeyCode();
    if (keyCode == KeyPress::upKey || keyCode == KeyPress::downKey) {
        if (keyCode == KeyPress::upKey)appMenu->selectPrevious();
        else appMenu->selectNext();
        return true;
    } else if (keyCode == KeyPress::leftKey || keyCode == KeyPress::escapeKey) {
        if (appMenu->activeColumn() > 0) {
            appMenu->closeFolder();
        }
        return true;
    } else if (keyCode == KeyPress::returnKey ||
            keyCode == KeyPress::spaceKey ||
            keyCode == KeyPress::rightKey) {
        appMenu->clickSelected();
        return true;
    }
    return false;
}

void AppMenuPage::visibilityChanged() {
    if (isVisible()) {
        grabKeyboardFocus();
    }
}

void AppMenuPage::resized() {

}