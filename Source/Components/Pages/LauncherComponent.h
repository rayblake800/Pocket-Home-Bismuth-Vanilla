#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sstream>
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../BatteryMonitor.h"
#include "../../ClockMonitor.hpp"
#include "../Basic/SwitchComponent.h"
#include "../Basic/OverlaySpinner.h"
#include "../Basic/VectorImageButton.h"
#include "../BatteryIcon.h"
#include "../WifiIcon.h"
#include "PageStackComponent.h"



class LauncherComponent;
class LibraryPageComponent;
class AppsPageComponent;

//class WifiIconTimer : public Timer {
//public:
//  WifiIconTimer() {};
//  void timerCallback();
//  LauncherComponent* launcherComponent;
//  OwnedArray<VectorImageButton> * buttons;
//};

class LauncherComponent : public Component, private Button::Listener {
public:
    ScopedPointer<ImageComponent> focusButtonPopup;
    ScopedPointer<Label> modeLabel;
    //WifiIconTimer wifiIconTimer;
    Component* defaultPage;
  
    // FIXME: we have no need for the pages/pagesByName if we're using scoped pointers for each page.
    // All these variables do is add an extra string key the compiler can't see through.
    OwnedArray<Component> pages;
    ScopedPointer<PageStackComponent> pageStack;
    HashMap<String, Component *> pagesByName;
    
    bool resize = false;
    
    LauncherComponent();
    ~LauncherComponent();
    
    void paint(Graphics &) override;
    void resized() override;
    void updateIp();
    void setIpVisible(bool);
    
    void setClockAMPM(bool);
    void setColorBackground(const String&);
    void setImageBackground(const String&);
    void setClockVisible(bool);
    
    void hideLaunchSpinner();
    
private:
    ScopedPointer<BatteryIcon> batteryIcon;
    ScopedPointer<WifiIcon> wifiIcon;
    Colour bgColor;
    Label labelip;
    Image bgImage;
    bool hasImg;
    ScopedPointer<ClockMonitor> clock;
    OwnedArray<VectorImageButton> cornerButtons;
  
    void buttonClicked(Button *) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LauncherComponent)
};
