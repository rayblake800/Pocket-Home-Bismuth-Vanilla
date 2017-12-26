/*
  ==============================================================================

    AppMenuPage.h
    Created: 25 Dec 2017 5:48:25pm
    Author:  anthony

  ==============================================================================
 */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppMenuComponent.h"
#include "LauncherComponent.h"

class AppMenuPage : public PageStackComponent {
public:
    AppMenuPage(LauncherComponent * launcherComponent);
    virtual ~AppMenuPage();
    void hideLaunchSpinner();
private:
    bool keyPressed(const KeyPress &) override;
    void visibilityChanged() override;
    void resized() override;
    
    ScopedPointer<AppMenuComponent> appMenu;
    LauncherComponent * launcherComponent;
};