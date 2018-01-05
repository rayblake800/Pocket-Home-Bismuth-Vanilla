/*
  ==============================================================================

    AppMenuButton.cpp
    Created: 17 Dec 2017 3:57:53pm
    Author:  anthony

  ==============================================================================
 */

#include "../PokeLookAndFeel.h"
#include "../Utils.h"
#include "../PocketHomeApplication.h"
#include "AppMenuButton.h"

AppMenuButton::AppMenuButton(DesktopEntry desktopEntry, int index, int column)
:TextButton(desktopEntry.getName()), 
        desktopEntry(desktopEntry), 
        index(index), 
        column(column){
    //setName(desktopEntry.getName());
    ConfigFile& configFile = PocketHomeApplication::getInstance()->getConfig();
    ConfigFile::ComponentSettings buttonSettings =
            configFile.getComponentSettings(APP_MENU_BUTTON);
    buttonSettings.applySize(this);
    std::vector<Colour> colours=buttonSettings.getColours();
    if (colours.size() >= 2) {
        fillColour = colours[0];
        selectedFillColour = colours[1];
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
    resized();
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
    ConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    String command = desktopEntry.getExec();
    if (desktopEntry.terminal()){
        command = config.getConfigString(TERMINAL_LAUNCH_COMMAND) + command;
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


  void AppMenuButton::setIndex(int index){
      this->index=index;
  }

  void AppMenuButton::setColumn(int column){
      this->column=column;
  }

Rectangle<int> AppMenuButton::getButtonSize(){
    ConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    ConfigFile::ComponentSettings buttonConf= 
            config.getComponentSettings(APP_MENU_BUTTON);
    return buttonConf.getBounds().withPosition(0,0);
}

void AppMenuButton::paint(Graphics& g) {
    Rectangle<int> border = getBounds().withPosition(0, 0);
    g.setColour(selected ? selectedFillColour : fillColour);
    g.setOpacity(selected ? .8 : .2);
    g.fillRect(border);
    g.setOpacity(1);
    //app icon
    g.drawImageWithin(appIcon, imageBox.getX(), imageBox.getY(),
            imageBox.getWidth(), imageBox.getHeight(),
            RectanglePlacement::centred, false);
    //app title
    g.setColour(Colours::white);
    g.setFont(titleFont);
    g.drawText(getAppName(), textBox, Justification::centredLeft, true);
    g.setColour(Colour(0x4D4D4D));
    g.setOpacity(selected ? 1.0 : 0.8);
    g.drawRect(border, 2);
}

void AppMenuButton::resized(){
    Rectangle<float> bounds=getLocalBounds().toFloat();
    imageBox=bounds.withWidth(bounds.getHeight());
    imageBox.reduce(2,2);
    textBox=bounds;
    textBox.setLeft(imageBox.getRight());
    textBox.reduce(4,4);
    //It looks messy if all the fonts are different sizes, so using a default
    //String for size calculations is preferable even if really long names can 
    //get clipped.
    titleFont=fontResizedToFit(titleFont,"DefaultAppName",textBox.toNearestInt());
}

