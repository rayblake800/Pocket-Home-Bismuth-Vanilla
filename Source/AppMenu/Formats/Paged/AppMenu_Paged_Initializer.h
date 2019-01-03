#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_Paged_Initializer.h
 *
 * @brief  Initializes a paged AppMenu's objects.
 */

#include "AppMenu_Initializer.h"

namespace AppMenu{ namespace Paged{ struct Initializer; } }

struct AppMenu::Paged::Initializer : public AppMenu::Initializer
{
    /**
     * @brief  Gets the format this object initializes.
     *
     * @return  Format::Paged
     */
    virtual AppMenu::Format getMenuFormat() const final override;

    /**
     * @brief  Creates a new paged menu component.
     *
     * @return   A new AppMenu::Paged::MenuComponent.
     */
    virtual AppMenu::MenuComponent* createMenuComponent() const final override;

    /**
     * @brief  Creates a new paged menu input handler.
     *
     * @param menuComponent  A paged menu component created with
     *                       createMenuComponent().
     *
     * @param controller     The menu's AppMenu::Controller object.
     *
     * @return               A new AppMenu::Paged::InputHandler.
     */
    virtual AppMenu::InputHandler* createInputHandler(
            AppMenu::MenuComponent* const menuComponent,
            AppMenu::Controller* const controller) const final override;
};
