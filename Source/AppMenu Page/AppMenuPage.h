/*
  ==============================================================================

    AppMenuPage.h
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */
#pragma once

#include "../LauncherComponent.h"
#include "../Power/PowerPageComponent.h"
#include "../Settings/SettingsPageComponent.h"
#include "../Configuration/AppConfigFile.h"
#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "Popup Editor Components/PopupEditorComponent.h"
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

    //TODO: remove these after implementing configuration change messaging

protected:
    /**
     * Tracks page background changes
     * @param config should be the MainConfigFile
     * @param key should be the background key
     */
    void loadConfigProperties(ConfigFile * config, String key);
private:    
    void setColorBackground(const String&);
    void setImageBackground(const String&);
    void buttonClicked(Button *) override;
    bool keyPressed(const KeyPress &) override;
    void visibilityChanged() override;
    void resized() override;
    void paint(Graphics &) override;

    AppConfigFile appConfig;
    
    Colour bgColor;
    ClockMonitor clock;
    ScopedPointer<AppMenuComponent> appMenu;
    BatteryIcon batteryIcon;
    WifiIcon wifiIcon;
    
    ConfigurableImageButton powerButton;
    ConfigurableImageButton settingsButton;

    PowerPageComponent powerPage;
    ScopedPointer<SettingsPageComponent> settingsPage;

    ScopedPointer<Image> bgImage;
    ConfigurableImageComponent frame;
    
    ScopedPointer<PopupEditorComponent> popupEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPage);
};