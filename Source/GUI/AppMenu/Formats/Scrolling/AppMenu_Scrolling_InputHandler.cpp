#define APPMENU_IMPLEMENTATION
#include "AppMenu_Scrolling_InputHandler.h"
#include "AppMenu_Scrolling_FolderComponent.h"
#include "AppMenu_Scrolling_MenuComponent.h"
#include "Layout_Transition_Animator.h"

// Initializes the InputHandler, setting it to handle the menu component's
// input events.
AppMenu::Scrolling::InputHandler::InputHandler
(AppMenu::MenuComponent* menuComponent, AppMenu::Controller* controller) :
    AppMenu::InputHandler(menuComponent, controller) { }


// Handles a key event received from the menu.
bool AppMenu::Scrolling::InputHandler::keyPressed
(const AppMenu::InputHandler::KeyType keyType)
{
    MenuComponent* scrollingMenu = dynamic_cast<MenuComponent*>
        (getMenuComponent());

    const int openFolderCount = scrollingMenu->openFolderCount();
    // There should always be an open folder component
    jassert(openFolderCount > 0);

    FolderComponent* activeFolder = dynamic_cast<FolderComponent*>
        (getMenuComponent()->getOpenFolder(openFolderCount - 1));
    // The open folder component should always be a scrolling folder
    jassert(activeFolder != nullptr);

    // Ignore key events if the folder is animating
    if (Layout::Transition::Animator::isAnimating(activeFolder))
    {
        return true;
    }

    const int selectedIndex = activeFolder->getSelectedIndex();
    const bool firstItemSelected = (selectedIndex == 0);
    const bool lastItemSelected =
        (selectedIndex == activeFolder->getFolderSize() - 1);
    switch(keyType)
    {
        case KeyType::Up:
            if (!firstItemSelected)
            {
                activeFolder->setSelectedIndex(selectedIndex - 1);
                scrollingMenu->updateMenuLayout(false);
            }
            return true;
        case KeyType::Down:
            if (!lastItemSelected)
            {
                activeFolder->setSelectedIndex(selectedIndex + 1);
                scrollingMenu->updateMenuLayout(false);
            }
            return true;
        case KeyType::Left:
        case KeyType::Cancel:
            if (selectedIndex >= 0)
            {
                activeFolder->setSelectedIndex(-1);
                scrollingMenu->updateMenuLayout(true);
            }
            else if (openFolderCount > 1)
            {
                getMenuComponent()->closeActiveFolder();
            }
            return true;
        case KeyType::Right:
        case KeyType::Select:
            if (selectedIndex >= 0)
            {
                getController()->activateMenuItem
                    (activeFolder->getSelectedItem());
            }
            else
            {
                activeFolder->setSelectedIndex(0);
                getMenuComponent()->updateMenuLayout();
            }
            return true;
        case KeyType::Edit:
        {
            MenuItem folder= activeFolder->getFolderMenuItem();
            if (selectedIndex >= 0)
            {
                getController()->showContextMenu
                    (folder.getFolderItem(selectedIndex));
            }
            else
            {
                getController()->showContextMenu
                    (folder, folder.getMovableChildCount());
            }
            return true;
        }
        case KeyType::Tab:
            activeFolder->setSelectedIndex(lastItemSelected ?
                    0 : selectedIndex + 1);
            scrollingMenu->updateMenuLayout(lastItemSelected);
            return true;
    }
    return false;
}


// Handles clicks to menu item buttons.
void AppMenu::Scrolling::InputHandler::menuItemClicked
(const AppMenu::MenuButton* clickedButton, const bool rightClicked)
{
    if (!clickedButton->isSelected())
    {
        FolderComponent* parentFolder = static_cast<FolderComponent*> (
                clickedButton->getParentComponent());
        parentFolder->setSelectedIndex(
                clickedButton->getMenuItem().getIndex());
        getMenuComponent()->updateMenuLayout();
    }
    else
    {
        if (rightClicked)
        {
            DBG("Right-Clicked " << clickedButton->getMenuItem().getTitle());
            getController()->showContextMenu(clickedButton->getMenuItem());
        }
        else
        {
            DBG("Left-Clicked " << clickedButton->getMenuItem().getTitle());
            getController()->activateMenuItem(clickedButton->getMenuItem());
        }
    }
}
