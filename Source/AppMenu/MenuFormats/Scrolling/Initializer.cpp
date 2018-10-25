#define APPMENU_IMPLEMENTATION_ONLY

#include "AppMenu/MenuFormats/Scrolling/Initializer.h"
#include "AppMenu/MenuFormats/Scrolling/MenuComponent.h"
#include "AppMenu/MenuFormats/Scrolling/InputHandler.h"

/*
 * Gets the format this object initializes.
 */
AppMenu::Format AppMenu::Scrolling::Initializer::getMenuFormat() const 
{
    return Format::Scrolling;
}

/*
 * Creates a new scrolling menu component.
 */
AppMenu::MenuComponent* 
AppMenu::Scrolling::Initializer::createMenuComponent() const 
{
    return new Scrolling::MenuComponent();
}

/*
 * Creates a new scrolling menu input handler.
 */
AppMenu::InputHandler* AppMenu::Scrolling::Initializer::createInputHandler(
        AppMenu::MenuComponent* const menuComponent,
        AppMenu::Controller* const controller) const 
{
    return new Scrolling::InputHandler();
}
