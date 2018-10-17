#include "ItemData.h"

/*
 * Gets this menu item's parent folder.
 */
AppMenu::ItemData::Ptr 
AppMenu::ItemData::getParentFolder() const
{
    return parent;
}

/*
 * Gets this menu item's index within its parent folder.
 */
int AppMenu::ItemData::getIndex() const
{
    return index;
}

/*
 * Checks if this menu item represents a folder within the menu.
 */
bool AppMenu::ItemData::isFolder() const
{
    return getCommand().isEmpty();
}

/*
 * Gets the number of folder items held by this menu item.
 */
int AppMenu::ItemData::getFolderSize() const
{
    return children.size();
}

/*
 * Gets a menu item contained in a folder menu item.
 */
AppMenu::ItemData::Ptr 
AppMenu::ItemData::getChild(const int childIndex) const
{
    if(childIndex < 0 || childIndex >= children.size())
    {
        return nullptr;
    }
    return children[childIndex];
}

/*
 * Gets all menu items contained in a folder menu item.
 */
juce::Array<AppMenu::ItemData::Ptr> 
AppMenu::ItemData::getChildren() const
{
    return children;
}

/*
 * Attempts to insert a new menu item into this folder menu item's array of 
 * child menu items, saving the change to this folder item's data source.
 */
bool AppMenu::ItemData::insertChild
(const AppMenu::ItemData::Ptr newChild, const int childIndex)
{
    if(childIndex < 0 || childIndex > getMovableChildCount())
    {
        return false;
    }
    children.insert(childIndex, newChild);
    newChild->parent = this;
    newChild->index = childIndex;
    for(int i = childIndex + 1; i < children.size(); i++)
    {
        children[i]->index++;
    }
    newChild->saveChanges();
    return true;
}

/*
 * Attempts to replace this menu item in its parent folder, saving the change
 * to the menu item's data source.
 */
bool AppMenu::ItemData::replace (const AppMenu::ItemData::Ptr replacement)
{
    if(parent == nullptr || index >= parent->getMovableChildCount()
            || index < 0)
    {
        return false;
    }
    replacement->parent = parent;
    replacement->index = index;
    parent->children.set(index, replacement);
    deleteFromSource();
    parent->saveChanges();
    parent = nullptr;
    index = -1;
    return true;
}

/*
 * Removes this menu item from its folder, deleting it from its data source.
 */
bool AppMenu::ItemData::remove()
{
    if(parent == nullptr || index < 0)
    {
        return false;
    }
    parent->children.remove(index);
    for(int i = index; i < parent->children.size(); i++)
    {
        parent->children[i]->index++;
    }
    parent = nullptr;
    index = -1;
    deleteFromSource();
    return true;
}

/*
 * Swaps the positions of two menu items, saving the change to this menu item's
 * data source.
 */
bool AppMenu::ItemData::swapChildren(const int childIdx1, const int childIdx2)
{
    if(index < 0 || index > getMovableChildCount())
    {
        return false;
    }
    children.swap(childIdx1, childIdx2);
    children[childIdx1]->index = childIdx1;
    children[childIdx2]->index = childIdx2;
    return true;
}
