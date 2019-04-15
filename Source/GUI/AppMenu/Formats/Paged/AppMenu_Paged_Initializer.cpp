#define APPMENU_IMPLEMENTATION
#include "AppMenu_Paged_Initializer.h"
#include "AppMenu_Paged_MenuComponent.h"
#include "AppMenu_Paged_InputHandler.h"

// Gets the format this object initializes.
AppMenu::Format AppMenu::Paged::Initializer::getMenuFormat() const
{
    return Format::Paged;
}


// Creates a new paged menu component.
AppMenu::MenuComponent* AppMenu::Paged::Initializer::createMenuComponent()
        const
{
    return new AppMenu::Paged::MenuComponent();
}


// Creates a new paged menu input handler.
AppMenu::InputHandler* AppMenu::Paged::Initializer::createInputHandler(
    AppMenu::MenuComponent* const menuComponent,
    AppMenu::Controller* const controller) const
{
    return new AppMenu::Paged::InputHandler(menuComponent, controller);
}
