#include "AppMenu/Formats/Tabbed/Initializer.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/Control/Initializer.h"

/*
 * Gets the format this object initializes.
 */AppMenu::Format AppMenu::Tabbed::Initializer::getMenuFormat() const final
{
}

/*
 * Creates a new tabbed menu component.
 */AppMenu::MenuComponent* AppMenu::Tabbed::Initializer::createMenuComponent() const final
{
}

/*
 * Creates a new tabbed menu input handler.
 */AppMenu::InputHandler* AppMenu::Tabbed::Initializer::createInputHandler(
AppMenu::MenuComponent* const menuComponent,
AppMenu::Controller* const controller) const final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
