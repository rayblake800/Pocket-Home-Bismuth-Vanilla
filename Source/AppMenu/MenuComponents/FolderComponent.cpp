#define APPMENU_IMPLEMENTATION_ONLY
#include "FolderComponent.h"

/*
 * Creates a new folder component.
 */
AppMenu::FolderComponent::FolderComponent
(MenuItem folderItem, MenuComponent& menuComponent) :
MenuComponent::Folder(folderItem, menuComponent) { } 

/*
 * Gets the current selected folder item.
 */
AppMenu::MenuItem 
AppMenu::FolderComponent::getSelectedItem() const
{
    if(selectedIndex < 0 || selectedIndex > folderButtons.size())
    {
        return MenuItem();
    }
    return folderButtons[selectedIndex]->getMenuItem();
}

/*
 * Gets the index of the current menu selection.
 */
int AppMenu::FolderComponent::getSelectedIndex() const
{
    return selectedIndex;
}

/*
 * Creates a new menu item component.
 */
AppMenu::FolderComponent::ItemButton::ItemButton
(const MenuItem menuItem, FolderComponent& parentFolder) :
Button(menuItem.getTitle() + "_Button"),
menuItem(menuItem), parentFolder(parentFolder) { }

/*
 * Gets the menu item that defines this ItemButton.
 */
AppMenu::MenuItem 
AppMenu::FolderComponent::ItemButton::getMenuItem() const
{
    return menuItem;
}

/*
 * Sets the current selected menu index.
 */
void AppMenu::FolderComponent::setSelectedIndex(const int newSelection)
{
    if(newSelection >= 0 && newSelection < folderButtons.size())
    {
        selectedIndex = newSelection;
    }
}

/*
 * Gets the button component of a child menu item in this folder.
 */
AppMenu::FolderComponent::ItemButton* 
AppMenu::FolderComponent::getButtonComponent(const int index)
{
    if(index < 0 || index >= folderButtons.size())
    {
        return nullptr;
    }
    return folderButtons[index];
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
void AppMenu::FolderComponent::childAdded(const int childIndex)
{
    MenuItem folderItem = getFolderItem();
    MenuItem newChild = folderItem.getFolderItem(childIndex);
    folderButtons.insert(childIndex, createMenuButton(newChild));
    addAndMakeVisible(folderButtons[childIndex]);
    updateButtonLayout();
}

/*
 * Removes the corresponding ItemButton when a child menu item is removed.
 */
void AppMenu::FolderComponent::childRemoved(const int removedIndex)
{
    removeChildComponent(folderButtons[removedIndex]);
    folderButtons.remove(removedIndex);
    updateButtonLayout();
}

/*
 * Swaps the positions of two of the folder's ItemButtons when their 
 * corresponding MenuItems are swapped.
 */
void AppMenu::FolderComponent::childrenSwapped
(const int swapIndex1, const int swapIndex2)
{
    juce::Rectangle<int> firstBounds = folderButtons[swapIndex1]->getBounds();
    folderButtons[swapIndex1]->setBounds
        (folderButtons[swapIndex2]->getBounds());
    folderButtons[swapIndex2]->setBounds(firstBounds);
    folderButtons.swap(swapIndex1, swapIndex2);
}
