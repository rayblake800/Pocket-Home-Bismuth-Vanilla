/*
  ==============================================================================

    AppMenuPage.cpp
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */

#include "../PokeLookAndFeel.h"
#include "../Utils.h"
#include "../Configuration/ConfigFile.h"
#include "../PocketHomeApplication.h"
#include "AppMenuPage.h"

AppMenuPage::AppMenuPage()
{
    ConfigFile * config = ConfigFile::getInstance();
    setWantsKeyboardFocus(true);
    setExplicitFocusOrder(1);

    addAndMakeVisible(&appMenu);

    ConfigFile::ComponentSettings frameSettings =
            config->getComponentSettings(MENU_FRAME);
    std::vector<String> assets = frameSettings.getAssetFiles();
    if (!assets.empty())
    {
        frame = createSVGDrawable(assetFile(assets[0]));
        std::vector<Colour> colours = frameSettings.getColours();
        if (!colours.empty())
        {
            std::vector<Colour> defaults = {Colour(0xff, 0xff, 0xff),
                Colour(0x00, 0x00, 0x00)};
            for (int i = 0; i < defaults.size() && i < colours.size(); i++)
            {
                frame->replaceColour(defaults[i], colours[i]);
            }
        }
        frameSettings.applyBounds(frame);
        frame->setTransformToFit(frame->getBounds().toFloat(),
                RectanglePlacement::stretchToFit);
        addAndMakeVisible(frame);
    }
    std::function<void(Label*, ComponentType) > positionLabel =
            [this, config](Label * label, ComponentType type)
            {
                config->getComponentSettings(type).applyBounds(label);
                label->setFont(Font(label->getHeight()));
                label->setWantsKeyboardFocus(false);
                addAndMakeVisible(label);
            };
    /* Setting the clock */
    positionLabel(&(clock.getLabel()), CLOCK);
    clock.getLabel().setJustificationType(Justification::centredRight);
    String formatclock = config->getConfigString(TIME_FORMAT);
    setClockVisible(config->getConfigBool(SHOW_CLOCK));
    setClockAMPM(formatclock == "ampm");

    String value = config->getConfigString(BACKGROUND);

    bgColor = Colour(0x4D4D4D);
    if (value.length() == 6 && value.containsOnly("0123456789ABCDEF"))
        setColorBackground(value);
    else
        setImageBackground(value);

    addAndMakeVisible(batteryIcon);
    addAndMakeVisible(wifiIcon);

    powerButton = new VectorImageButton(config->getComponentSettings(POWER), 
            "Power");
    settingsButton = 
            new VectorImageButton(config->getComponentSettings(SETTINGS), 
            "Settings");
}

AppMenuPage::~AppMenuPage()
{
}

void AppMenuPage::stopWaitingOnLaunch()
{
    appMenu.stopWaitingOnLaunch();
}

void AppMenuPage::setColorBackground(const String& str)
{
    String value = "FF" + str;
    bgColor = Colour(str.getHexValue32());
    bgImage = nullptr;
}

void AppMenuPage::setImageBackground(const String& str)
{
    String value(str);
    if (value == "") value = "mainBackground.png";
    File f;
    if (value[0] == '~' || value[0] == '/') f = File(value);
    else f = assetFile(value);
    if (bgImage == nullptr)
    {
        bgImage = new Image();
    }
    *bgImage = createImageFromFile(f);
}

void AppMenuPage::setClockVisible(bool visible)
{
    if (visible)
    {
        if (clock.isThreadRunning()) return;
        addAndMakeVisible(clock.getLabel(), 10);
        clock.startThread();
    } else
    {
        if (!clock.isThreadRunning()) return;
        Label& l = clock.getLabel();
        removeChildComponent(&l);
        clock.stopThread(1500);
    }
}

void AppMenuPage::setClockAMPM(bool ampm)
{
    clock.setAmMode(ampm);
}

void AppMenuPage::buttonClicked(Button * button)
{
    PageStackComponent& pageStack = PocketHomeApplication::getInstance()
            ->getMainStack();
    if (button == settingsButton)
    {
        pageStack.pushPage(&settingsPage, 
                PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == powerButton)
    {
        pageStack.pushPage(&powerPage, 
                PageStackComponent::kTransitionTranslateHorizontalLeft);
    }
}

bool AppMenuPage::keyPressed(const KeyPress& key)
{
    //don't interrupt animation
    if (Desktop::getInstance().getAnimator().isAnimating(&appMenu))return false;
    int keyCode = key.getKeyCode();
    if (keyCode == KeyPress::upKey || keyCode == KeyPress::downKey)
    {
        if (keyCode == KeyPress::upKey)appMenu.selectPrevious();
        else appMenu.selectNext();
        return true;
    } else if (keyCode == KeyPress::leftKey || keyCode == KeyPress::escapeKey)
    {
        if (appMenu.activeColumn() > 0)
        {
            appMenu.closeFolder();
        }
        return true;
    } else if (keyCode == KeyPress::returnKey ||
            keyCode == KeyPress::spaceKey ||
            keyCode == KeyPress::rightKey)
    {
        DBG("AppMenuPage:click selected key");
        appMenu.clickSelected();
        return true;
    }
    return false;
}

void AppMenuPage::visibilityChanged()
{
    if (isVisible())
    {
        grabKeyboardFocus();
    }
}

void AppMenuPage::resized()
{
    ConfigFile * config = ConfigFile::getInstance();
    ConfigFile::ComponentSettings frameSettings =
            config->getComponentSettings(MENU_FRAME);
    ConfigFile::ComponentSettings menuSettings =
            config->getComponentSettings(APP_MENU);
    menuSettings.applyBounds(&appMenu);
    if (frame != nullptr)
    {
        frameSettings.applyBounds(frame);
        frame->setTransformToFit(frame->getBounds().toFloat(),
                RectanglePlacement::stretchToFit);
    }
    config->getComponentSettings(BATTERY).applyBounds(&batteryIcon);
    config->getComponentSettings(WIFI).applyBounds(&wifiIcon);
    Label * clockLabel = &(clock.getLabel());
    config->getComponentSettings(CLOCK).applyBounds(clockLabel);
    Font labelFont = clockLabel->getFont();
    labelFont.setHeight(clockLabel->getHeight());
    clockLabel->setFont(labelFont);
}

void AppMenuPage::paint(Graphics &g)
{
    auto bounds = getLocalBounds();
    g.fillAll(bgColor);
    if (bgImage != nullptr)
    {
        g.drawImage(*bgImage, bounds.getX(), bounds.getY(),
                bounds.getWidth(), bounds.getHeight(),
                0, 0, bgImage->getWidth(), bgImage->getHeight(), false);
    }
}