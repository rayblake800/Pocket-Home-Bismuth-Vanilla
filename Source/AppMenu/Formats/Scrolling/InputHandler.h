#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  InputHandler.h
 *
 * @brief  Handles input events for a scrolling AppMenu.
 */

#include "AppMenu/Control/InputHandler.h"

namespace AppMenu { namespace Scrolling { class InputHandler; } }

/**
 *  The Scrolling::InputHandler processes key events for the Scrolling menu,
 * using them to scroll through the active folder component, close the folder,
 * or activate menu items. The InputHandler also slightly changes the behavior
 * of clicked menu items, only activating them if they are currently selected.
 */
class AppMenu::Scrolling::InputHandler : public AppMenu::InputHandler
{
public:
    /**
     * @brief  Initializes the InputHandler, setting it to handle the menu
     *         component's input events.
     *
     * @param menuComponent  The menu component tracked by the InputHandler.
     *
     * @param controller     The controller used to control menu behavior.
     */
    InputHandler(AppMenu::MenuComponent* menuComponent,
            AppMenu::Controller* controller);

    virtual ~InputHandler() { }

private:
    /**
     * @brief  Handles a key event received from the menu.
     *
     * @param keyType  The type of key event that was detected.
     *
     * @return         True to indicate that the key event was handled, false
     *                 to pass it on to other active juce::KeyListener objects.
     */
    virtual bool keyPressed(const AppMenu::InputHandler::KeyType keyType)
        final override;

    /**
     * @brief  Handles clicks to menu item buttons.
     *
     *  The scrolling menu handles menu button click events exactly like the
     * standard InputHandler, except that only selected buttons are activated
     * when they are clicked.
     *
     * @param clickedButton  The button component that was clicked.
     *
     * @param rightClicked   Whether the button was right clicked.
     */
    virtual void menuItemClicked(const AppMenu::MenuButton* clickedButton,
            const bool rightClicked) final override;
};
