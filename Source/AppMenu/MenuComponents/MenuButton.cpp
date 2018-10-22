#define APPMENU_IMPLEMENTATION_ONLY
#include "IconLoader.h"
#include "ComponentConfigFile.h"
#include "MenuButton.h"

/* Extra characters applied when calculating title width, defining title padding
   space relative to the font size. */
static const constexpr char* titleBuffer = "     ";

/*
 * Creates a new MenuButton component for a menu item.
 */
AppMenu::MenuButton::MenuButton
(MenuItem menuItem, FolderComponent& sourceFolder) : 
ItemButton(menuItem, sourceFolder)
{
    setWantsKeyboardFocus(false);
} 

/*
 * Recalculates and saves the menu button title bounds.
 */
void AppMenu::MenuButton::updateTitleBounds()
{
    titleBounds = findTitleBounds();
}

/*
 * Recalculates and saves the menu button icon bounds.
 */
void AppMenu::MenuButton::updateIconBounds()
{
    iconBounds = findIconBounds();
}

/*
 * Recreates and saves the font used to draw the menu button's title.
 */
void AppMenu::MenuButton::updateFont()
{
    titleFont = findTitleFont(titleBounds);
    textWidth = findTitleBGWidth(titleBounds, titleFont);
}

/*
 * Updates the title font to fit the current title bounds.
 */
juce::Font AppMenu::MenuButton::findTitleFont
(const juce::Rectangle<float>& titleBounds)
{
    ComponentConfigFile config;
    return juce::Font(config.getFontHeight(ComponentConfigFile::smallText));
}

/*
 * Finds the width of the background area that will be drawn behind the button's
 * title.
 */
int AppMenu::MenuButton::findTitleBGWidth
(const juce::Rectangle<float>& titleBounds, const juce::Font& titleFont)
{
    int width = titleFont.getStringWidth(getMenuItem().getTitle() 
            + titleBuffer);
    return std::min(width, int(titleBounds.getWidth()));
}

/*
 * Updates the component if necessary whenever its menu data changes.
 */
void AppMenu::MenuButton::dataChanged(MenuItem::DataField changedField)
{
    using DataField = MenuItem::DataField;
    if(changedField == DataField::title)
    {
        updateTitleBounds();
        repaint();
    }
    else if(changedField == DataField::icon)
    {
        loadIcon();
    }
}


/*
 * Reloads the button icon image if necessary.
 */
void AppMenu::MenuButton::loadIcon()
{
    using juce::Image;
    IconLoader iconThread;
    iconThread.loadIcon(getMenuItem().getTitle(),
            iconBounds.toNearestInt().getWidth(),
    [this](Image iconImg)
    {
        icon = iconImg;
        repaint();
    });
}

/*
 * Calls AppMenu::MenuButton::menuButtonResized() and re-calculates title and 
 * icon layout whenever the button's bounds change.
 */
void AppMenu::MenuButton::resized()
{
    menuButtonResized();
    updateTitleBounds();
    updateIconBounds();
    updateFont();
}

/*
 * Draws the button object.
 */
void AppMenu::MenuButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    using juce::Rectangle;
    using juce::RectanglePlacement;
    Rectangle<int> border = getLocalBounds();
    if ((iconBounds.isEmpty() || titleBounds.isEmpty()) && 
            !border.isEmpty())
    {
        resized();
    }
    g.setColour(findColour(isSelected() ?
            selectionColourId : backgroundColourId));
    if (shouldFillBackground())
    {
        g.fillRect(border);
    }
    else
    {
        Rectangle<float> textOval = titleBounds.withSizeKeepingCentre
                (textWidth, titleBounds.getHeight());
        g.fillRoundedRectangle(textOval, textOval.getHeight() / 6);
    }
    // Draw icon:
    g.setOpacity(1);
    g.drawImageWithin(icon, iconBounds.getX(), iconBounds.getY(),
            iconBounds.getWidth(), iconBounds.getHeight(),
            RectanglePlacement::xMid | RectanglePlacement::yTop, false);
    // Draw title:
    g.setColour(findColour(textColourId));
    g.setFont(titleFont);
    g.drawText(getMenuItem().getTitle(), titleBounds, 
            getTextJustification(), true);
    if (shouldDrawBorder())
    {
        g.setColour(findColour(borderColourId));
        g.drawRect(border, 2);
    }
}
