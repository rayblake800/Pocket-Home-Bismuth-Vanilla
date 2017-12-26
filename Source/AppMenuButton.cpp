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

//Create a new button representing an application or folder

AppMenuButton::AppMenuButton(DesktopEntry desktopEntry, int index, int column, int width, int height)
: TextButton(desktopEntry.getName()),
desktopEntry(desktopEntry), index(index), column(column) {
    setSize(width, height);

    String iconPath = desktopEntry.getIconPath();
    if (iconPath == "") {
        iconPath = "/usr/share/pocket-home/appIcons/";
        if (isFolder())iconPath += "filebrowser.png";
        else iconPath += "default.png";
    }
    DBG(String("Found icon path " + iconPath));
    appIcon = createImageFromFile(File(iconPath));
}

//Set whether this button is currently selected.

void AppMenuButton::setSelected(bool select) {
    selected = select;
}

//return true if this button is for an application folder

bool AppMenuButton::isFolder() {
    return desktopEntry.getType() == DesktopEntry::DIRECTORY;
}

//return the display name of the associated application

String AppMenuButton::getAppName() {
    return desktopEntry.getName();
}

//return application shell command or directory path.

String AppMenuButton::getCommand() {
    String command=desktopEntry.getExec();
    if(desktopEntry.terminal()){
        command = String(std::getenv("TERM"))+" -e "+command;
    }
    return command;
}

//return button position in its column

int AppMenuButton::getIndex() {
    return index;
}

//return button's column in the AppMenu

int AppMenuButton::getColumn() {
    return column;
}

//If called, the button will not be drawn outside of clipRegion

void AppMenuButton::setClipRegion(Rectangle<int>clipRegion) {
    this->clipRegion = clipRegion;
    clip = true;
}

void AppMenuButton::paint(Graphics& g) {
    Rectangle<int> border = getBounds().withPosition(0,0);
    if (clip) {
        Rectangle<int>clip = clipRegion.translated(-getScreenX(), -getScreenY());
        if(border.intersects(clip)){
            border = border.getIntersection(clip);
        }
        g.reduceClipRegion(clip);
    }
    //background
    g.setColour(selected ? PokeLookAndFeel::chipPurple : PokeLookAndFeel::medGrey);
    //g.setOpacity(selected?.2:.8);
    g.fillRect(border);
    g.setOpacity(1);
    //app icon
    Rectangle<float> imgBox=getBounds().withPosition(0,0).toFloat();
    imgBox.setWidth(imgBox.getHeight());
    imgBox.reduce(2,2);
    if (getScreenX() < 0) {
        imgBox.setX(2 - getScreenX());
    }
    if (imgBox.getRight() > getRight()) {
        imgBox.setRight(getRight() - 2);
    }
    g.drawImageWithin(appIcon, imgBox.getX(), imgBox.getY(),
            imgBox.getWidth(), imgBox.getHeight(), RectanglePlacement::centred, false);
    //app title
    g.setColour(Colours::white);
    g.setFont(Font(15.00f, Font::plain));
    Rectangle<float> textBox=getBounds().withPosition(0,0).toFloat();
    textBox.setLeft(imgBox.getRight());
    textBox.reduce(4,4);
    g.drawText(getAppName(),textBox,Justification::centredLeft, true);
    g.setColour(Colour(0x4D4D4D));
    g.setOpacity(selected ? 1.0 : 0.8);
    g.drawRect(border, 2);
}
