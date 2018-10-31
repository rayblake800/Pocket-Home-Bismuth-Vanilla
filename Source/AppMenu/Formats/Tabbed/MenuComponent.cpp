#include "AppMenu/Formats/Tabbed/MenuComponent.h"

/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/MenuComponents/MenuComponent.h"
 AppMenu::Tabbed::MenuComponent::~MenuComponent() { }/*
 * Arranges folders from left to right, with the selected item of
 *         the active folder centered.
 */
void AppMenu::Tabbed::MenuComponent::updateMenuLayout() final
{
}/*
 * Creates a new Tabbed::FolderComponent for an opened folder
 *         menu item.
 */AppMenu::FolderComponent* AppMenu::Tabbed::MenuComponent::createFolderComponent(MenuItem folderItem)
const final
{
}

/*
 * Updates the menu's bounds whenever its parent component is
 *         resized.
 */void AppMenu::Tabbed::MenuComponent::parentResized(const juce::Rectangle<int> parentBounds)
final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
