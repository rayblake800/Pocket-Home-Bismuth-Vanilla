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

AppMenuButton::AppMenuButton(DesktopEntry desktopEntry,int index,int column)
:TextButton(desktopEntry.getName()),
 desktopEntry(desktopEntry),index(index),column(column){
    setSize(416,Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight()/6);
    
    String iconPath=desktopEntry.getIconPath();
    if(iconPath=="")iconPath="appIcons/default.png";
    appIcon = createImageFromFile(File(iconPath));
}


void AppMenuButton::setSelected(bool select) {
    selected = select;
}

bool AppMenuButton::isFolder(){
    return desktopEntry.getType()==DesktopEntry::DIRECTORY;
}

String AppMenuButton::getAppName(){
    return desktopEntry.getName();
}

String AppMenuButton::getCommand(){
    return desktopEntry.getExec();
}

int AppMenuButton::getIndex(){
    return index;
}

int AppMenuButton::getColumn(){
    return column;
}

void AppMenuButton::paint(Graphics& g) {
    //background
    int x=0;
    int y=0;
    int width = getWidth();
    int height = getHeight();
    //std::cout<<"drawing "<<getAppName()<<" at "<<x<<","<<y<<","<<width<<","<<height<<"\n";
    g.setColour(Colours::black);
    g.drawRect(x,y,width,height,2);
    //g.drawRoundedRectangle(Rectangle<float>(x,y,width,height),10.0f,2.0f);
    g.setColour(selected ? selectedFillColour : fillColour);
    g.setOpacity(.5);
    g.fillRect(x,y,width,height);
    //g.fillRoundedRectangle(x, y, width, height, 10.000f);
    g.setOpacity(1);
    //app icon
    Rectangle<float> imgBox(x + 2, +2, height - 4, height - 4);
    if(getScreenX()<0){
        imgBox.setX(-getScreenX()+2);
    }
    if(imgBox.getRight() > getRight()){
        imgBox.setRight(getRight()-2);
    }
    g.drawImageWithin(appIcon,x+2,y+2,height-4,height-4,RectanglePlacement::centred,false);
    //app title
    g.setColour(Colours::black);
    g.setFont(Font(15.00f, Font::plain));
    g.drawText(getAppName(), x + height + 8, y + 4, width - height - 8, height - 8,
            Justification::centredLeft, true);
}
