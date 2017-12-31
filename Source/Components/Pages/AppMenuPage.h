/*
  ==============================================================================

    AppMenuPage.h
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../AppMenuComponent.h"
#include "LauncherComponent.h"

class AppMenuPage : public Component{
public:
    AppMenuPage(LauncherComponent * launcherComponent);
    virtual ~AppMenuPage();
    void stopWaitingOnLaunch();
private:
    bool keyPressed(const KeyPress &) override;
    void visibilityChanged() override;
    void resized() override;
    
    ScopedPointer<AppMenuComponent> appMenu=nullptr;
    ScopedPointer<Drawable> innerFrame=nullptr;
    ScopedPointer<Drawable> outerFrame=nullptr;
    LauncherComponent * launcherComponent;
    
    //for each of these values, screenSize/val=totalSize
    //static const int leftFrameRatio=
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPage);
};