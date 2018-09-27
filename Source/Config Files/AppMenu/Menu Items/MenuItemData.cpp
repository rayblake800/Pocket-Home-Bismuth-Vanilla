#include "MenuItemData.h"

/*
 * Creates a menu data object for an item in the application menu.
 */
MenuItemData::MenuItemData(const int index, const MenuItemData* parent) :
index(index),
parent((parent == nullptr) ? nullptr : parent->clone()) { }

/*
 * Gets the data object of the folder menu item that defines this object.
 */
const MenuItemData* MenuItemData::getParent() const
{
    return parent;
}

/*
 *Gets the menu item's index within its menu folder.
 *
 * @return  The menu item's index.
 */
int MenuItemData::getIndex() const
{
    return index;
}
