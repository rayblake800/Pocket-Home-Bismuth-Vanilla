/*
  ==============================================================================

    AppMenuPage.cpp
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */

#include "AppMenuPage.h"
#include "PokeLookAndFeel.h"
#include "Utils.h"
#include "ConfigFile.h"
#include "Main.h"

AppMenuPage::AppMenuPage(LauncherComponent * launcherComponent) {
    setWantsKeyboardFocus(true);
    setExplicitFocusOrder(1);
    appMenu = new AppMenuComponent();
    addAndMakeVisible(appMenu);
    ConfigFile * config = ConfigFile::getInstance();
    ConfigFile::ComponentSettings frameSettings =
            config->getComponentSettings(ConfigFile::MENU_FRAME);
    std::function < Drawable * (int) > loadSVG =
            [this, &frameSettings](int assetIndex) ->Drawable* {
                String filename = frameSettings.getAssetFiles()[assetIndex];
                Colour colour = frameSettings.getColours()[assetIndex];
                File svgFile = assetFile(filename);
                if (!svgFile.exists()) {
                    return nullptr;
                }
                ScopedPointer<XmlElement> svgElement = XmlDocument::parse(svgFile);
                Drawable * drawable = Drawable::createFromSVG(*svgElement);
                drawable->replaceColour(Colours::black, 
                        frameSettings.getColours()[assetIndex]);
                addAndMakeVisible(drawable);
                drawable->setTransformToFit(getWindowSize().toFloat(),
                        RectanglePlacement::stretchToFit);
                drawable->setWantsKeyboardFocus(false);
                return drawable;
            };

    if (frameSettings.getColours().size() >= 2 &&
            frameSettings.getAssetFiles().size() >= 2) {
        innerFrame = loadSVG(0);
        outerFrame = loadSVG(1);
    }
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
        DBG("AppMenuPage:click selected key");
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
    ConfigFile * config = ConfigFile::getInstance();
    ConfigFile::ComponentSettings frameSettings =
            config->getComponentSettings(ConfigFile::MENU_FRAME);
    if (innerFrame != nullptr) {
        frameSettings.applyBounds(innerFrame);
        innerFrame->setTransformToFit(innerFrame->getBounds().toFloat(),
                RectanglePlacement::stretchToFit);
    }
    if (outerFrame != nullptr) {
        frameSettings.applyBounds(outerFrame);
        outerFrame->setTransformToFit(outerFrame->getBounds().toFloat(),
                RectanglePlacement::stretchToFit);
    }
    if (appMenu != nullptr) {
        ConfigFile::ComponentSettings menuSettings =
                config->getComponentSettings(ConfigFile::APP_MENU);
        menuSettings.applyBounds(appMenu);
    }
}