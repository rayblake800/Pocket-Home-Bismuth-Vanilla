/*
  ==============================================================================

    AppMenuButton.cpp
    Created: 17 Dec 2017 3:57:53pm
    Author:  anthony

  ==============================================================================
 */

#include "AppMenuButton.h"
#include "PokeLookAndFeel.h"
#include "Utils.h"

AppMenuButton::AppMenuButton(DesktopEntry desktopEntry)
:ShapeButton(desktopEntry.getName(),Colours::transparentBlack,Colours::transparentBlack,Colours::transparentBlack){
    this->shell=shell;
    setSize(416, 64);
    appName = desktopEntry.getName();
    
    std::string iconPath=desktopEntry.getIconPath();
    if(iconPath=="")iconPath="appIcons/default.png";
    appIcon = createImageFromFile(File(iconPath));
}


void AppMenuButton::setSelected(bool selected) {
    this->selected = selected;
}

bool AppMenuButton::isFolder(){
    return folder;
}

void AppMenuButton::paint(Graphics& g) {
    //background
    int x = getX();
    int y = getY();
    int width = getWidth();
    int height = getHeight();
    g.setColour(selected ? selectedFillColour : fillColour);
    g.fillRoundedRectangle(x, y, width, height, 10.000f);
    //app icon
    Rectangle<float> imgBox(x + 2, +2, height - 4, height - 4);
    g.drawImageWithin(appIcon,x+2,y+2,height-4,height-4,RectanglePlacement::centred,false);
    //app title
    g.setColour(Colours::black);
    g.setFont(Font(15.00f, Font::plain));
    g.drawText(appName, x + height + 8, y + 4, width - height - 8, height - 8,
            Justification::centredLeft, true);
}