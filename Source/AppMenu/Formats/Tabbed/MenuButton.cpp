#include "AppMenu/Formats/Tabbed/MenuButton.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/MenuComponents/MenuButton.h"

/*
 * Creates a new MenuButton component for a menu item.
 AppMenu::Tabbed::MenuButton::*/MenuButton(MenuItem menuItem)
{
} AppMenu::Tabbed::MenuButton::~MenuButton() { }/*
 * Finds the area relative to the menu button's bounds where the
 *         title should be drawn.
 */juce::Rectangle<float> AppMenu::Tabbed::MenuButton::findTitleBounds() const final
{
}

/*
 * Finds the area relative to the menu button's bounds where the
 *         icon should be drawn.
 */juce::Rectangle<float> AppMenu::Tabbed::MenuButton::findIconBounds() const final
{
}

/*
 * Checks if this button will draw an outline around its border.
 */bool AppMenu::Tabbed::MenuButton::shouldDrawBorder() const final
{
}

/*
 * Checks if this button will fill in its background with its 
 *         background color.
 */bool AppMenu::Tabbed::MenuButton::shouldFillBackground() const final
{
}

/*
 * Gets the text justification of the button title.
 */juce::Justification AppMenu::Tabbed::MenuButton::getTextJustification() const final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
