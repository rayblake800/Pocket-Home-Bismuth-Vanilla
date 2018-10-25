#pragma once
#include "Control/Initializer.h"

struct AppMenu::Paged::Initializer : public AppMenu::Initializer
{
    virtual AppMenu::Format getMenuFormat() const final override;

    virtual AppMenu::MenuComponent* createMenuComponent() const final override;

    virtual AppMenu::InputHandler* createInputHandler(
            AppMenu::MenuComponent* const menuComponent,
            AppMenu::Controller* const controller) const final override;
};
