/*
  ==============================================================================

    AppMenuPage.cpp
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */

#include "../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../PokeLookAndFeel.h"
#include "../Utils.h"
#include "../PocketHomeApplication.h"
#include "AppMenuPage.h"

AppMenuPage::AppMenuPage() :
Configurable(&PocketHomeApplication::getInstance()->getConfig(),
{

    MainConfigFile::backgroundKey
}),
frame(ComponentConfigFile::menuFrameKey, 0, RectanglePlacement::stretchToFit),
powerButton(ComponentConfigFile::powerButtonKey),
settingsButton(ComponentConfigFile::settingsButtonKey),
appMenu(appConfig)
{
    setWantsKeyboardFocus(true);
    setExplicitFocusOrder(1);
    
    addAndMakeVisible(appMenu);
    addAndMakeVisible(frame);
    addAndMakeVisible(clock.getLabel());

    loadAllConfigProperties();
    addAndMakeVisible(batteryIcon);
    addAndMakeVisible(wifiIcon);

    powerButton.addListener(this);
    powerButton.setWantsKeyboardFocus(false);
    addAndMakeVisible(powerButton);

    settingsButton.addListener(this);
    settingsButton.setWantsKeyboardFocus(false);
    addAndMakeVisible(settingsButton);

    settingsPage = new SettingsPageComponent(appConfig);
}

AppMenuPage::~AppMenuPage()
{
    if (clock.isThreadRunning())
    {
        clock.stopThread(1000);
    }
}

void AppMenuPage::stopWaitingOnLaunch()
{
    appMenu.stopWaitingForLoading();
}

void AppMenuPage::loadConfigProperties(ConfigFile * config, String key)
{
    MainConfigFile& mainConf = PocketHomeApplication::getInstance()->getConfig();
    if (mainConf == *config && key == MainConfigFile::backgroundKey)
    {
        String background = mainConf.getConfigString
                (MainConfigFile::backgroundKey);
        if (background.containsOnly("0123456789ABCDEF"))
        {
            setColorBackground(background);
        } else
        {
            setImageBackground(background);
        }
    }
}

void AppMenuPage::buttonClicked(Button * button)
{
    PageStackComponent& pageStack = PocketHomeApplication::getInstance()
            ->getMainStack();
    if (button == &settingsButton)
    {
        pageStack.pushPage(settingsPage,
                PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == &powerButton)
    {
        pageStack.pushPage(&powerPage,
                PageStackComponent::kTransitionTranslateHorizontalLeft);
    }
}

void AppMenuPage::setColorBackground(const String& color)
{
    String value = "FF" + color;
    bgColor = Colour(value.getHexValue32());
    bgImage = nullptr;
}

void AppMenuPage::setImageBackground(const String& path)
{
    if(path.isEmpty())
    {
        return;
    }
    File f;
    if (path[0] == '~' || path[0] == '/') f = File(path);
    else f = assetFile(path);
    if (bgImage == nullptr)
    {
        bgImage = new Image();
    }
    *bgImage = createImageFromFile(f);
}

bool AppMenuPage::keyPressed(const KeyPress& key)
{
    //don't interrupt animation or loading
    if (Desktop::getInstance().getAnimator().isAnimating(&appMenu)
            || appMenu.isLoading())
    {
        return false;
    }
    int keyCode = key.getKeyCode();
    if (keyCode == KeyPress::tabKey)
    {
        appMenu.loadButtons();
    }
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
    } else if (key == KeyPress::createFromDescription("CTRL+e"))
    {
        DBG("show editor");
        popupEditor = appMenu.getEditorForSelected();
        if (popupEditor != nullptr)
        {
            addAndMakeVisible(popupEditor);
            DBG(popupEditor->getBounds().toString());
            return true;
        }
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
    appMenu.applyConfigBounds();
    frame.applyConfigBounds();
    clock.getLabel().applyConfigBounds();
    batteryIcon.applyConfigBounds();
    wifiIcon.applyConfigBounds();
    powerButton.applyConfigBounds();
    settingsButton.applyConfigBounds();
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