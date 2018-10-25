#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/MenuFormats/Scrolling/FolderComponent.h"
#include "AppMenu/MenuFormats/Scrolling/InputHandler.h"

/*
 * Initializes the InputHandler, setting it to handle the menu component's input
 * events.
 */
AppMenu::Scrolling::InputHandler::InputHandler
(AppMenu::MenuComponent* menuComponent, AppMenu::Controller* controller) :
    AppMenu::InputHandler(menuComponent, controller) { } 

/*
 * Handles a key event received from the menu.
 */
bool AppMenu::Scrolling::InputHandler::keyPress
(const AppMenu::InputHandler::KeyType keyType)
{
}

/*
 * Handles clicks to menu item buttons.
 * The scrolling menu handles menu button click events exactly like the
 * standard InputHandler, except that only selected buttons are activated
 * when they are clicked.
 */
void AppMenu::Scrolling::InputHandler::menuItemClicked
(const AppMenu::MenuButton* clickedButton, const bool rightClicked) 
{
    if(!clickedButton->isSelected())
    {
        FolderComponent* parentFolder 
            = static_cast<FolderComponent*>
            (clickedButton->getParentComponent());
        parentFolder->setSelectedIndex(clickedButton->getMenuItem().getIndex());
        getMenuComponent()->updateMenuLayout();
    }
    else
    {
        if(rightClicked)
        {
            getController()->showContextMenu(clickedButton->getMenuItem());
        }
        else
        {
            getController()->activateMenuItem(clickedButton->getMenuItem());
        }
    }
}
