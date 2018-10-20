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
     * @brief  Creates the MenuComponent as whatever MenuComponent subclass
     *         is appropriate for the current AppMenu format.
     *
     * @return   The new MenuComponent object.
     */
    virtual ControlledMenu* createMenuComponent() override;

    /**
     * @brief  Gets the menu controller's AppMenu format.
     *
     * @return  The controller's menu format. 
     */
    virtual AppMenu::Format getMenuFormat() const override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
