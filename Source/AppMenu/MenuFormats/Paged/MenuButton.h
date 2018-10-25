/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/MenuComponents/MenuButton.h"

/**
 * @file  Paged/MenuButton.h
 *
 * @brief  Creates a MenuButton suitable for a paged AppMenu.
 */
class AppMenu::Paged::MenuButton : public AppMenu::MenuButton
{
public:
    /**
     * @brief  Creates a new MenuButton component for a menu item.
     *
     * @param menuItem  The new button's menu data source.
     */
    MenuButton(MenuItem menuItem);

    virtual ~MenuButton() { }

private:
    /**
     * @brief  Finds the area relative to the menu button's bounds where the
     *         title should be drawn.
     *
     * @return  A rectangular area at the bottom of the button.
     */
    virtual juce::Rectangle<float> findTitleBounds() const final override;

    /**
     * @brief  Finds the area relative to the menu button's bounds where the
     *         icon should be drawn.
     *
     * @return  A square area above the title bounds.
     */

    virtual juce::Rectangle<float> findIconBounds() const final override;

    /**
     * @brief  Checks if this button will draw an outline around its border.
     *
     * @return  True only when the button is selected.
     */
    virtual bool shouldDrawBorder() const final override;

    /**
     * @brief  Checks if this button will fill in its background with its 
     *         background color.
     *
     * @return  True only when the button is selected.
     */
    virtual bool shouldFillBackground() const final override;
    
    /**
     * @brief  Gets the text justification of the button title.
     *
     * @return   Center justification for all menu buttons.
     */
    virtual juce::Justification getTextJustification() const final override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
