#ifndef APPMENU_IMPLEMENTATION
    #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_FolderComponent.h
 *
 * @brief  Creates and displays menu item components for all items in a single
 *         application menu folder.
 */

#include "AppMenu_MenuButton.h"
#include "AppMenu_MenuItem.h"

namespace AppMenu { class FolderComponent; }

/**
 * @brief  Creates and displays MenuButton components for each item in a menu
 *         folder.
 *
 *  The FolderComponent is assigned a folder MenuItem on construction. It is
 * responsible for creating MenuButton components for each of that MenuItem's
 * child menu items, holding those MenuButtons as child components, and setting
 * how they are arranged within its bounds. It listens for changes to its
 * MenuItem, updating its child items whenever its menu data changes. It also
 * tracks which one of its MenuButtons, if any, is currently selected by the
 * user.
 *
 *  FolderComponent is an abstract base class. Each AppMenu Format should have
 * a FolderComponent implementation that defines how that format arranges menu
 * buttons within its folders.
 */
class AppMenu::FolderComponent : public juce::Component,
    private MenuItem::Listener
{
public:
    /**
     * @brief  Creates a new folder component.
     *
     * @param folderItem     The menu data object used to construct the folder.
     */
    FolderComponent(MenuItem folderItem);

    virtual ~FolderComponent() { }

    /**
     * @brief  Creates, adds, and shows the folder's menu buttons if they have
     *         not yet been initialized.
     */
    void initMenuItems();

    /**
     * @brief  Gets the number of menu items in this folder.
     *
     * @return  The number of MenuButton child components this folder holds.
     */
    int getFolderSize() const;

    /**
     * @brief  Gets the folder's menu item data.
     *
     * @return  The MenuItem defining this folder.
     */
    MenuItem getFolderMenuItem() const;

    /**
     * @brief  Gets the current selected folder item.
     *
     * @return  The item currently selected by the user, or a null MenuItem if
     *          no item is selected.
     */
    MenuItem getSelectedItem() const;

    /**
     * @brief  Gets the index of the current menu selection.
     *
     * @return  The index of the menu item selected by the user, or -1 if no
     *          item is selected.
     */
    int getSelectedIndex() const;

    /**
     * @brief  Sets the current selected menu index.
     *
     * @param newSelection  The index of a menu item in this folder that should
     *                      be selected.
     */
    void setSelectedIndex(const int newSelection);

    /**
     * @brief  Finds the closest menu index to a place where the menu was
     *         clicked.
     *
     * @param xPos  The x-coordinate of the spot that was clicked, measured in
     *              pixels from the left edge of the folder component.
     *
     * @param yPos  The y-coordinate of the spot that was clicked, measured in
     *              pixels from the top edge of the folder component.
     *
     * @return      The closest menu index to the clicked spot, to use when
     *              inserting new menu items.
     */
    virtual int closestIndex(const int xPos, const int yPos) const = 0;

    /**
     * @brief  Updates the sizes and positions of all buttons in the menu.
     */
    virtual void updateButtonLayout() = 0;

    /**
     * @brief  Gets the button component of a child menu item in this folder.
     *
     * @param index  The index of one of this folder's child menu items.
     *
     * @return       The MenuButton representing that menu item, or nullptr if
     *               the index is out of bounds.
     */
    MenuButton* getButtonComponent(const int index) const;

private:
    /**
     * @brief  Creates a button component for one of the folder's child menu
     *         items.
     *
     * @param folderItem  The menu item data that will define the new button.
     *
     * @return            The button component, initialized as some concrete
     *                    subclass of FolderComponent::MenuButton.
     */
    virtual MenuButton* createMenuButton(const MenuItem folderItem) const = 0;

    /**
     * @brief  Creates and inserts a new MenuButton when a new child menu
     *         button is created.
     *
     * @param childIndex  The index of the new menu item.
     */
    virtual void childAdded(const int childIndex) final override;

    /**
     * @brief  Removes the corresponding MenuButton when a child menu item is
     *         removed.
     *
     * @param removedIndex  The index of the item that was just removed.
     */
    virtual void childRemoved(const int removedIndex) final override;

    /**
     * @brief  Swaps the positions of two of the folder's MenuButtons when
     *         their corresponding MenuItems are swapped.
     *
     * @param swapIndex1  The index of the first swapped child item.
     *
     * @param swapIndex2  The index of the second swapped child item.
     */
    virtual void childrenSwapped(const int swapIndex1, const int swapIndex2)
        final override;

    /**
     * @brief  Recalculates menu button layout when the folder bounds change.
     */
    virtual void resized() override;

    // Holds the button components representing the menu's child folder items.
    juce::OwnedArray<MenuButton> folderButtons;

    // The current selected button index.
    int selectedIndex = -1;

    // The menu item defining this folder.
    MenuItem folderItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FolderComponent)
};
