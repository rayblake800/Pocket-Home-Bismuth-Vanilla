#include "MenuIndex.h"

/*
 * Gets the index of a menu item within the folder represented by this menu 
 * index.
 */
MenuIndex MenuIndex::childIndex(const int indexInFolder) const
{
    MenuIndex child;
    child.menuIndices = menuIndices;
    child.menuIndices.add(indexInFolder);
    return child;
}

/*
 * Gets the menu index of the parent folder of the menu item represented by this
 * menu item.
 */
MenuIndex MenuIndex::parentIndex() const
{
    MenuIndex parent;
    if(!menuIndices.isEmpty())
    {
        parent.menuIndices = menuIndices;
        parent.menuIndices.removeLast();
    }
    return parent;
}

/*
 * Gets the index of this menu item within its parent folder.
 */
int MenuIndex::folderIndex() const
{
    if(menuIndices.isEmpty())
    {
        return -1;
    }
    return menuIndices.getLast();
}

/*
 * Gets the index of one of this menu item's ancestor folders within its parent 
 * folder.
 */
int MenuIndex::indexAtDepth(const int depth) const
{
    if(depth < 0 || depth >= menuIndices.size())
    {
        return -1;
    }
    return menuIndices[depth];
}

/*
 * Gets the indexed menu item's depth in the menu tree.
 */
int MenuIndex::getDepth() const
{
    return menuIndices.size() - 1;
}

/*
 * Moves this index on a single level of the menu tree.
 */
void MenuIndex::moveIndex(const int depth, const int offset)
{
    if(depth >= 0 && depth < menuIndices.size())
    {
        int newIndex = offset + menuIndices[depth];
        if(newIndex >= 0)
        {
            menuIndices.set(depth, newIndex);
        }
    }
}

/*
 * Calls indexAtDepth when passed a number using square brackets.
 */
int MenuIndex::operator[](const int depth) const
{
    return indexAtDepth(depth);
}

/*
 * Checks if two menu indices mark the same location in the menu tree.
 */
bool MenuIndex::operator==(const MenuIndex& toCompare) const
{
    return menuIndices == toCompare.menuIndices;
}

/*
 * Compares two menu indices based on their position in the menu tree.
 */
bool MenuIndex::operator<(const MenuIndex& toCompare) const
{
    for(int i = 0; i < menuIndices.size(); i++)
    {
        if(i >= toCompare.menuIndices.size())
        {
            return true;
        }
        if(menuIndices[i] == toCompare.menuIndices[i])
        {
            continue;
        }
        return menuIndices[i] < toCompare.menuIndices[i];
    }
    return false;
}

#ifdef JUCE_DEBUG
    /*
     * Returns a string representation of the menu index for debug purposes.
     */
    juce::String MenuIndex::toString() const
    {
        if(menuIndices.isEmpty())
        {
            return "[Root folder Item]";
        }
        juce::String debugStr = "[";
        for(int i = 0; i < menuIndices.size(); i++)
        {
            debugStr += juce::String(menuIndices[i]);
            debugStr += ((i == (menuIndices.size() - 1)) ? "]" : ",");
        }
        return debugStr;
    }
#endif
