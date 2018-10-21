#include "Source/AppMenu/MenuComponents/MenuButton.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "FolderComponent.h"
#include "AppMenu.h"

/*
 * Creates a new MenuButton component for a menu item.
 AppMenu::MenuButton::*/MenuButton(MenuItem menuItem)
{
} AppMenu::MenuButton::~MenuButton() { }/*
 * Sets the location relative to the button's bounds where the
 *         button's icon will be drawn.
 */void AppMenu::MenuButton::setIconBounds(const juce::Rectangle<float>& newBounds)
{
}

/*
 * Sets the location relative to the button's bounds where the
 *         button's title will be printed.
 */void AppMenu::MenuButton::setTitleBounds(const juce::Rectangle<float>& newBounds)
{
}/*
 * Updates the component if necessary whenever its menu data
 *         changes.
 */void AppMenu::MenuButton::dataChanged(DataField changedField)
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
 * Runs whenever the menu button is resized, just before
 *         recalculating icon bounds, text bounds, and font size.  

/*
 * Calls AppMenu::MenuButton::menuButtonResized() and re-calculates title and icon layout
 *         whenever the button's bounds change.
 */
void AppMenu::MenuButton::resized() final
{
}

/*
 * Draws the button object.
 */void AppMenu::MenuButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
