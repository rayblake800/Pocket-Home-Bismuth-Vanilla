#include "../../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../../Configuration/MainConfigFile.h"
#include "../../PokeLookAndFeel.h"
#include "../../Utils.h"
#include "AppMenuComponents/PagedAppMenu.h"
#include "AppMenuComponents/ScrollingAppMenu.h"
#include "HomePage.h"

HomePage::HomePage() :
PageComponent("HomePage"),
Configurable(new MainConfigFile(),{
    MainConfigFile::backgroundKey,
    MainConfigFile::menuTypeKey
}),
frame(ComponentConfigFile::menuFrameKey, 0, RectanglePlacement::stretchToFit),
powerButton(ComponentConfigFile::powerButtonKey),
settingsButton(ComponentConfigFile::settingsButtonKey)
{	
    addMouseListener(this, false);
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

    settingsPage = new SettingsPage();
    
    addChildComponent(loadingSpinner);
    loadingSpinner.setAlwaysOnTop(true);
    
    loadAllConfigProperties();
}

HomePage::~HomePage() { }

void HomePage::loadConfigProperties(ConfigFile* config, String key)
{
    MainConfigFile mainConf = MainConfigFile();
    if(mainConf == *config)
    {
		if(key == MainConfigFile::backgroundKey)
		{
			String background = mainConf.getConfigValue<String>
					(MainConfigFile::backgroundKey);
			if (background.containsOnly("0123456789ABCDEFXabcdefx"))
			{
				setBackgroundImage(Image::null);
				Colour bgColour(background.getHexValue32());
				setColour(backgroundColourId, bgColour.withAlpha(1.0f));
			}
			else
			{
				setBackgroundImage(ImageFileFormat::loadFrom(assetFile(background)));
			}
		}
		else if(key == MainConfigFile::menuTypeKey)
		{
			if(appMenu != nullptr)
			{
				removeChildComponent(appMenu);
				appMenu = nullptr;
			}
			
			String menuType = mainConf.getConfigValue<String>
					(MainConfigFile::menuTypeKey);
			DBG(String("Menu type is ")+menuType);
			if(menuType == "Scrolling menu")
			{
				appMenu = new ScrollingAppMenu(loadingSpinner);
			}
			else//menuType == "pagedMenu"
			{
				appMenu = new PagedAppMenu(loadingSpinner);
			}
			addAndMakeVisible(appMenu);
			appMenu->toBack();
			pageResized();
		}
    }
}


//Forward all clicks (except button clicks) to the appMenu 

void HomePage::mouseDown(const MouseEvent &event)
{
    if (event.mods.isPopupMenu() || event.mods.isCtrlDown())
    {
        appMenu->openPopupMenu(false);
    }
}

void HomePage::pageButtonClicked(Button * button)
{
    if (button == &settingsButton)
    {
        pushPageToStack(settingsPage,
                PageStackComponent::kTransitionTranslateHorizontal);
    }
    else if (button == &powerButton)
    {
        pushPageToStack(&powerPage,
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

void HomePage::windowFocusChanged(bool windowFocus)
{
    if (!windowFocus)
    {
        appMenu->exitLoadingState();
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
}
