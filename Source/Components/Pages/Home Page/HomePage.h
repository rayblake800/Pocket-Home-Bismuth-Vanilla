#pragma once

#include "OverlaySpinner.h"
#include "DesktopEntryLoader.h"
#include "IconLoader.h"
#include "PageComponent.h"
#include "ComponentConfigFile.h"
#include "Config/MainFile.h"
#include "ConfigurableImageButton.h"
#include "ClockLabel.h"
#include "WifiIcon.h"
#include "BatteryIcon.h"
#include "SystemCommands.h"
#include "AppMenu.h"

/**
 * @file HomePage.h
 * 
 * @brief The main page of the application. 
 *
 * It displays the app launch menu, assorted system information, and navigation
 * controls to access power and settings pages.
 */

/**
 * TODO:
 *  -Add configurable key shortcuts
 *  -Complete documentation
 */

class HomePage : public PageComponent, public Config::MainFile::Listener
{
public:
    HomePage();
    
    virtual ~HomePage() { }
protected:
    /**
     * @brief  Tracks page background and menu type changes. 
     *
     * Only the MainConfigFile should be calling this.  Depending on the key 
     * provided, this will update the page background or recreate the AppMenu.
     * 
     * @param key     This should be the background key or the menu type key.
     */
    virtual void configValueChanged(const juce::Identifier& key) override;

private:
    /**
     * @brief  Opens the power page or the settings page, depending on which 
     *         button was clicked.
     * 
     * @param button  The page button that was clicked.
     */
    void pageButtonClicked(juce::Button* button) override;

    /**
     * @brief  Grabs keyboard focus when the page becomes visible.
     */
    void visibilityChanged() override;

    /**
     * @brief  Updates all child component bounds when the page is resized.
     */
    void pageResized() override;

    /* Loads system commands.  This resource should exist as long as the home
       page exists. */
    SystemCommands systemCommands;

    /* Loads desktop entry file data.  This resource should exist as long as
       the home page exists. */
    DesktopEntryLoader entryLoader;

    /* Loads icon images. This resource should exist as long as the home page
       exists. */
    IconLoader iconLoader;
    
    /* Displays the time. */
    ClockLabel clock;
   
    /* The application menu.  This is recreated whenever the menu type 
       preference changes in MainConfigFile. */
    std::unique_ptr<juce::Component> appMenu;
    
    /* Displays the current battery status. */
    BatteryIcon batteryIcon;
    
    /* Displays the current wifi status. */
    WifiIcon wifiIcon;

    /* Opens the power page when clicked. */
    ConfigurableImageButton powerButton;
    
    /* Opens the settings page when clicked. */
    ConfigurableImageButton settingsButton;

    /* Page frame image.  This component is entirely decorative. */
    ConfigurableImageComponent frame;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomePage);
};
