#define APPMENU_IMPLEMENTATION_ONLY
#include "ComponentConfigFile.h"
#include "AppMenu/Components/MenuButton.h"
#include "AppMenu/Formats/Paged/MenuButton.h"

/*
 * Creates a new MenuButton component for a menu item.
 */
AppMenu::Paged::MenuButton::MenuButton(MenuItem menuItem) :
AppMenu::MenuButton(menuItem) { } 

/*
 * Finds the area relative to the menu button's bounds where the title should be
 * drawn.
 */
juce::Rectangle<float> AppMenu::Paged::MenuButton::findTitleBounds() const 
{
    ComponentConfigFile config;
    const int textHeight = config.getFontHeight(ComponentConfigFile::smallText); 
    return getLocalBounds().withTop(getHeight() - textHeight).toFloat();
}

/*
 * Finds the area relative to the menu button's bounds where the icon should be 
 * drawn.
 */
juce::Rectangle<float> AppMenu::Paged::MenuButton::findIconBounds() const 
{
    ComponentConfigFile config;
    const int textHeight = config.getFontHeight(ComponentConfigFile::smallText); 
    return getLocalBounds().reduced(0, textHeight).toFloat();
}

/*
 * Checks if this button will draw an outline around its border.
 */
bool AppMenu::Paged::MenuButton::shouldDrawBorder() const 
{
    return isSelected();
}

/*
 * Checks if this button will fill in its background with its background color.
 */
bool AppMenu::Paged::MenuButton::shouldFillBackground() const 
{
    return isSelected();
}

/*
 * Gets the text justification of the button title.
 */
juce::Justification AppMenu::Paged::MenuButton::getTextJustification() const 
{
    return juce::Justification::centred;
}
