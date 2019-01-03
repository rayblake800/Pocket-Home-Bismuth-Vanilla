#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_Scrolling_Initializer.h
 *
 * @brief  Initializes a scrolling AppMenu's objects.
 */

#include "AppMenu_Initializer.h"

namespace AppMenu { namespace Scrolling { struct Initializer; } }

struct AppMenu::Scrolling::Initializer : public AppMenu::Initializer
{
    /**
     * @brief  Gets the format this object initializes.
     *
     * @return  Format::Scrolling
     */
    virtual AppMenu::Format getMenuFormat() const final override;

    /**
     * @brief  Creates a new scrolling menu component.
     *
     * @return   A new AppMenu::Scrolling::MenuComponent.
     */
    virtual AppMenu::MenuComponent* createMenuComponent() const final override;

    /**
     * @brief  Creates a new scrolling menu input handler.
     *
     * @param menuComponent  A scrolling menu component created with
     *                       createMenuComponent().
     *
     * @param controller     The menu's AppMenu::Controller object.
     *
     * @return               A new AppMenu::Scrolling::InputHandler.
     */
    virtual AppMenu::InputHandler* createInputHandler(
            AppMenu::MenuComponent* const menuComponent,
            AppMenu::Controller* const controller) const final override;
};
