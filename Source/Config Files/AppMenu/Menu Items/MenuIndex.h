#pragma once
#include "JuceHeader.h"

/**
 * @file  MenuIndex.h
 *
 * @brief  Holds the position of a menu item within the application menu
 *         tree.
 */
 
class MenuIndex
{
public:
    /**
     * @brief  Creates a menu index representing the root folder of the 
     *         application menu.
     */
    MenuIndex();

    MenuIndex childIndex(const int indexInFolder) const;

    MenuIndex parentIndex() const;

    int folderIndex() const;
private:
    /**
     * @brief  Stores the index of the menu item within its parent, and the
     *         indices of each of it's ancestors within their parents.
     *
     * If the menu item is at level n in the menu tree, menuIndices[n] 
     * holds its index within its parent folder.  menuIndices[n - 1] holds
     * the index of the menu item's parent folder item within that item's
     * parent, and so on, up until menuIndices[0], which holds the index of
     * the menu item's ancestor within the root folder of the menu.
     *
     * The root menu item object, that represents the root folder itself, has
     * an empty array as its index.
     */
    juce::Array<int> menuIndices;
};
