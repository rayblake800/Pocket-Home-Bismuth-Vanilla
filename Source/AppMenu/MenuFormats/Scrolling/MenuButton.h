/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/AbstractComponents/MenuButton.h"

class AppMenu::Scrolling::MenuButton : public AppMenu::MenuButton
{
public:
    MenuButton() { }
    virtual ~MenuButton() { }
};

/* Only include this file directly in the AppMenu implementation! */
#endif
