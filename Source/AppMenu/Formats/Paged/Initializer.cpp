#if 0
#include "./Source/AppMenu/MenuFormats/Paged/Initializer.h"
#pragma once
#include "AppMenu/Control/Initializer.h"

/*
 * Gets the format this object initializes.
 */AppMenu::Format AppMenu::Paged::Initializer::getMenuFormat() const final
{
}

/*
 * Creates a new paged menu component.
 */AppMenu::MenuComponent* AppMenu::Paged::Initializer::createMenuComponent() const final
{
}

/*
 * Creates a new paged menu input handler.
 */AppMenu::InputHandler* AppMenu::Paged::Initializer::createInputHandler(
AppMenu::MenuComponent* const menuComponent,
AppMenu::Controller* const controller) const final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
