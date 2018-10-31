#include "./Source/AppMenu/MenuFormats/Tabbed/InputHandler.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/Control/InputHandler.h"

/*
 * Initializes the InputHandler, setting it to handle the menu
 *         component's input events.
 AppMenu::Tabbed::InputHandler::*/InputHandler(AppMenu::MenuComponent* menuComponent,
AppMenu::Controller* controller)
{
} AppMenu::Tabbed::InputHandler::~InputHandler() { }/*
 * Handles a key event received from the menu.
 */bool AppMenu::Tabbed::InputHandler::keyPress(const AppMenu::InputHandler::KeyType keyType)
final
{
}

/*
 * Handles clicks to menu item buttons.
 * The tabbed menu handles menu button click events exactly like the
 * standard InputHandler, except that only selected buttons are activated
 * when they are clicked.
 */void AppMenu::Tabbed::InputHandler::menuItemClicked(const AppMenu::MenuButton* clickedButton,
const bool rightClicked) final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
