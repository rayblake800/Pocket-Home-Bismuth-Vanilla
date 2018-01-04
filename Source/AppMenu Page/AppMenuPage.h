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
#include "../Basic Components/VectorImageButton.h"
#include "Info Components/ClockMonitor.h"
#include "Info Components/BatteryIcon.h"
#include "Info Components/WifiIcon.h"
#include "AppMenuComponent.h"

class AppMenuPage : public Component, private Button::Listener{
public:
    AppMenuPage();
    virtual ~AppMenuPage();
    void stopWaitingOnLaunch();

    //TODO: remove these after implementing configuration change messaging
    void setClockAMPM(bool);
    void setColorBackground(const String&);
    void setImageBackground(const String&);
    void setClockVisible(bool);
private:
    void buttonClicked(Button *) override;
    bool keyPressed(const KeyPress &) override;
    void visibilityChanged() override;
    void resized() override;
    void paint(Graphics &) override;

    AppMenuComponent appMenu;
    Colour bgColor;
    ClockMonitor clock;
    ScopedPointer<BatteryIcon> batteryIcon;
    ScopedPointer<WifiIcon> wifiIcon;
    ScopedPointer<VectorImageButton> powerButton;
    ScopedPointer<VectorImageButton> settingsButton;
    
    PowerPageComponent powerPage;
    SettingsPageComponent settingsPage;
    
    ScopedPointer<Image> bgImage;
    ScopedPointer<Drawable> frame;
    //for each of these values, screenSize/val=totalSize
    //static const int leftFrameRatio=

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPage);
};