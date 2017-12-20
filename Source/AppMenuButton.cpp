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
:TextButton(desktopEntry.getName()){
    this->shell=shell;
    setSize(416,Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight()/6);
    appName = desktopEntry.getName();
    
    String iconPath=desktopEntry.getIconPath();
    if(iconPath=="")iconPath="appIcons/default.png";
    appIcon = createImageFromFile(File(iconPath));
}


void AppMenuButton::setSelected(bool select) {
    selected = select;
}

bool AppMenuButton::isFolder(){
    return folder;
}

String AppMenuButton::getName(){
    return appName;
}

void AppMenuButton::paint(Graphics& g) {
    //background
    int x =0;// getX();
    int y =0;// getY();
    int width = getWidth();
    int height = getHeight();
    //std::cout<<"drawing "<<appName<<" at "<<x<<","<<y<<","<<width<<","<<height<<"\n";
    g.setColour(Colours::black);
    g.drawRoundedRectangle(Rectangle<float>(x,y,width,height),10.0f,2.0f);
    g.setColour(selected ? selectedFillColour : fillColour);
    g.setOpacity(.5);
    g.fillRoundedRectangle(x, y, width, height, 10.000f);
    g.setOpacity(1);
    //app icon
    Rectangle<float> imgBox(x + 2, +2, height - 4, height - 4);
    g.drawImageWithin(appIcon,x+2,y+2,height-4,height-4,RectanglePlacement::centred,false);
    //app title
    g.setColour(Colours::black);
    g.setFont(Font(15.00f, Font::plain));
    g.drawText(appName, x + height + 8, y + 4, width - height - 8, height - 8,
            Justification::centredLeft, true);
}
