/**
 * @file HomePage.h
 * 
 * HomePage is the main page of the application. It displays the app launch
 * menu, assorted system information, and navigation controls to access
 * power and settings pages.
 */
#pragma once

#include "PowerPage.h"
#include "SettingsPage.h"
#include "OverlaySpinner.h"
#include "PageComponent.h"
#include "AppConfigFile.h"
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

class HomePage : public PageComponent, public Configurable{
public:
    HomePage();
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

    OverlaySpinner loadingSpinner;
    ClockLabel clock;
    ScopedPointer<AppMenuComponent> appMenu;
    BatteryIcon batteryIcon;
    WifiIcon wifiIcon;

    ConfigurableImageButton powerButton;
    ConfigurableImageButton settingsButton;

    PowerPage powerPage;
    ScopedPointer<SettingsPage> settingsPage;
    ConfigurableImageComponent frame;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomePage);
};
