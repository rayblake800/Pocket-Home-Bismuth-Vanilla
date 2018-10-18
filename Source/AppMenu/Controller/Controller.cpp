#define APPMENU_IMPLEMENTATION_ONLY
#include "Controller.h"
#include "MenuItem.h"

AppMenu::Controller::Controller() : contextMenu(*this) { }

/*
 * Cancels any pending callback functions.
 */
AppMenu::Controller::~Controller()
{
    DesktopEntryLoader entryLoader;
    entryLoader.clearCallback(loaderCallbackID.get());
}

/*
 * Creates a MenuComponent, linking it to a controller.
 */
 AppMenu::Controller::MenuComponent::MenuComponent(Controller& controller) :
     controller(controller) { }

/*
 * Signals to the controller that a menu item was clicked.
 */
void AppMenu::Controller::MenuComponent::signalItemClicked
(MenuItem clickedItem, const bool rightClicked)
{
    controller.menuItemClicked(clickedItem, rightClicked);
}

/*
 * Signals to the controller that an open folder was clicked.
 */
void AppMenu::Controller::MenuComponent::signalFolderClicked
(MenuItem folderItem, const bool rightClicked, const int closestIndex)
{
    controller.folderClicked(folderItem, rightClicked, closestIndex);
}

/*
 * Signals to the controller that the menu was clicked somewhere other than at a
 * folder or menu item.
 */
void AppMenu::Controller::MenuComponent::signalMenuClicked
(const bool rightClicked)
{
    menuClicked(rightClicked);
}

/*
 * Checks if user input should be enabled.
 */
bool AppMenu::Controller::MenuComponent::getInputEnabled() const
{
    return inputEnabled;
}

/*
 * Sets if the MenuComponent and all its child Components should register user 
 * input.
 */
void AppMenu::Controller::MenuComponent::setInputEnabled
(const bool allowUserInput)
{
    inputEnabled = allowUserInput;
}

/*
 * Launches or focuses an application from the menu.
 */
void AppMenu::Controller::launchOrFocusApplication(MenuItem toLaunch)
{

}

/*
 * Activates or deactivates the loading spinner component held
 *         by the Controller's MainComponent.
 */void AppMenu::Controller::setLoadingSpinnerVisible(const bool shouldShow)
{
}

/*
 * Handles mouse clicks on menu items.
 */void AppMenu::Controller::menuItemClicked(MenuItem clickedItem, const bool rightClicked)
{
}

/*
 * Handles mouse clicks on open menu folders.
 */void AppMenu::Controller::folderClicked(MenuItem clickedFolder, const bool rightClicked,
const int closestIndex)
{
}

/*
 * Handles any other clicks on the menu component.
 */void AppMenu::Controller::menuClicked(const bool rightClicked)
{
}/*
 * Scans desktop entries for updates whenever window focus is
 *         gained.
 */
void AppMenu::Controller::windowFocusGained()
{
}

/*
 * Hides the loading spinner and context menu, destroy any menu
 *         editor component, and stop waiting for applications to launch
 *         when window focus is lost.
 */
void AppMenu::Controller::windowFocusLost()
{
}

/*
 * Links the ContextMenu with the controller that holds it.
 AppMenu::Controller::ContextMenu::*/ContextMenu(Controller& controller)
{
} AppMenu::Controller::ContextMenu::~ContextMenu() { }/*
 * Inserts a new menu item into the menu, updating the menu
 *         component.
 */void AppMenu::Controller::ContextMenu::insertMenuItem(MenuItem folder, MenuItem newItem,
const int index)
{
}

/*
 * Swaps two menu items, updating the menu component.
 */void AppMenu::Controller::ContextMenu::swapMenuItems(MenuItem folder, const int swapIndex1,
const int swapIndex2)
{
}

/*
 * Removes a menu item, updating the menu component.
 */void AppMenu::Controller::ContextMenu::removeMenuItem(MenuItem toRemove)
{
}

/*
 * Shows an editor component above the MenuComponent.
 */void AppMenu::Controller::ContextMenu::showPopupEditor(PopupEditor* editor)
{
}
