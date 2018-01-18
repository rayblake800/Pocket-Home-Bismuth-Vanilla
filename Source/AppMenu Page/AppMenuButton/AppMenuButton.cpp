/*
  ==============================================================================

    AppMenuButton.cpp
    Created: 17 Dec 2017 3:57:53pm
    Author:  anthony

  ==============================================================================
 */

#include "../../PokeLookAndFeel.h"
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "AppMenuButton.h"

AppMenuButton::AppMenuButton
(String name, int index, int column)
: TextButton(name),
index(index),
column(column)
{
    ComponentConfigFile& configFile = PocketHomeApplication::getInstance()
            ->getComponentConfig();
    ComponentConfigFile::ComponentSettings buttonSettings =
            configFile.getComponentSettings
            (ComponentConfigFile::appMenuButtonKey);
    buttonSettings.applySize(this);
    Array<Colour> colours = buttonSettings.getColours();
    if (colours.size() >= 2)
    {
        fillColour = colours[0];
        selectedFillColour = colours[1];
    }
    setWantsKeyboardFocus(false);
    resized();
}

void AppMenuButton::setSelected(bool select)
{
    selected = select;
}

int AppMenuButton::getIndex() const
{
    return index;
}

int AppMenuButton::getColumn() const
{
    return column;
}

void AppMenuButton::setIndex(int index)
{
    this->index = index;
}

void AppMenuButton::setColumn(int column)
{
    this->column = column;
}

/**
 * @return the icon image used by this button.
 */
const Image& AppMenuButton::getIcon()
{
    return appIcon;
}

Rectangle<int> AppMenuButton::getButtonSize()
{
    ComponentConfigFile& config = PocketHomeApplication::getInstance()
            ->getComponentConfig();
    ComponentConfigFile::ComponentSettings buttonConf =
            config.getComponentSettings(ComponentConfigFile::appMenuButtonKey);
    return buttonConf.getBounds().withPosition(0, 0);
}

void AppMenuButton::paint(Graphics& g)
{
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

void AppMenuButton::resized()
{
    Rectangle<float> bounds = getLocalBounds().toFloat();
    imageBox = bounds.withWidth(bounds.getHeight());
    imageBox.reduce(2, 2);
    textBox = bounds;
    textBox.setLeft(imageBox.getRight());
    textBox.reduce(4, 4);
    //It looks messy if all the fonts are different sizes, so using a default
    //String for size calculations is preferable even if really long names can 
    //get clipped.
    titleFont = fontResizedToFit(titleFont, "DefaultAppNameString",
            textBox.toNearestInt());
}

