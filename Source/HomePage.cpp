#include "ConfigurableImageComponent.h"
#include "MainKeys.h"
#include "ComponentConfigKeys.h"
#include "PokeLookAndFeel.h"
#include "AssetFiles.h"
#include "AppMenu/AppMenu.h"
#include "AppMenu/ConfigFile.h"
#include "HomePage.h"

HomePage::HomePage() :
PageComponent("HomePage"),
frame(ComponentConfigKeys::menuFrameKey, 0,
        juce::RectanglePlacement::stretchToFit),
powerButton(ComponentConfigKeys::powerButtonKey),
settingsButton(ComponentConfigKeys::settingsButtonKey)
{
#    if JUCE_DEBUG
    setName("HomePage");
#    endif
    addTrackedKey(Config::MainKeys::backgroundKey);

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
 * Tracks page background and menu type changes. Only the MainConfigFile 
 * should be calling this.  Depending on the key provided, this will update
 * the page background or recreate the AppMenu.
 */
void HomePage::configValueChanged(const juce::Identifier& key)
{
    using juce::String;
    using juce::Image;
    using juce::Colour;
    Config::MainFile mainConfig;
    if (key == Config::MainKeys::backgroundKey)
    {
        String background = mainConfig.getConfigValue<String>
                (Config::MainKeys::backgroundKey);
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
                TransitionAnimator::moveRight);
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
    frame.applyConfigBounds();
    clock.applyConfigBounds();
    batteryIcon.applyConfigBounds();
    wifiIcon.applyConfigBounds();
    powerButton.applyConfigBounds();
    settingsButton.applyConfigBounds();
}
