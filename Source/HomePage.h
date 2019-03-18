#pragma once
/**
 * @file  HomePage.h
 * 
 * @brief  Creates the application's primary UI page.
 */

#include "Widgets_OverlaySpinner.h"
#include "DesktopEntry_Loader.h"
#include "Icon_Loader.h"
#include "Page_Component.h"
#include "Theme_Image_Component.h"
#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_ConfigListener.h"
#include "Widgets_DrawableImageButton.h"
#include "Info_Clock.h"
#include "Info_BatteryIcon.h"
#include "Layout_Component_Manager.h"
#include "Info_ConnectionIcon.h"
#include "Util_Commands.h"
#include "AppMenu.h"

/**
 * @brief  The root Page::Component within the application.
 *
 *  HomePage displays the application menu, time, battery percentage, and Wifi
 * connection state. HomePage also provides navigation buttons used to access 
 * the power and settings pages.
 *
 *  HomePage is Pocket-Home's base page component, and the first page shown,
 * except perhaps the login page. The HomePage will continue to exist as long as
 * Pocket-Home is running.
 */
class HomePage : public Page::Component, public Theme::Image::ConfigListener
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
     * @brief  Grabs keyboard focus when the page becomes visible.
     */
    void visibilityChanged() override;

    /**
     * @brief  Updates all child component bounds when the page is resized.
     */
    void pageResized() override;

    class PageListener: public juce::Button::Listener
    {
    public:
        PageListener(HomePage& homePage) : homePage(homePage) { }

    private:
        /**
         * @brief  Opens the power page or the quick settings page, depending on 
         *         which button was clicked.
         * 
         * @param button  The page button that was clicked.
         */
        void buttonClicked(juce::Button* button) override;

        HomePage& homePage;
    };
    PageListener pageListener;

    /* Loads system commands.  This resource should exist as long as the home
       page exists. */
    Util::Commands systemCommands;

    /* Loads desktop entry file data.  This resource should exist as long as
       the home page exists. */
    DesktopEntry::Loader entryLoader;

    /* Loads icon images. This resource should exist as long as the home page
       exists. */
    Icon::Loader iconLoader;
    
    /* Displays the time: */
    Info::Clock clock;
   
    /* The application menu. This is automatically updated whenever the menu 
       type preference changes in MainConfigFile. */
    std::unique_ptr<juce::Component> appMenu;
    
    /* Displays the current battery status: */
    Info::BatteryIcon batteryIcon;
    
    /* Displays the current wifi status: */
    Info::ConnectionIcon wifiIcon;

    /* Loads the background image and ensures the image asset JSON resource
     * remains available: */
    const Theme::Image::ConfigFile imageConfig;

    /* Opens the power page when clicked: */
    Theme::Image::Component<Widgets::DrawableImageButton> powerButton;
    
    /* Opens the settings page when clicked: */
    Theme::Image::Component<Widgets::DrawableImageButton> settingsButton;

    /* Page frame image. This component is entirely decorative. */
    Theme::Image::Component<> frame;

    /* Manages the layouts of configurable components: */ 
    juce::Array<Layout::Component::Manager> layoutManagers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomePage);
};
