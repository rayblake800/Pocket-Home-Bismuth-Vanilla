#define APPMENU_IMPLEMENTATION
#include "AppMenu_Scrolling_MenuButton.h"

/* Padding to leave around the title bounds, as a fraction of button height: */
static const constexpr float titlePaddingFraction = 0.05;

/*
 * Creates a new MenuButton component for a menu item.
 */
 AppMenu::Scrolling::MenuButton::MenuButton(MenuItem menuItem) :
     AppMenu::MenuButton(menuItem) { } 
     
/*
 * Finds the area relative to the menu button's bounds where the title should be
 * drawn.
 */
juce::Rectangle<float> AppMenu::Scrolling::MenuButton::findTitleBounds() const 
{
    const int paddingSize = titlePaddingFraction * getHeight();
    const int xPos = getHeight() + paddingSize;
    const int width = getWidth() - xPos - paddingSize;
    return juce::Rectangle<float>(xPos, paddingSize, width,
            getHeight() - 2 * paddingSize);
}

/*
 * Finds the area relative to the menu button's bounds where the icon should be 
 * drawn.
 */
juce::Rectangle<float> AppMenu::Scrolling::MenuButton::findIconBounds() const 
{
    return juce::Rectangle<float>(0, 0, getHeight(), getHeight());
}

/*
 * Checks if this button will draw an outline around its border.
 */
bool AppMenu::Scrolling::MenuButton::shouldDrawBorder() const 
{
    return true;
}

/*
 * Checks if this button will fill in its background with its background color.
 */
bool AppMenu::Scrolling::MenuButton::shouldFillBackground() const 
{
    return true;
}

/*
 * Gets the text justification of the button title.
 */
juce::Justification AppMenu::Scrolling::MenuButton::getTextJustification() const 
{
    return juce::Justification::centredLeft;
}
