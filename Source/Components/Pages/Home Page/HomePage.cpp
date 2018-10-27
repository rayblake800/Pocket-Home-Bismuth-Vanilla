#include "ConfigurableImageComponent.h"
#include "MainKeys.h"
#include "ComponentConfigKeys.h"
#include "PokeLookAndFeel.h"
#include "AssetFiles.h"
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
    addTrackedKey(Config::MainKeys::menuTypeKey);

    setWantsKeyboardFocus(true);
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

    loadAllConfigProperties();
}

/**
 * Tracks page background and menu type changes. Only the MainConfigFile 
 * should be calling this.  Depending on the key provided, this will update
 * the page background or recreate the AppMenu.
 */
void HomePage::configValueChanged(const juce::Identifier& key)
{
    using namespace juce;
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
    
    else if (key == Config::MainKeys::menuTypeKey && appMenu == nullptr)
    {
        appMenu.reset(AppMenu::createAppMenu(AppMenu::Format::Scrolling)); 
        appMenu->setBounds(getLocalBounds());
        addAndMakeVisible(appMenu.get());
        appMenu->toBack();
        if(isShowing())
        {
            appMenu->grabKeyboardFocus();
        }
    }

}

/**
 * Forward all clicks (except button clicks) to the appMenu so that it can 
 * potentially create a pop-up menu
 */
void HomePage::mouseDown(const juce::MouseEvent &event)
{
// Disabled until redesign
#if 0
    if (event.mods.isPopupMenu() || event.mods.isCtrlDown())
    {
        appMenu->openPopupMenu(nullptr);
    }

//Disabled until redesign
#endif
}

/**
 * Opens the power page or the settings page, depending on which button
 * was clicked.
 * 
 * @param button
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

/**
 * Forwards all key events to the AppMenu.
 */
bool HomePage::keyPressed(const juce::KeyPress& key)
{
// Disabled until redesign
#if 0
    using namespace juce;
    //don't interrupt animation or loading
    if (Desktop::getInstance().getAnimator().isAnimating(appMenu)
        || appMenu->isLoading())
    {
        return true;
    }
    else return appMenu->keyPressed(key);

//Disabled until redesign
#else
    return false;
#endif
}

/**
 * Grab keyboard focus when the page becomes visible.
 */
void HomePage::visibilityChanged()
{
    if (isShowing())
    {
        juce::MessageManager::callAsync([this]()
        {
            grabKeyboardFocus();
        });
    }
}

/**
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
