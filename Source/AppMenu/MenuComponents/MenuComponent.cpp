#include "Source/AppMenu/MenuComponents/MenuComponent.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "Controller.h"
#include "AppMenu.h"

/*
 * Creates the MenuComponent, linking it with its controller.
 AppMenu::MenuComponent::*/MenuComponent(Controller& controller)
{
} AppMenu::MenuComponent::~MenuComponent() { }

/*
 * Gets the MenuItem for the current active folder component.
 */MenuItem AppMenu::MenuComponent::getActiveFolder()
{
}

/*
 * Creates a new folder component.
 AppMenu::MenuComponent::Folder::*/Folder(MenuComponent& menuComponent, MenuItem folderItem)
{
} AppMenu::MenuComponent::Folder::~Folder() { }

/*
 * Gets the MenuItem that defines this folder.
 */MenuItem AppMenu::MenuComponent::Folder::getFolderItem() const
{
}/*
 * Signals to this Folder's MenuComponent that a menu item was
 *         clicked.
 */void AppMenu::MenuComponent::Folder::signalItemClicked(MenuItem clickedItem, const bool rightClicked)
{
}

/*
 * Signals to this Folder's MenuComponent that the folder was
 *         clicked.
 */void AppMenu::MenuComponent::Folder::signalFolderClicked
(const bool rightClicked, const int closestIndex)
{
}

/*
 * Gets the number of open folder components.
 */int AppMenu::MenuComponent::openFolderCount() const
{
}

/*
 * Gets an open folder component from the list of open folders.
 */Folder* AppMenu::MenuComponent::getOpenFolder(const int folderIndex) const
{
}/*
 * Creates, shows, and activates a new folder component.
 */void AppMenu::MenuComponent::openFolder(MenuItem folderItem)
{
}

/*
 * Closes the current active folder as long as more than one
 *         folder is open. 
 */
void AppMenu::MenuComponent::closeActiveFolder()
{
}

/*
 * Handles menu navigation key events not handled by the folder
 *         component.
 */bool AppMenu::MenuComponent::keyPressed(const juce::KeyPress& key)
{
} 

/*
 * Updates the menu layout when the component is resized.
 */
void AppMenu::MenuComponent::resized() final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
