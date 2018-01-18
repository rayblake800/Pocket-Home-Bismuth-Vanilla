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
settingsButton(ComponentConfigFile::settingsButtonKey)
{
    setWantsKeyboardFocus(true);
    setExplicitFocusOrder(1);

    appMenu = new AppMenuComponent(appConfig);
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
    appMenu->stopWaitingOnLaunch();
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

void AppMenuPage::setColorBackground(const String& str)
{
    String value = "FF" + str;
    bgColor = Colour(value.getHexValue32());
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

bool AppMenuPage::keyPressed(const KeyPress& key)
{
    //don't interrupt animation or loading
    if (Desktop::getInstance().getAnimator().isAnimating(appMenu)
            || appMenu->waitingOnLaunch())
    {
        return false;
    }
    int keyCode = key.getKeyCode();
    if (keyCode == KeyPress::tabKey)
    {
        appMenu->loadButtons();
    }
    if (keyCode == KeyPress::upKey || keyCode == KeyPress::downKey)
    {
        if (keyCode == KeyPress::upKey)appMenu->selectPrevious();
        else appMenu->selectNext();
        return true;
    } else if (keyCode == KeyPress::leftKey || keyCode == KeyPress::escapeKey)
    {
        if (appMenu->activeColumn() > 0)
        {
            appMenu->closeFolder();
        }
        return true;
    } else if (keyCode == KeyPress::returnKey ||
            keyCode == KeyPress::spaceKey ||
            keyCode == KeyPress::rightKey)
    {
        DBG("AppMenuPage:click selected key");
        appMenu->clickSelected();
        return true;
    } else if (key == KeyPress::createFromDescription("CTRL+e"))
    {
        DBG("show editor");
        popupEditor = appMenu->getEditorForSelected();
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
    ComponentConfigFile& config = PocketHomeApplication::getInstance()
            ->getComponentConfig();
    ComponentConfigFile::ComponentSettings menuSettings =
            config.getComponentSettings(ComponentConfigFile::appMenuKey);
    ComponentConfigFile::ComponentSettings popupSettings =
            config.getComponentSettings(ComponentConfigFile::popupMenuKey);

    menuSettings.applyBounds(appMenu);

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