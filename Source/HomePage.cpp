#include "HomePage.h"
#include "Layout_Component_JSONKeys.h"
#include "Theme_Image_JSONKeys.h"
#include "Config_MainFile.h"
#include "Config_MainKeys.h"
#include "AssetFiles.h"
#include "AppMenu.h"
#include "AppMenu_ConfigFile.h"

/*
 * Initializes all page components and creates the AppMenu.
 */
HomePage::HomePage() :
PageComponent("HomePage"),
frame(Theme::Image::JSONKeys::menuFrame, 0,
        juce::RectanglePlacement::stretchToFit),
powerButton(Theme::Image::JSONKeys::powerButton),
settingsButton(Theme::Image::JSONKeys::settingsButton)
{
#    if JUCE_DEBUG
    setName("HomePage");
#    endif
    addTrackedKey(Config::MainKeys::backgroundKey);

    using namespace Layout::Component;
    layoutManagers.add(Manager(&wifiIcon, JSONKeys::wifiIcon));
    layoutManagers.add(Manager(&powerButton, JSONKeys::powerButton));
    layoutManagers.add(Manager(&settingsButton, JSONKeys::settingsButton));
    layoutManagers.add(Manager(&frame, JSONKeys::menuFrame));

    setWantsKeyboardFocus(false);
    addAndMakeVisible(frame);
    addAndMakeVisible(clock);

    addAndMakeVisible(batteryIcon);
    addAndMakeVisible(wifiIcon);

    powerButton.addListener(this);
    powerButton.setWantsKeyboardFocus(false);
    addAndMakeVisible(powerButton);

    settingsButton.addListener(this);
    settingsButton.setWantsKeyboardFocus(false);
    addAndMakeVisible(settingsButton);
    
    AppMenu::ConfigFile appConfig;
    appMenu.reset(AppMenu::createAppMenu()); 
    const AppMenu::Format menuFormat = appConfig.getMenuFormat();
    DBG("HomePage::" << __func__ << ": Initialized "
            << appConfig.formatToString(menuFormat) << " AppMenu");
    appMenu->setBounds(getLocalBounds());
    addAndMakeVisible(appMenu.get());
    appMenu->toBack();
    if(isShowing())
    {
        appMenu->grabKeyboardFocus();
    }

    loadAllConfigProperties();
}

/*
 * Tracks page background changes.
 */
void HomePage::configValueChanged(const juce::Identifier& key)
{
    using juce::String;
    using juce::Image;
    using juce::Colour;
    Config::MainFile mainConfig;
    if (key == Config::MainKeys::backgroundKey)
    {
        String background = mainConfig.getHomeBackground();
        if (background.containsOnly("0123456789ABCDEFXabcdefx"))
        {
            setBackgroundImage(Image());
            Colour bgFill(background.getHexValue32());
            setColour(backgroundColourId, bgFill.withAlpha(1.0f));
        }
        else
        {
            setBackgroundImage(AssetFiles::loadImageAsset(background));
        }
    }
    else
    {
        //invalid key!
        jassertfalse;
    }
}

/*
 * Opens the power page or the settings page, depending on which button
 * was clicked.
 */
void HomePage::pageButtonClicked(juce::Button * button)
{
    if (button == &settingsButton)
    {
        pushPageToStack(PageComponent::PageType::QuickSettings);
    }
    else if (button == &powerButton)
    {
        pushPageToStack(PageComponent::PageType::Power,
                Layout::Transition::Type::moveRight);
    }
}

/*
 * Grab keyboard focus when the page becomes visible.
 */
void HomePage::visibilityChanged()
{
    if (isShowing())
    {
        juce::MessageManager::callAsync([this]()
        {
            if(appMenu != nullptr)
            {
                appMenu->grabKeyboardFocus();
            }
        });
    }
}

/*
 * Update all child component bounds when the page is resized.
 */
void HomePage::pageResized()
{
    if (appMenu != nullptr)
    {
        appMenu->setBounds(getLocalBounds());
    }
    batteryIcon.applyConfigBounds();
    for(Layout::Component::Manager& layoutManager : layoutManagers)
    {
        layoutManager.applyConfigBounds();
    }
}
