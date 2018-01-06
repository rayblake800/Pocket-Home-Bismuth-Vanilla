/*
  ==============================================================================

    AppMenuPage.cpp
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */

#include "../PokeLookAndFeel.h"
#include "../Utils.h"
#include "../PocketHomeApplication.h"
#include "AppMenuPage.h"

AppMenuPage::AppMenuPage()
{
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    setWantsKeyboardFocus(true);
    setExplicitFocusOrder(1);

    appMenu=new AppMenuComponent();
    addAndMakeVisible(appMenu);

    MainConfigFile::ComponentSettings frameSettings =
            config.getComponentSettings(MENU_FRAME);
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
            [this, &config](Label * label, ComponentType type)
            {
                config.getComponentSettings(type).applyBounds(label);
                label->setFont(Font(label->getHeight()));
                label->setWantsKeyboardFocus(false);
                addAndMakeVisible(label);
            };
    /* Setting the clock */
    positionLabel(&(clock.getLabel()), CLOCK);
    clock.getLabel().setJustificationType(Justification::centredRight);

    bgColor = Colour(0x4D4D4D);
    String bgValue=config.getConfigString(MainConfigFile::backgroundKey);
    if (bgValue.length() == 6 && bgValue.containsOnly("0123456789ABCDEF"))
    {
        setColorBackground(bgValue);
    }
    else
    {
        setImageBackground(bgValue);
    }
    batteryIcon=new BatteryIcon();
    wifiIcon=new WifiIcon();
    addAndMakeVisible(batteryIcon);
    addAndMakeVisible(wifiIcon);

    powerButton = new VectorImageButton(config.getComponentSettings(POWER), 
            "Power");
    settingsButton = 
            new VectorImageButton(config.getComponentSettings(SETTINGS), 
            "Settings");
    powerButton->addListener(this);
    settingsButton->addListener(this);
    addAndMakeVisible(powerButton);
    addAndMakeVisible(settingsButton);
    
    powerPage=new PowerPageComponent();
    settingsPage=new SettingsPageComponent();
}

AppMenuPage::~AppMenuPage()
{
    if(clock.isThreadRunning())
    {
        clock.stopThread(1000);
    }
}

void AppMenuPage::stopWaitingOnLaunch()
{
    appMenu->stopWaitingOnLaunch();
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


void AppMenuPage::buttonClicked(Button * button)
{
    PageStackComponent& pageStack = PocketHomeApplication::getInstance()
            ->getMainStack();
    if (button == settingsButton)
    {
        pageStack.pushPage(settingsPage, 
                PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == powerButton)
    {
        pageStack.pushPage(powerPage, 
                PageStackComponent::kTransitionTranslateHorizontalLeft);
    }
}

bool AppMenuPage::keyPressed(const KeyPress& key)
{
    //don't interrupt animation
    if (Desktop::getInstance().getAnimator().isAnimating(appMenu))return false;
    int keyCode = key.getKeyCode();
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
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    MainConfigFile::ComponentSettings frameSettings =
            config.getComponentSettings(MENU_FRAME);
    MainConfigFile::ComponentSettings menuSettings =
            config.getComponentSettings(APP_MENU);
    menuSettings.applyBounds(appMenu);
    if (frame != nullptr)
    {
        frameSettings.applyBounds(frame);
        frame->setTransformToFit(frame->getBounds().toFloat(),
                RectanglePlacement::stretchToFit);
    }
    config.getComponentSettings(BATTERY).applyBounds(batteryIcon);
    config.getComponentSettings(WIFI).applyBounds(wifiIcon);
    config.getComponentSettings(POWER).applyBounds(powerButton);
    config.getComponentSettings(SETTINGS).applyBounds(settingsButton);
    Label * clockLabel = &(clock.getLabel());
    config.getComponentSettings(CLOCK).applyBounds(clockLabel);
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