#define APPMENU_IMPLEMENTATION_ONLY
#include "FolderComponent.h"

/*
 * Creates a new folder component.
 */
AppMenu::FolderComponent::FolderComponent
(MenuComponent& menuComponent, MenuItem folderItem) :
MenuComponent::Folder(menuComponent, folderItem) { } 

/*
 * Gets the MenuItem that defines this folder.
 */
AppMenu::MenuItem 
AppMenu::FolderComponent::getFolderItem() const
{
}

/*
 * Gets the current selected folder item.
 */
AppMenu::MenuItem 
AppMenu::FolderComponent::getSelectedItem() const
{
}

/*
 * Gets the index of the current menu selection.
 */
int AppMenu::FolderComponent::getSelectedIndex() const
{
}

/*
 * Creates a new menu item component.
 */
AppMenu::FolderComponent::ItemButton::ItemButton(const MenuItem menuItem) :
Button(menuItem.getTitle() + "_Button"), menuItem(menuItem) { }

/*
 * Gets the menu item that defines this ItemButton.
 */
AppMenu::MenuItem 
AppMenu::FolderComponent::ItemButton::getMenuItem() const
{
}

/*
 * Sets the current selected menu index.
 */
void AppMenu::FolderComponent::setSelectedIndex(const int newSelection)
{
}

/*
 * Gets the button component of a child menu item in this folder.
 */
AppMenu::FolderComponent::ItemButton* 
AppMenu::FolderComponent::getButtonComponent(const int index)
{
}

/*
 * Called whenever a folder menu item is clicked, allowing the folder to handle
 * the event.
 */
bool AppMenu::FolderComponent::handleMenuClick
(const MenuItem clickedItem, bool rightClicked)
{
}

/*
 * Handles all mouse events over this folder component.  This passes on all 
 * mouse events to the MenuComponent, unless the FolderComponent implementation 
 * chooses to intercept them.
 */
void AppMenu::FolderComponent::mouseDown(const juce::MouseEvent& event)
{
}

/*
 * Creates and inserts a new ItemButton when a new child menu button is created.
 */
void AppMenu::FolderComponent::childAdded
(MenuItem folderItem, const int childIndex)
{
}

/*
 * Removes the corresponding ItemButton when a child menu item is removed.
 */
void AppMenu::FolderComponent::childRemoved(const int removedIndex)
{
}

/*
 * Swaps the positions of two of the folder's ItemButtons when their 
 * corresponding MenuItems are swapped.
 */
void AppMenu::FolderComponent::childrenSwapped
(const int swapIndex1, const int swapIndex2)
{
}
