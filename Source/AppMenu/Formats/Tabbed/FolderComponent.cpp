#include "./Source/AppMenu/MenuFormats/Tabbed/FolderComponent.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/MenuComponents/FolderComponent.h"

/*
 * Creates a new tabbed folder component.
 AppMenu::Tabbed::FolderComponent::*/FolderComponent(MenuItem folderItem)
{
} AppMenu::Tabbed::FolderComponent::~FolderComponent() { }

/*
 * Finds the closest index to a spot on the folder that was clicked.
 */int AppMenu::Tabbed::FolderComponent::closestIndex(const int xPos, const int yPos) 
const final
{
}

/*
 * Creates a button component for one of the folder's child menu
 *         items.
 */AppMenu::MenuButton* AppMenu::Tabbed::FolderComponent::createMenuButton(const MenuItem folderItem)
const final
{
}

/*
 * Updates the sizes and positions of all buttons in the menu.
 */
void AppMenu::Tabbed::FolderComponent::updateButtonLayout() const final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
