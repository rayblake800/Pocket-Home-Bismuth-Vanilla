#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Formats/Paged/FolderComponent.h"

/*
 * Creates a new paged folder component.
 */
 AppMenu::Paged::FolderComponent::FolderComponent(MenuItem folderItem) :
     AppMenu::FolderComponent(folderItem) { } 
     

/*
 * Finds the closest index to a spot on the folder that was clicked.
 */
int AppMenu::Paged::FolderComponent::closestIndex
(const int xPos, const int yPos) const 
{
}

/*
 * Creates a button component for one of the folder's child menu items.
 */
AppMenu::MenuButton* AppMenu::Paged::FolderComponent::createMenuButton
(const MenuItem folderItem) const 
{
}

/*
 * Updates the sizes and positions of all buttons in the menu.
 */
void AppMenu::Paged::FolderComponent::updateButtonLayout() const
{
}
