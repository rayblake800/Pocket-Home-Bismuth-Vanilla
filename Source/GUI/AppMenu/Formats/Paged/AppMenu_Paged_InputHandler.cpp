#define APPMENU_IMPLEMENTATION
#include "AppMenu_Paged_InputHandler.h"
#include "AppMenu_Paged_MenuComponent.h"
#include "AppMenu_Paged_FolderComponent.h"
#include "Layout_Transition_Animator.h"
#include "Widgets_NavButton.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "AppMenu::Paged::InputHandler::";
#endif

// Initializes the InputHandler, setting it to handle the menu component's
// input events.
AppMenu::Paged::InputHandler::InputHandler
(AppMenu::MenuComponent* menuComponent, AppMenu::Controller* controller) :
AppMenu::InputHandler(menuComponent, controller)
{
    MenuComponent* menu = getPagedMenuComponent();
    menu->addNavButtonListener(this);
}


// Handles a key event received from the menu.
bool AppMenu::Paged::InputHandler::keyPressed
(const AppMenu::InputHandler::KeyType keyType)
{
    FolderComponent* activeFolder = getActivePagedFolderComponent();

    // Suppress key events while animating the menu.
    if (Layout::Transition::Animator::isAnimating(activeFolder))
    {
        return true;
    }
    const int folderPageIndex     = activeFolder->getCurrentFolderPage();
    const int selectedIndex       = activeFolder->getSelectedIndex();
    if (keyType == KeyType::Edit)
    {
        if (selectedIndex >= 0)
        {
            getController()->showContextMenu(activeFolder->getSelectedItem());
        }
        else
        {
            const MenuItem folderItem = activeFolder->getFolderMenuItem();
            const int firstPageIndex = std::min(
                    activeFolder->positionIndex(folderPageIndex, 0, 0),
                    folderItem.getMovableChildCount());
            getController()->showContextMenu(folderItem, firstPageIndex);
        }
        return true;
    }
    if (keyType == KeyType::Select)
    {
        if (selectedIndex >= 0)
        {
            getController()->activateMenuItem
                (activeFolder->getSelectedItem());
        }
        return true;
    }
    if (keyType == KeyType::Cancel)
    {
        if (selectedIndex >= 0)
        {
            activeFolder->setSelectedIndex(-1);
        }
        else if (folderPageIndex > 0)
        {
            activeFolder->setCurrentFolderPage(0);
            getMenuComponent()->updateMenuLayout(true);
        }
        else if (getMenuComponent()->openFolderCount() > 0)
        {
            getMenuComponent()->closeActiveFolder();
        }
        return true;
    }
    if (keyType == KeyType::Tab)
    {
        return false;
    }

    ConfigFile formatConfig;
    const int maxIndex            = activeFolder->getFolderSize() - 1;
    const int selectionColumn     = activeFolder->getSelectionColumn();
    const int numColumns          = formatConfig.getPagedMenuColumns();
    const int selectionRow        = activeFolder->getSelectionRow();
    const int numRows             = formatConfig.getPagedMenuRows();

    int newRow    = selectionRow;
    int newColumn = selectionColumn;
    int newPage   = folderPageIndex;
    if (selectedIndex < 0)
    {
        if (keyType == KeyType::Right || keyType == KeyType::Down)
        {
            newRow = 0;
            newColumn = 0;
        }
        else if (keyType == KeyType::Left || keyType == KeyType::Up)
        {
            newRow = numRows - 1;
            newColumn = numColumns - 1;
        }

    }
    else if (keyType == KeyType::Up)
    {
        if (selectionRow > 0)
        {
            newRow--;
        }
    }
    else if (keyType == KeyType::Down)
    {
        if (selectionRow < (numRows - 1))
        {
            newRow++;
        }
    }
    else if (keyType == KeyType::Left)
    {
        if (selectionColumn > 0)
        {
            newColumn--;
        }
        else if (folderPageIndex > 0)
        {
            newPage--;
            newColumn = numColumns - 1;
        }
    }
    else if (keyType ==  KeyType::Right)
    {
        if (selectionColumn < (numColumns - 1))
        {
            newColumn++;
        }
        else if (folderPageIndex < (activeFolder->getNumFolderPages() - 1))
        {
            newPage++;
            newColumn = 0;
        }
    }
    const int newIndex = std::min(activeFolder->getFolderSize() - 1,
            activeFolder->positionIndex(newPage, newColumn, newRow));
    if (newIndex >= 0 && newIndex != selectedIndex)
    {
        activeFolder->setSelectedIndex(newIndex);
        if (newPage != folderPageIndex)
        {
            activeFolder->setCurrentFolderPage(newPage);
            getMenuComponent()->updateMenuLayout();
        }
    }
    return true;
}


// Handles button click events from the folder navigation buttons.
void AppMenu::Paged::InputHandler::buttonClicked(juce::Button* button)
{
    if (getController()->ignoringInput())
    {
        DBG(dbgPrefix << __func__ 
                << ": Loading or editing menu, ignoring NavButton click.");
        return;
    }
    using Widgets::NavButton;
    NavButton* navButton = static_cast<NavButton*> (button);
    int activeFolderIndex = getMenuComponent()->openFolderCount() - 1;
    switch(navButton->getEdge())
    {
        case NavButton::WindowEdge::up: // close folder
            if (activeFolderIndex > 0)
            {
                getMenuComponent()->closeActiveFolder();
            }
            break;
        case NavButton::WindowEdge::left:
        case NavButton::WindowEdge::right: // move folder page
        {
            FolderComponent* activeFolder = getActivePagedFolderComponent();
            const int numFolderPages = activeFolder->getNumFolderPages();
            const int targetFolderPage = activeFolder->getCurrentFolderPage()
                + ( (navButton->getEdge() == NavButton::WindowEdge::left)
                    ? -1 : 1);
            if (targetFolderPage >= 0 && targetFolderPage < numFolderPages)
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


// Gets the AppMenu::Paged::MenuComponent used by the AppMenu.
AppMenu::Paged::MenuComponent*
AppMenu::Paged::InputHandler::getPagedMenuComponent()
{
    return static_cast<Paged::MenuComponent*> (getMenuComponent());
}


// Gets the active Paged FolderComponent used by the AppMenu.
AppMenu::Paged::FolderComponent*
AppMenu::Paged::InputHandler::getActivePagedFolderComponent()
{
    const int activeFolderIndex = getMenuComponent()->openFolderCount() - 1;
    jassert(activeFolderIndex >= 0);
    return static_cast<Paged::FolderComponent*>
        (getMenuComponent()->getOpenFolder(activeFolderIndex));
}
