#include "MenuItemData.h"

/*
 * Gets this menu item's parent folder.
 */
MenuItemData::Ptr MenuItemData::getParentFolder() const
{
    return parent;
}

/*
 * Gets this menu item's index within its parent folder.
 */
int MenuItemData::getIndex() const
{
    return index;
}

/*
 * Checks if this menu item represents a folder within the menu.
 */
bool MenuItemData::isFolder() const
{
    return !getCommand.isEmpty();
}

/*
 * Gets the number of folder items held by this menu item.
 */
int MenuItemData::getFolderSize() const
{
    return children.size();
}

/*
 * Gets a menu item contained in a folder menu item.
 */
MenuItemData::Ptr MenuItemData::getChild(const int index) const
{
    if(index < 0 || index >= children.size())
    {
        return nullptr;
    }
    return children[index];
}

/*
 * Gets all menu items contained in a folder menu item.
 */
juce::Array<MenuItemData::Ptr> MenuItemData::getChildren() const
{
    return children;
}

/*
 * Attempts to insert a new menu item into this folder menu item's array of 
 * child menu items, saving the change to this folder item's data source.
 */
bool MenuItemData::insertChild
(const MenuItemData::Ptr newChild, const int index)
{
    if(index < 0 || index > getMovableChildCount())
    {
        return false;
    }
    children.insert(index, newChild);
    newChild->parent = this;
    newChild->index = index;
    for(int i = index + 1; i < children.size(); i++)
    {
        children[i]->index++;
    }
    newChild->saveChanges();
    return true;
}

/*
 * Attempts to replace a menu item in this folder menu item's array of child 
 * menu items, saving the change to this folder item's data source.
 */
bool MenuItemData::replaceChild
(const MenuItemData::Ptr newChild, const int index)
{
    if(index < 0 || index > getMovableChildCount())
    {
        return false;
    }
    children[index]->deleteFromSource();
    newChild->parent = this;
    newChild->index = index;
    newChild->saveChanges();
    children.set(index, newChild);
    return true;
}

/*
 * Removes a menu item from this folder, deleting it from its data source.
 */
bool MenuItemData::removeChild(const int index)
{
    if(index < 0 || index >= children.size())
    {
        return false;
    }
    children[index]->deleteFromSource();
    children.remove(index);
    return true;
}

/*
 * Swaps the positions of two menu items, saving the change to this menu item's
 * data source.
 */
bool MenuItemData::swapChildren(const int childIdx1, const int childIdx2)
{
    if(index < 0 || index > getMovableChildCount())
    {
        return false;
    }
    chilren[childIdx1]->index = childIdx2;
    chilren[childIdx2]->index = childIdx1;
    children.swap(childIdx1, childIdx2);
    return true;
}
