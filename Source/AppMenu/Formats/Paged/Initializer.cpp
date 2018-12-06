#define APPMENU_IMPLEMENTATION
#include "AppMenu/Formats/Paged/Initializer.h"
#include "AppMenu/Formats/Paged/MenuComponent.h"
#include "AppMenu/Formats/Paged/InputHandler.h"

/*
 * Gets the format this object initializes.
 */
AppMenu::Format AppMenu::Paged::Initializer::getMenuFormat() const 
{
    return Format::Paged;
}

/*
 * Creates a new paged menu component.
 */
AppMenu::MenuComponent* AppMenu::Paged::Initializer::createMenuComponent() const 
{
    return new AppMenu::Paged::MenuComponent();
}

/*
 * Creates a new paged menu input handler.
 */
AppMenu::InputHandler* AppMenu::Paged::Initializer::createInputHandler(
    AppMenu::MenuComponent* const menuComponent,
    AppMenu::Controller* const controller) const 
{
    return new AppMenu::Paged::InputHandler(menuComponent, controller);
}
