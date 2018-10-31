#pragma once
#include "AppMenu/Components/MenuComponent.h"
#include "AppMenu/Control/Controller.h"
#include "AppMenu/Control/InputHandler.h"
#include "AppMenu/AppMenu.h"

struct AppMenu::Initializer
{
    virtual Format getMenuFormat() const = 0;

    virtual MenuComponent* createMenuComponent() const = 0;

    virtual InputHandler* createInputHandler(
            MenuComponent* const menuComponent,
            Controller* const controller) const = 0;
};

