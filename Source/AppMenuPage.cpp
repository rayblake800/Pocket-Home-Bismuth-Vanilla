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
#include "Main.h"

AppMenuPage::AppMenuPage(LauncherComponent * launcherComponent) {
    setWantsKeyboardFocus(true);
    Rectangle<int> appMenuBorder = getScreenSize();
    appMenuBorder.setLeft(10);
    appMenuBorder.setTop(30);
    appMenu = new AppMenuComponent(getConfigJSON(), appMenuBorder);
    addAndMakeVisible(appMenu);

    std::function<Drawable*(String,Colour)>loadSVG=
    [this](String filename,Colour fillColour)->Drawable*{
      File svgFile=assetFile(filename);
      if(!svgFile.exists()){
          return NULL;
      }
      ScopedPointer<XmlElement> svgElement=XmlDocument::parse(svgFile);
      Drawable * drawable= Drawable::createFromSVG(*svgElement);
      drawable->replaceColour(Colours::black,fillColour);
      addAndMakeVisible(drawable);
      drawable->setTransformToFit(Desktop::getInstance().getDisplays().getMainDisplay().userArea.toFloat(),
              RectanglePlacement::stretchToFit);
      return drawable;
    };
    innerFrame=loadSVG("innerFrame.svg",PokeLookAndFeel::chipPurple);
    outerFrame=loadSVG("outerFrame.svg",PokeLookAndFeel::medGrey);
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