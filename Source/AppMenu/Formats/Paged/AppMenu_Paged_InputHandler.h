#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_Paged_InputHandler.h
 *
 * @brief  Handles input events for a paged AppMenu.
 */

#include "AppMenu_InputHandler.h"

namespace AppMenu { namespace Paged { class InputHandler; } }
namespace AppMenu { namespace Paged { class MenuComponent; } }
namespace AppMenu { namespace Paged { class FolderComponent; } }

/*/
 *  The Paged::InputHandler's primary task is handling key events, determining
 * what index to select next based on the active folder page, the dimensions of
 * the folder page grid, and the position of the current selected menu item.  
 * It also handles button events from the Paged::MenuComponent's navigation
 * buttons, changing the visible folder page when the left and right navigation
 * buttons are clicked, and closing the last opened FolderComponent when the up
 * navigation button is clicked.
 */
class AppMenu::Paged::InputHandler : public AppMenu::InputHandler,
    public juce::Button::Listener
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
    /**
     * @brief  Handles a key event received from the menu.
     *
     * @param keyType  The type of key event that was detected.
     *
     * @return         True to indicate that the key event was handled, false
     *                 to pass it on to other active juce::KeyListener objects.
     */
    virtual bool keyPressed(const AppMenu::InputHandler::KeyType keyType)
        final override;
    
    /**
     * @brief  Handles button click events from the folder navigation buttons.
     *
     * @param button  A NavButton held by the Paged MenuComponent.
     */
    virtual void buttonClicked(juce::Button* button) override;

    /**
     * @brief  Gets the AppMenu::Paged::MenuComponent used by the AppMenu.
     *
     * @return  The AppMenu's menu component.
     */
    Paged::MenuComponent* getPagedMenuComponent();

    /**
     * @brief  Gets the active Paged FolderComponent used by the AppMenu.
     *
     * @return   The last opened folder component.
     */
    Paged::FolderComponent* getActivePagedFolderComponent();
};
