#define APPMENU_IMPLEMENTATION_ONLY
#include "MenuButton.h"

/* Extra characters applied when calculating title width, defining title padding
   space relative to the font size. */
static const constexpr char* titleBuffer = "     ";

/*
 * Creates a new MenuButton component for a menu item.
 */
AppMenu::MenuButton::MenuButton(MenuItem menuItem) : ItemButton(menuItem)
{
    setWantsKeyboardFocus(false);
} 

/*
 * Sets the location relative to the button's bounds where the button's icon 
 * will be drawn.
 */
void AppMenu::MenuButton::setIconBounds(const juce::Rectangle<float>& newBounds)
{
    iconBounds = newBounds;
}

/*
 * Sets the location relative to the button's bounds where the button's title 
 * will be printed.
 */
void AppMenu::MenuButton::setTitleBounds
(const juce::Rectangle<float>& newBounds)
{
    titleBounds = newBounds;
}

/*
 * Updates the component if necessary whenever its menu data changes.
 */
void AppMenu::MenuButton::dataChanged(MenuItem::DataField changedField)
{
    switch(changedField)
    {
        default:
            DBG("You forgot to implement " << __func__ );
    }
}


/*
 * Reloads the button icon image if necessary.
 */
void AppMenu::MenuButton::loadIcon()
{
    using juce::Icon;
    IconLoader iconThread;
    iconThread.loadIcon(icon, getIconBounds().toNearestInt().getWidth(),
    [this](Image iconImg)
    {
        appIcon = iconImg;
        repaint();
    });
}

/*
 * Updates the button's title font to fit the current title bounds.
 */
void AppMenu::MenuButton::updateFont()
{
}

/*
 * Calls AppMenu::MenuButton::menuButtonResized() and re-calculates title and 
 * icon layout whenever the button's bounds change.
 */
void AppMenu::MenuButton::resized()
{
}

/*
 * Draws the button object.
 */
void AppMenu::MenuButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    using juce::Rectangle;
    Rectangle<int> border = getLocalBounds();
    const Rectangle<float>& titleBounds = getTitleBounds();
    const Rectangle<float>& iconBounds = getIconBounds();
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
    g.drawImageWithin(appIcon, iconBounds.getX(), iconBounds.getY(),
            iconBounds.getWidth(), iconBounds.getHeight(),
            RectanglePlacement::xMid | RectanglePlacement::yTop, false);
    // Draw title:
    g.setColour(findColour(textColourId));
    g.setFont(getTitleFont());
    g.drawText(getMenuItem().getTitle(), titleBounds, 
            getTextJustification(), true);
    if (shouldDrawBorder())
    {
        g.setColour(findColour(borderColourId));
        g.drawRect(border, 2);
    }
}
