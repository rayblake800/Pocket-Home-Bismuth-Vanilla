#define APPMENU_IMPLEMENTATION_ONLY
#include "NavButton.h"
#include "AppMenu/Formats/Paged/MenuComponent.h"
#include "AppMenu/Formats/Paged/FolderComponent.h"
#include "AppMenu/Formats/Paged/InputHandler.h"

/*
 * Initializes the InputHandler, setting it to handle the menu component's input
 * events.
 */
AppMenu::Paged::InputHandler::InputHandler
(AppMenu::MenuComponent* menuComponent, AppMenu::Controller* controller) :
AppMenu::InputHandler(menuComponent, controller) 
{ 
    MenuComponent* menu = static_cast<Paged::MenuComponent*>
        (getMenuComponent());
    menu->addNavButtonListener(this);
}

/*
 * Handles a key event received from the menu.
 */
bool AppMenu::Paged::InputHandler::keyPressed
(const AppMenu::InputHandler::KeyType keyType)
{
    // implement after initial menu layout testing
    return false;
}

/*
 * Handles button click events from the folder navigation buttons.
 */
void AppMenu::Paged::InputHandler::buttonClicked(juce::Button* button)
{
    NavButton* navButton = static_cast<NavButton*>(button);
    int activeFolderIndex = getMenuComponent()->openFolderCount() - 1; 
    switch(navButton->getEdge())
    {
        case NavButton::WindowEdge::up: // close folder
            if(activeFolderIndex > 0)
            {
                getMenuComponent()->closeActiveFolder();
            }
            break;
        case NavButton::WindowEdge::left: 
        case NavButton::WindowEdge::right: // move folder page 
        {
            FolderComponent* activeFolder = static_cast<Paged::FolderComponent*>
                (getMenuComponent()->getOpenFolder(activeFolderIndex));
            const int numFolderPages = activeFolder->getNumFolderPages();
            const int targetFolderPage = activeFolder->getCurrentFolderPage()
                + ((navButton->getEdge() == NavButton::WindowEdge::left) 
                  ? -1 : 1);
            if(targetFolderPage >= 0 && targetFolderPage < numFolderPages)
            {
                activeFolder->setSelectedIndex(-1);
                activeFolder->setCurrentFolderPage(targetFolderPage);
                getMenuComponent()->updateMenuLayout();
            }
            break;
        }
        case NavButton::WindowEdge::down: // this should never happen
            jassertfalse;
    }
}
