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
    MenuIndex() { }

    /**
     * @brief  Gets the index of a menu item within the folder represented by
     *         this menu index.
     *
     * This does not check the validity of the new index.  The caller is
     * responsible for ensuring that this menu item actually represents a
     * folder, and the new index points to a real menu item.
     *
     * @param indexInFolder  The index of the child menu item within its folder.
     *
     * @return  The MenuIndex representing the location of the child menu item.
     */
    MenuIndex childIndex(const int indexInFolder) const;

    /**
     * @brief  Gets the menu index of the parent folder of the menu item
     *         represented by this menu item.
     *
     * @return  The parent item's index, or a copy of this index if this is
     *          the root folder index.
     */
    MenuIndex parentIndex() const;

    /**
     * @brief  Gets the index of this menu item within its parent folder.
     *
     * @return  The index within the parent folder, or -1 if this index
     *          represents the root menu folder.
     */
    int folderIndex() const;

    /**
     * @brief  Gets the index of one of this menu item's ancestor folders 
     *         within its parent folder.
     *
     * @param depth  A depth in the menu tree, between 0 and the indexed item's
     *               depth.
     *
     * @return       The index of the menu item's ancestor at that depth within 
     *               its parent folder, or -1 if the depth given was out of 
     *               range.
     */
    int indexAtDepth(const int depth) const;

    /**
     * @brief  Gets the indexed menu item's depth in the menu tree.
     *
     * @return  The menu item's depth, where -1 represents the depth of the root
     *          folder item, 0 represents the depth of all item in the root 
     *          folder, and so on.
     */
    int getDepth() const;

    /**
     * @brief  Moves this index on a single level of the menu tree.
     * 
     * If the depth is out of bounds or the offset would move the index at
     * (depth) to less than zero, the menu index will not change.
     *
     * @param depth   The level in the menu tree of the menu item being moved.
     *
     * @param offset  The amount to move the index at (depth) relative to its
     *                current position.
     */
    void moveIndex(const int depth, const int offset);

    /**
     * @brief  Calls indexAtDepth when passed a number using square brackets.
     *
     * @param depth  A depth in the menu tree, between 0 and the indexed item's
     *               depth.
     *
     * @return       The result of calling indexAtDepth(depth).
     */
    int operator[](const int depth) const;

    /**
     * @brief  Checks if two menu indices mark the same location in the menu
     *         tree.
     *
     * @param toCompare  Another menu index to compare with this one.
     *
     * @return           Whether both indices are identical on every level of
     *                   the menu tree.
     */
    bool operator==(const MenuIndex& toCompare) const;
    
    /**
     * @brief  Compares two menu indices based on their position in the menu
     *         tree.
     *
     * @param toCompare  Another menu index to compare with this one.
     *
     * @return           Whether this menu index comes before rhs in the menu
     *                   tree.
     */
    bool operator<(const MenuIndex& toCompare) const;

#ifdef JUCE_DEBUG
    /**
     * @brief   Returns a string representation of the menu index for debug
     *          purposes.
     *
     * @return  The index, represented as a string.
     */
    juce::String toString() const;
#endif

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
