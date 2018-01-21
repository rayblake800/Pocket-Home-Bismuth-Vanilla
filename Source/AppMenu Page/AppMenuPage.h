/**
 * @file AppMenuPage.h
 * @author Anthony Brown
 * 
 * AppMenuPage is the main page of the application. It displays the app launch
 * menu, assorted system information, and navigation controls to access
 * power and settings pages.
 */
#pragma once

#include "../LauncherComponent.h"
#include "../Power/PowerPageComponent.h"
#include "../Settings/SettingsPageComponent.h"
#include "../Configuration/AppConfigFile.h"
#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "Popup Editor Components/AppMenuPopupEditor.h"
#include "Info Components/ClockMonitor.h"
#include "Info Components/BatteryIcon.h"
#include "Info Components/WifiIcon.h"
#include "AppMenuComponent.h"

class AppMenuPage : public Component,
public Configurable,
private Button::Listener {
public:
    AppMenuPage();
    virtual ~AppMenuPage();
    void stopWaitingOnLaunch();
    
    /**
     * Add a popup editor window to the page.
     * @param editor will be added, made visible, and positioned/scaled as
     * needed.
     */
    void showPopupEditor(AppMenuPopupEditor* editor);

protected:
    /**
     * Tracks page background changes. Only the MainConfigFile should be
     * calling this.
     * @param config should be the MainConfigFile
     * @param key should be the background key
     */
    void loadConfigProperties(ConfigFile * config, String key);
private:    
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
    void buttonClicked(Button *) override;
    bool keyPressed(const KeyPress &) override;
    void visibilityChanged() override;
    void resized() override;
    void paint(Graphics &) override;

    AppConfigFile appConfig;
    
    Colour bgColor;
    ClockMonitor clock;
    AppMenuComponent appMenu;
    BatteryIcon batteryIcon;
    WifiIcon wifiIcon;
    
    ConfigurableImageButton powerButton;
    ConfigurableImageButton settingsButton;

    PowerPageComponent powerPage;
    ScopedPointer<SettingsPageComponent> settingsPage;

    ScopedPointer<Image> bgImage;
    ConfigurableImageComponent frame;
    
    ScopedPointer<AppMenuPopupEditor> popupEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPage);
};