/**
 * @file HomePage.h
 * 
 * HomePage is the main page of the application. It displays the app launch
 * menu, assorted system information, and navigation controls to access
 * power and settings pages.
 */
#pragma once

#include "OverlaySpinner.h"
#include "WifiStateManager.h"
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

class HomePage : public PageComponent, public ConfigFile::Listener{
public:
    HomePage(PageFactoryInterface& pageFactory, 
            WifiStateManager& wifiState,
            MainConfigFile& mainConfig,
            ComponentConfigFile& componentConfig);
    
    virtual ~HomePage();

protected:
    /**
     * Tracks page background changes. Only the MainConfigFile should be
     * calling this.
     * @param config should be the MainConfigFile
     * @param key should be the background key
     */
    void loadConfigProperties(ConfigFile* config, String key);
private:

    /**
     * Forward all clicks (except button clicks) to the appMenu so that it can 
     * potentially create a pop-up menu
     */
    virtual void mouseDown(const MouseEvent &event) override;

    /**
     * Set the page background as a solid color.
     * @param color a six-digit hex color string
     */
    void setColorBackground(const String& color);

    /**
     * Set the page background to a background image.
     * @param path the path to an image file.
     */
    void setImageBackground(const String& path);

    /**
     * 
     * @param 
     */
    void pageButtonClicked(Button *) override;

    /**
     * 
     * @param 
     * @return 
     */
    bool keyPressed(const KeyPress &) override;

    /**
     * 
     */
    void visibilityChanged() override;

    /**
     * 
     */
    void pageResized() override;

    AppConfigFile appConfig;
    MainConfigFile& mainConfig;
    ComponentConfigFile& componentConfig;
    
    OverlaySpinner loadingSpinner;
    ClockLabel clock;
    ScopedPointer<AppMenuComponent> appMenu;
    BatteryIcon batteryIcon;
    WifiIcon wifiIcon;

    ConfigurableImageButton powerButton;
    ConfigurableImageButton settingsButton;
    
    ConfigurableImageComponent frame;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomePage);
};
