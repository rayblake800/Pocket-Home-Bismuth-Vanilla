#include "MenuItemData.h"

/*
 * Creates a menu data object for an item in the application menu.
 */
MenuItemData::MenuItemData
(const int index, const juce::Array<int> folderIndex) :
index(index),
folderIndex(folderIndex) { }


/*
 * Accesses the menu data lock.
 */
const juce::ReadWriteLock& MenuItemData::getLock()
{
    return dataLock;
}

/*
 * Gets the menu item's index within its menu folder.
 */
int MenuItemData::getIndex() const
{
    return index;
}

/*
 * Gets the index of the menu folder holding this menu item.
 */
const juce::Array<int>& MenuItemData::getFolderIndex() const
{
    return folderIndex;
}
