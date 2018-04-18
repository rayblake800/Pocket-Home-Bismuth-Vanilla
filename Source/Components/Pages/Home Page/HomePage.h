/**
 * @file HomePage.h
 * 
 * HomePage is the main page of the application. It displays the app launch
 * menu, assorted system information, and navigation controls to access
 * power and settings pages.
 */
#pragma once

#include "OverlaySpinner.h"
#include "PageComponent.h"
#include "AppConfigFile.h"
#include "ComponentConfigFile.h"
#include "MainConfigFile.h"
#include "ConfigurableImageButton.h"
#include "ClockLabel.h"
#include "BatteryIcon.h"
#include "WifiIcon.h"
#include "AppMenuPopupEditor.h"
#include "AppMenuComponent.h"

/**
 * TODO:
 *  -Add configurable key shortcuts
 *  -Complete documentation
 */

class HomePage : public PageComponent, public ConfigFile::Listener
{
public:
    HomePage();

    virtual ~HomePage() { }

protected:
    /**
     * Tracks page background and menu type changes. Only the MainConfigFile 
     * should be calling this.  Depending on the key provided, this will update
     * the page background or recreate the AppMenu.
     * 
     * 
     * @param key     This should be the background key or the menu type key.
     */
    void configValueChanged(String key);

private:
    /**
     * Forward all clicks (except button clicks) to the appMenu so that it can 
     * potentially create a pop-up menu
     * 
     * @param event
     */
    virtual void mouseDown(const MouseEvent& event) override;

    /**
     * Opens the power page or the settings page, depending on which button
     * was clicked.
     * 
     * @param button
     */
    void pageButtonClicked(Button* button) override;

    /**
     * Forwards all key events to the AppMenu.
     */
    bool keyPressed(const KeyPress &) override;

    /**
     * Grab keyboard focus when the page becomes visible.
     */
    void visibilityChanged() override;

    /**
     * Update all child component bounds when the page is resized.
     */
    void pageResized() override;

    //Loads AppMenu shortcuts and folder definitions.  This resource should
    //exist as long as the home page exists.
    AppConfigFile appConfig;

    //This spinner activates when the AppMenu is loading.  The AppMenu controls
    //it through a reference passed in on menu creation.
    OverlaySpinner loadingSpinner;
    
    //Displays the time.
    ClockLabel clock;
    
    //The application menu.  This is recreated whenever the menu type preference
    //changes in MainConfigFile
    ScopedPointer<AppMenuComponent> appMenu;
    
    //On pocketChip this displays the current battery status.
    BatteryIcon batteryIcon;
    
    //Displays the current wifi status.
    WifiIcon wifiIcon;

    //Click to this open the power page
    ConfigurableImageButton powerButton;
    
    //Click this to open the settings page
    ConfigurableImageButton settingsButton;

    //Page frame image.  This component is entirely decorative.
    ConfigurableImageComponent frame;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomePage);
};
