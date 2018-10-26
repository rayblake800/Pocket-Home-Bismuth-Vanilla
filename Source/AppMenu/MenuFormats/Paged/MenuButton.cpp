#if 0
#include "./Source/AppMenu/MenuFormats/Paged/MenuButton.h"

#include "AppMenu/MenuComponents/MenuButton.h"

/*
 * Creates a new MenuButton component for a menu item.
 AppMenu::Paged::MenuButton::*/MenuButton(MenuItem menuItem)
{
} AppMenu::Paged::MenuButton::~MenuButton() { }/*
 * Finds the area relative to the menu button's bounds where the
 *         title should be drawn.
 */juce::Rectangle<float> AppMenu::Paged::MenuButton::findTitleBounds() const final
{
}

/*
 * Finds the area relative to the menu button's bounds where the
 *         icon should be drawn.
 */juce::Rectangle<float> AppMenu::Paged::MenuButton::findIconBounds() const final
{
}

/*
 * Checks if this button will draw an outline around its border.
 */bool AppMenu::Paged::MenuButton::shouldDrawBorder() const final
{
}

/*
 * Checks if this button will fill in its background with its 
 *         background color.
 */bool AppMenu::Paged::MenuButton::shouldFillBackground() const final
{
}

/*
 * Gets the text justification of the button title.
 */juce::Justification AppMenu::Paged::MenuButton::getTextJustification() const final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
