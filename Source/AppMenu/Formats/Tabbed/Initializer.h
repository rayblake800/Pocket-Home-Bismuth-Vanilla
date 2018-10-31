/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/Control/Initializer.h"

/**
 * @brief  Initializes a tabbed AppMenu's objects.
 */
struct AppMenu::Tabbed::Initializer : public AppMenu::Initializer
{
    /**
     * @brief  Gets the format this object initializes.
     *
     * @return  Format::Tabbed
     */
    virtual AppMenu::Format getMenuFormat() const final override;

    /**
     * @brief  Creates a new tabbed menu component.
     *
     * @return   A new AppMenu::Tabbed::MenuComponent.
     */
    virtual AppMenu::MenuComponent* createMenuComponent() const final override;

    /**
     * @brief  Creates a new tabbed menu input handler.
     *
     * @param menuComponent  A tabbed menu component created with
     *                       createMenuComponent().
     *
     * @param controller     The menu's AppMenu::Controller object.
     *
     * @return               A new AppMenu::Tabbed::InputHandler.
     */
    virtual AppMenu::InputHandler* createInputHandler(
            AppMenu::MenuComponent* const menuComponent,
            AppMenu::Controller* const controller) const final override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
