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

AppMenuPage::AppMenuPage() :
Configurable(&PocketHomeApplication::getInstance()->getConfig(),
{

    MainConfigFile::backgroundKey
})
{
    ComponentConfigFile& config = PocketHomeApplication::getInstance()
            ->getComponentConfig();
    setWantsKeyboardFocus(true);
    setExplicitFocusOrder(1);

    appMenu = new AppMenuComponent(appConfig);
    addAndMakeVisible(appMenu);

    ComponentConfigFile::ComponentSettings frameSettings =
            config.getComponentSettings(ComponentConfigFile::menuFrameKey);
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
    std::function<void(Label*, String) > positionLabel =
            [this, &config](Label * label, String componentKey)
            {
                config.getComponentSettings(componentKey).applyBounds(label);
                label->setFont(Font(label->getHeight()));
                label->setWantsKeyboardFocus(false);
                addAndMakeVisible(label);
            };
    /* Setting the clock */
    positionLabel(&(clock.getLabel()), ComponentConfigFile::clockIconKey);
    clock.getLabel().setJustificationType(Justification::centredRight);

    loadAllConfigProperties();
    batteryIcon = new BatteryIcon();
    wifiIcon = new WifiIcon();
    addAndMakeVisible(batteryIcon);
    addAndMakeVisible(wifiIcon);

    powerButton = new VectorImageButton(ComponentConfigFile::powerButtonKey,
            "Power");
    settingsButton =
            new VectorImageButton(ComponentConfigFile::settingsButtonKey,
            "Settings");
    powerButton->addListener(this);
    settingsButton->addListener(this);
    addAndMakeVisible(powerButton);
    addAndMakeVisible(settingsButton);

    powerPage = new PowerPageComponent();
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
    //don't interrupt animation
    if (Desktop::getInstance().getAnimator().isAnimating(appMenu))return false;
    int keyCode = key.getKeyCode();
    if (keyCode == KeyPress::tabKey){
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
    ComponentConfigFile::ComponentSettings frameSettings =
            config.getComponentSettings(ComponentConfigFile::menuFrameKey);
    ComponentConfigFile::ComponentSettings menuSettings =
            config.getComponentSettings(ComponentConfigFile::appMenuKey);
    menuSettings.applyBounds(appMenu);
    if (frame != nullptr)
    {
        frameSettings.applyBounds(frame);
        frame->setTransformToFit(frame->getBounds().toFloat(),
                RectanglePlacement::stretchToFit);
    }
    config.getComponentSettings(ComponentConfigFile::batteryIconKey)
            .applyBounds(batteryIcon);
    config.getComponentSettings(ComponentConfigFile::wifiIconKey)
            .applyBounds(wifiIcon);
    config.getComponentSettings(ComponentConfigFile::powerButtonKey)
            .applyBounds(powerButton);
    config.getComponentSettings(ComponentConfigFile::settingsButtonKey)
            .applyBounds(settingsButton);
    Label * clockLabel = &(clock.getLabel());
    config.getComponentSettings(ComponentConfigFile::clockIconKey)
            .applyBounds(clockLabel);
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