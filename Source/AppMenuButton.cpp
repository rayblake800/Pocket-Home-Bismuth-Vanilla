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
#include "ConfigFile.h"

AppMenuButton::AppMenuButton(DesktopEntry desktopEntry, int index, int column)
: TextButton(desktopEntry.getName()),
desktopEntry(desktopEntry), index(index), column(column) {
    ConfigFile * configFile = ConfigFile::getInstance();
    ConfigFile::ComponentSettings buttonSettings =
            configFile->getComponentSettings(ConfigFile::APP_MENU_BUTTON);
    buttonSettings.applyComponentBounds(this);
    if (buttonSettings.colours.size() >= 2) {
        fillColour = buttonSettings.colours[0];
        selectedFillColour = buttonSettings.colours[1];
    }

    String iconPath = desktopEntry.getIconPath();
    if (iconPath == "") {
        iconPath = "/usr/share/pocket-home/appIcons/";
        if (isFolder())iconPath += "filebrowser.png";
        else iconPath += "default.png";
    }
    DBG(String("Found icon path " + iconPath));
    appIcon = createImageFromFile(File(iconPath));
    setWantsKeyboardFocus(false);
}

void AppMenuButton::setSelected(bool select) {
    selected = select;
}

bool AppMenuButton::isFolder() {
    return desktopEntry.getType() == DesktopEntry::DIRECTORY;
}

String AppMenuButton::getAppName() {
    return desktopEntry.getName();
}

String AppMenuButton::getCommand() {
    String command = desktopEntry.getExec();
    if (desktopEntry.terminal()) {
        command = String(std::getenv("TERM")) + " -e " + command;
    }
    return command;
}

std::vector<String> AppMenuButton::getCategories() {
    return desktopEntry.getCategories();
}

int AppMenuButton::getIndex() {
    return index;
}

int AppMenuButton::getColumn() {
    return column;
}

void AppMenuButton::paint(Graphics& g) {
    Rectangle<int> border = getBounds().withPosition(0, 0);
    g.setColour(selected ? selectedFillColour : fillColour);
    g.setOpacity(selected ? .8 : .2);
    g.fillRect(border);
    g.setOpacity(1);
    //app icon
    Rectangle<float> imgBox = getBounds().withPosition(0, 0).toFloat();
    imgBox.setWidth(imgBox.getHeight());
    imgBox.reduce(2, 2);
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
    Rectangle<float> textBox = getBounds().withPosition(0, 0).toFloat();
    textBox.setLeft(imgBox.getRight());
    textBox.reduce(4, 4);
    g.drawText(getAppName(), textBox, Justification::centredLeft, true);
    g.setColour(Colour(0x4D4D4D));
    g.setOpacity(selected ? 1.0 : 0.8);
    g.drawRect(border, 2);
}
