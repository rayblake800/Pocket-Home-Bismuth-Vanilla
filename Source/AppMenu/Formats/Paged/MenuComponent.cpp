#if 0
#include "./Source/AppMenu/MenuFormats/Paged/MenuComponent.h"

#include "AppMenu/MenuComponents/MenuComponent.h"
 AppMenu::Paged::MenuComponent::~MenuComponent() { }/*
 * Arranges folders from top to bottom, with the active folder
 *         filling the menu's bounds.
 */
void AppMenu::Paged::MenuComponent::updateMenuLayout() final
{
}/*
 * Creates a new Paged::FolderComponent for an opened folder
 *         menu item.
 */AppMenu::FolderComponent* AppMenu::Paged::MenuComponent::createFolderComponent(MenuItem folderItem)
const final
{
}

/*
 * Updates the menu's bounds whenever its parent component is
 *         resized.
 */void AppMenu::Paged::MenuComponent::parentResized(const juce::Rectangle<int> parentBounds)
final
{
}

/* Only include this file directly in the AppMenu implementation! */
#endif
