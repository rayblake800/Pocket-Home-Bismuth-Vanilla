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

std::function<void()> AppMenuButton::reloadAllButtons = []()
{
};

AppMenuButton::AppMenuButton
(String name, int index, int column, IconThread& iconThread)
: Button(name),
ConfigurableComponent(ComponentConfigFile::appMenuButtonKey),
index(index),
column(column),
iconThread(iconThread)
{
    loadAllConfigProperties();
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

/**
 * Sets a callback to run when button data changes and should be
 * reloaded.
 */
void AppMenuButton::setReloadButtonsCallback(std::function<void() > reload)
{
    reloadAllButtons = reload;
}

void AppMenuButton::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
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
    g.setColour(textColour);
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
    titleFont = fontResizedToFit(titleFont, "DefaultAppNameStr",
            textBox.toNearestInt());
}

void AppMenuButton::applyConfigAssets(Array<String> assetNames,
        Array<Colour> colours)
{
    while (colours.size() < 3)
    {
        colours.add(Colours::transparentBlack);
    }
    textColour = colours[0];
    fillColour = colours[1];
    selectedFillColour = colours[2];
}

/**
 * Requests an icon from the icon thread.
 */
void AppMenuButton::loadIcon(String icon)
{
    iconThread.loadIcon(icon, [this](Image iconImg)
    {
        appIcon = iconImg;
        repaint();
    });
}
