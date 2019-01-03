#pragma once
#include "OverlaySpinner.h"
#include "DesktopEntry/Loader.h"
#include "Icon/IconLoader.h"
#include "PageComponent.h"
#include "ComponentConfigFile.h"
#include "Config/MainListener.h"
#include "ConfigurableImageButton.h"
#include "ClockLabel.h"
#include "Wifi/Component/StatusIcon.h"
#include "BatteryIcon.h"
#include "SystemCommands.h"
#include "AppMenu/AppMenu.h"

/**
 * @file  HomePage.h
 * 
 * @brief  The application's main Page Component.
 *
 *  HomePage displays the application menu, time, battery percentage, and Wifi
 * connection state. HomePage also provides navigation buttons used to access 
 * the power and settings pages.
 *
 *  HomePage is Pocket-Home's base page component, and the first page shown,
 * except perhaps the login page. The HomePage will continue to exist as long as
 * Pocket-Home is running.
 */
class HomePage : public PageComponent, public Config::MainListener
{
public:
    /**
     * @brief  Initializes all page components and creates the AppMenu.
     */
    HomePage();
    
    virtual ~HomePage() { }
protected:
    /**
     * @brief  Tracks page background changes. 
     *
     *  Only the MainConfigFile should be call this method. This will update the
     * page background. 
     * 
     * @param key  The page background key.
     */
    virtual void configValueChanged(const juce::Identifier& key) override;

private:
    /**
     * @brief  Opens the power page or the quick settings page, depending on 
     *         which button was clicked.
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
    DesktopEntry::Loader entryLoader;

    /* Loads icon images. This resource should exist as long as the home page
       exists. */
    IconLoader iconLoader;
    
    /* Displays the time. */
    ClockLabel clock;
   
    /* The application menu. This is automatically updated whenever the menu 
       type preference changes in MainConfigFile. */
    std::unique_ptr<juce::Component> appMenu;
    
    /* Displays the current battery status. */
    BatteryIcon batteryIcon;
    
    /* Displays the current wifi status. */
    Wifi::StatusIcon wifiIcon;

    /* Opens the power page when clicked. */
    ConfigurableImageButton powerButton;
    
    /* Opens the settings page when clicked. */
    ConfigurableImageButton settingsButton;

    /* Page frame image. This component is entirely decorative. */
    ConfigurableImageComponent frame;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomePage);
};
