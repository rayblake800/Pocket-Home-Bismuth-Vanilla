/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/Control/Initializer.h"

/**
 * @brief  Initializes a scrolling AppMenu's objects.
 */
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

/* Only include this file directly in the AppMenu implementation! */
#endif
