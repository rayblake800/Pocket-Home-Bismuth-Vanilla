/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/Control/InputHandler.h"

/**
 * @file  InputHandler.h
 *
 * @brief  Handles input events for a scrolling AppMenu.
 */
class AppMenu::Scrolling::InputHandler : public AppMenu::InputHandler
{
public:
    /**
     * @brief  Initializes the InputHandler, setting it to handle the menu
     *         component's input events.
     *
     * @param menuComponent  The menu component tracked by the InputHandler.
     *
     * @param controller     The controller used to control menu behavior.
     */
    InputHandler(AppMenu::MenuComponent* menuComponent,
            AppMenu::Controller* controller);

    virtual ~InputHandler() { }

private:
    virtual bool keyPress(const AppMenu::InputHandler::KeyType keyType)
        final override;

    virtual void menuItemClicked(const AppMenu::MenuButton* clickedButton,
            const bool rightClicked) final override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
