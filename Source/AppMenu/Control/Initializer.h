#pragma once
#include "MenuComponents/MenuComponent.h"
#include "Control/Controller.h"
#include "Control/InputHandler.h"
#include "AppMenu.h"

struct AppMenu::Initializer
{
    virtual Format getMenuFormat() const = 0;

    virtual MenuComponent* createMenuComponent() const = 0;

    virtual InputHandler* createInputHandler(
            MenuComponent* const menuComponent,
            Controller* const controller) const = 0;
};

