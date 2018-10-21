/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/Controller/Controller.h"

class AppMenu::Scrolling::Controller : public AppMenu::Controller
{
public:
    Controller() { }
    virtual ~Controller() { }

private:
    /**
     * @brief  Creates the ControlledMenu as a scrolling MenuComponent.
     *
     * @return   The new Scrolling::MenuComponent object.
     */
    virtual ControlledMenu* createMenuComponent() override;

    /**
     * @brief  Gets the menu controller's AppMenu format.
     *
     * @return  The scrolling menu format type.
     */
    virtual AppMenu::Format getMenuFormat() const override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
