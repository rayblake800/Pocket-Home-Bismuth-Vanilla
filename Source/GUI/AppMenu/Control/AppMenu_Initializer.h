#ifndef APPMENU_IMPLEMENTATION
    #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_Initializer.h
 *
 * @brief  Provides functions needed for initializing an AppMenu with a specific
 *         menu format.
 */

#include "AppMenu_Format.h"

namespace AppMenu { struct Initializer; }
namespace AppMenu { class MenuComponent; }
namespace AppMenu { class Controller; }
namespace AppMenu { class InputHandler; }

/**
 * @brief  Creates and initializes the main AppMenu component.
 *
 *  Each AppMenu format must provide its own implementation of
 * AppMenu::Initializer. Each format's initializer will need to be added to the
 * switch statement within the createInitializer function in AppMenu.cpp.
 */
struct AppMenu::Initializer
{
    /**
     * @brief  Gets the menu format initialized by this object.
     *
     * @return  The AppMenu::Format this object initializes.
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
     * @return               An instance of the InputHandler subclass defined
     *                       by the format.
     */
    virtual InputHandler* createInputHandler(
            MenuComponent* const menuComponent,
            Controller* const controller) const = 0;
};

