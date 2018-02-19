#include "../../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../../PokeLookAndFeel.h"
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "AppMenuComponents/PagedAppMenu.h"
#include "AppMenuComponents/ScrollingAppMenu.h"
#include "HomePage.h"

HomePage::HomePage() :
PageComponent("HomePage"),
Configurable(&PocketHomeApplication::getInstance()->getConfig(),
{
    MainConfigFile::backgroundKey
}),
frame(ComponentConfigFile::menuFrameKey, 0, RectanglePlacement::stretchToFit),
powerButton(ComponentConfigFile::powerButtonKey),
settingsButton(ComponentConfigFile::settingsButtonKey),
appMenu(new ScrollingAppMenu(appConfig))
{
    addMouseListener(this, false);
    appMenu->setPopupCallback([this](AppMenuPopupEditor * newEditor)
    {
        showPopupEditor(newEditor);
    });
    setWantsKeyboardFocus(true);

    addAndMakeVisible(appMenu);
    addAndMakeVisible(frame);
    addAndMakeVisible(clock);

    loadAllConfigProperties();
    addAndMakeVisible(batteryIcon);
    addAndMakeVisible(wifiIcon);

    powerButton.addListener(this);
    powerButton.setWantsKeyboardFocus(false);
    addAndMakeVisible(powerButton);

    settingsButton.addListener(this);
    settingsButton.setWantsKeyboardFocus(false);
    addAndMakeVisible(settingsButton);

    settingsPage = new SettingsPage();
}

HomePage::~HomePage()
{
}

void HomePage::stopWaitingOnLaunch()
{
    appMenu->stopWaitingForLoading();
}

/**
 * Add a pop-up editor window to the page.
 */
void HomePage::showPopupEditor(AppMenuPopupEditor* editor)
{
    popupEditor = editor;
    if (popupEditor != nullptr)
    {
        addAndMakeVisible(popupEditor);
        popupEditor->setCentrePosition(getBounds().getCentreX(),
                getBounds().getCentreY());
    }
}



void HomePage::loadConfigProperties(ConfigFile * config, String key)
{
    MainConfigFile& mainConf = PocketHomeApplication::getInstance()->getConfig();
    if (mainConf == *config && key == MainConfigFile::backgroundKey)
    {
        String background = mainConf.getConfigString
                (MainConfigFile::backgroundKey);
        if (background.containsOnly("0123456789ABCDEFXabcdefx"))
        {
            setBackgroundImage(Image::null);
            Colour bgColour(background.getHexValue32());
            setColour(backgroundColourId,bgColour.withAlpha(1.0f));
        } else
        {
            setBackgroundImage(ImageFileFormat::loadFrom(assetFile(background)));
        }
    }
}


//Forward all clicks (except button clicks) to the appMenu 

void HomePage::mouseDown(const MouseEvent &event)
{
    if(event.mods.isPopupMenu() || event.mods.isCtrlDown())
    {
        appMenu->openPopupMenu(false);
    }
}

void HomePage::pageButtonClicked(Button * button)
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


bool HomePage::keyPressed(const KeyPress& key)
{
    //don't interrupt animation or loading
    if (Desktop::getInstance().getAnimator().isAnimating(appMenu)
            || appMenu->isLoading())
    {
        return true;
    }
    else return appMenu->keyPressed(key);
}

void HomePage::visibilityChanged()
{
    if (isVisible())
    {
        grabKeyboardFocus();
    }
}

void HomePage::pageResized()
{
    appMenu->applyConfigBounds();
    frame.applyConfigBounds();
    clock.applyConfigBounds();
    batteryIcon.applyConfigBounds();
    wifiIcon.applyConfigBounds();
    powerButton.applyConfigBounds();
    settingsButton.applyConfigBounds();
    if (popupEditor != nullptr)
    {
        popupEditor->applyConfigBounds();
        popupEditor->setCentrePosition(getBounds().getCentreX(),
                getBounds().getCentreY());
    }
}
