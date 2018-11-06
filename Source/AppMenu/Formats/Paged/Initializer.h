/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/Control/Initializer.h"

/**
 * @file  AppMenu/Formats/Paged/Initializer.h
 *
 * @brief  Initializes a paged AppMenu's objects.
 */
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

/* Only include this file directly in the AppMenu implementation! */
#endif
