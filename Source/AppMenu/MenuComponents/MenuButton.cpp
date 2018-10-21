#define APPMENU_IMPLEMENTATION_ONLY
#include "MenuButton.h"

/*
 * Creates a new MenuButton component for a menu item.
 */
AppMenu::MenuButton::MenuButton(MenuItem menuItem) : ItemButton(menuItem)
{
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
}

/*
 * Updates the component if necessary whenever its menu data changes.
 */
void AppMenu::MenuButton::dataChanged(MenuItem::DataField changedField)
{
}

/*
 * Reloads the button icon image if necessary.
 */
void AppMenu::MenuButton::loadIcon()
{
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
}
