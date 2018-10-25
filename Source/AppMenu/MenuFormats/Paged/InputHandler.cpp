#include "./Source/AppMenu/MenuFormats/Paged/InputHandler.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/Control/InputHandler.h"

/*
 * Initializes the InputHandler, setting it to handle the menu
 *         component's input events.
 AppMenu::Paged::InputHandler::*/InputHandler(AppMenu::MenuComponent* menuComponent,
AppMenu::Controller* controller)
{
} AppMenu::Paged::InputHandler::~InputHandler() { }/*
 * Handles a key event received from the menu.
 */bool AppMenu::Paged::InputHandler::keyPress(const AppMenu::InputHandler::KeyType keyType)
final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
