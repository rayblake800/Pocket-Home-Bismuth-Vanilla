#ifndef APPMENU_IMPLEMENTATION
  #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_Scrolling_MenuButton.h
 *
 * @brief  Creates a MenuButton suitable for a scrolling AppMenu.
 */

#include "AppMenu_MenuButton.h"

namespace AppMenu { namespace Scrolling { class MenuButton; } }

/**
 *  The Scrolling::MenuButton displays the icon on the left side of the button,
 * with the title printed directly to the right of the icon.  The button's
 * background and outline are always drawn.
 */
class AppMenu::Scrolling::MenuButton : public AppMenu::MenuButton
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
     * @return  The rectangular area to the right of the button's icon.
     */
    virtual juce::Rectangle<float> findTitleBounds() const final override;

    /**
     * @brief  Finds the area relative to the menu button's bounds where the
     *         icon should be drawn.
     *
     * @return  A square area on the left side of the button.
     */
    virtual juce::Rectangle<float> findIconBounds() const final override;

    /**
     * @brief  Checks if this button will draw an outline around its border.
     *
     * @return  True, as the scrolling menu always draws button borders.
     */
    virtual bool shouldDrawBorder() const final override;

    /**
     * @brief  Checks if this button will fill in its background with its 
     *         background color.
     *
     * @return  True, as the scrolling menu alwas draws button backgrounds.
     */
    virtual bool shouldFillBackground() const final override;
    
    /**
     * @brief  Gets the text justification of the button title.
     *
     * @return   Left justification for all menu buttons.
     */
    virtual juce::Justification getTextJustification() const final override;
};
