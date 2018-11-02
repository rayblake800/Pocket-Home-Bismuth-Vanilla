#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Formats/Paged/InputHandler.h"

/*
 * Initializes the InputHandler, setting it to handle the menu component's input
 * events.
 */
AppMenu::Paged::InputHandler::InputHandler
(AppMenu::MenuComponent* menuComponent, AppMenu::Controller* controller) :
AppMenu::InputHandler(menuComponent, controller) { }

/*
 * Handles a key event received from the menu.
 */
bool AppMenu::Paged::InputHandler::keyPressed
(const AppMenu::InputHandler::KeyType keyType)
{
    // implement after initial menu layout testing
    return false;
}
