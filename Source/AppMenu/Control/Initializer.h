#pragma once
#include "AppMenu/Format.h"

/**
 * @file  AppMenu/Control/Initializer.h
 *
 * @brief  Provides methods needed for initializing an AppMenu with a specific
 *         menu format.
 *
 * Each AppMenu format must provide its own implementation of 
 * AppMenu::Initializer.  Each format's initializer will need to be added to the
 * switch statement within the createInitializer function in 
 * AppMenu/AppMenu.cpp.
 */
namespace AppMenu { struct Initializer; }
namespace AppMenu { class MenuComponent; }
namespace AppMenu { class Controller; }
namespace AppMenu { class InputHandler; }
struct AppMenu::Initializer
{
    /**
     * @brief  Gets the menu format initialized by this object.
     *
     * @return  The AppMenu::Format that identifies this AppMenu implementation.
     */
    virtual Format getMenuFormat() const = 0;

    /**
     * @brief  Creates a new MenuComponent object appropriate for this menu 
     *         format.
     *
     * @return  An instance of the MenuComponent subclass defined by the format.
     */
    virtual MenuComponent* createMenuComponent() const = 0;

    /**
     * @brief  Creates a new InputHandler object appropriate for this menu
     *         format.
     *
     * @param menuComponent  A MenuComponent previously created with
     *                       createMenuComponent().
     *
     * @param controller     A menu controller connected to the menu component.
     *
     * @return               An instance of the InputHandler subclass defined by
     *                       the format.
     */
    virtual InputHandler* createInputHandler(
            MenuComponent* const menuComponent,
            Controller* const controller) const = 0;
};

