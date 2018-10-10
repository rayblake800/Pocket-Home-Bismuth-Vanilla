#include "MenuItemData.h"

/*
 * Creates a menu data object for an item in the application menu.
 */
MenuItemData::MenuItemData(const MenuIndex& index) : index(index) { }

/*
 * Gets the menu item's index within the menu.
 */
const MenuIndex& MenuItemData::getIndex() const
{
    return index;
}

/*
 * Accesses the menu data lock.
 */
const juce::ReadWriteLock& MenuItemData::getLock()
{
    return dataLock;
}

/*
 * Updates the menu item data's saved index value.
 */
void MenuItemData::updateIndex(const int depth, const int offset)
{
    index.moveIndex(depth, offset);
}


