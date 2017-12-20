/*
  ==============================================================================

    AppMenuButton.h
    Created: 17 Dec 2017 3:57:53pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DesktopEntry.h"

class AppMenuButton : public TextButton {
public:
    AppMenuButton(DesktopEntry desktopEntry);
    void paint(Graphics& g) override;

    void setSelected(bool select);
    bool isFolder();
    String getName();
    String getExec();

private:
    String exec;
    DesktopEntry dEntry;
    bool folder = false;
    bool selected = false;
    Image appIcon;
    String appName;
    Colour fillColour = Colour(0xff939089);
    Colour selectedFillColour = Colour(0xffffade8);
};
