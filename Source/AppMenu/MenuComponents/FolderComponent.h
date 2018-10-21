/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "MenuComponent.h"
#include "AppMenu.h"

/**
 * @file  FolderComponent.h
 *
 * @brief  Creates and displays menu item components for all items in a single
 *         application menu folder.
 */
class AppMenu::FolderComponent : public MenuComponent::Folder,
private MenuItem::Listener
{
public:
    /**
     * @brief  Creates a new folder component.
     *
     * @param menuComponent  The menuComponent that created this folder
     *                       component.
     *
     * @param folderItem     The menu data object used to construct the
     *                       folder.
     */
    FolderComponent(MenuComponent& menuComponent, MenuItem folderItem);

    virtual ~FolderComponent() { }

    /**
     * @brief  Gets the MenuItem that defines this folder.
     *
     * @return   The menu data object used to construct the folder.
     */
    virtual MenuItem getFolderItem() const override;

    /**
     * @brief  Gets the current selected folder item.
     *
     * @return  The item currently selected by the user, or a null MenuItem
     *          if no item is selected.
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
     * @brief  Represents a menu item within the folder as a juce button
     *         component.
     */
    class ItemButton : public juce::Button
    {
    public:
        /**
         * @brief  Creates a new menu item component.
         *
         * @param folderItem  Menu item data that will define this ItemButton.
         */
        ItemButton(const MenuItem folderItem);

        virtual ~ItemButton() { }

        /**
         * @brief  Gets the menu item that defines this ItemButton.
         *
         * @return  The menu data object used to construct the button.
         */
        MenuItem getMenuItem() const;

    private:
        /* The button's menu data object. */
        MenuItem menuItem;
    };

protected:
    /**
     * @brief  Sets the current selected menu index.
     *
     * @param newSelection  The index of a menu item in this folder that should
     *                      be selected.
     */
    void setSelectedIndex(const int newSelection);

    /**
     * @brief  Gets the button component of a child menu item in this folder.
     *
     * @param index  The index of one of this folder's child menu items.
     *
     * @return       The ItemButton representing that menu item, or nullptr if
     *               the index is out of bounds.
     */
    ItemButton* getButtonComponent(const int index);

private:
    /**
     * @brief  Creates a button component for one of the folder's child menu
     *         items.
     *
     * @param folderItem  The menu item data that will define the new button.
     *
     * @return            The button component, initialized as some concrete
     *                    subclass of FolderComponent::ItemButton.
     */
    virtual ItemButton* createMenuButton(const MenuItem folderItem) = 0;

    /**
     * @brief  Called whenever a folder menu item is clicked, allowing the
     *         folder to handle the event.
     *
     * @param clickedItem   The menu item that was clicked, either a child menu
     *                      item, or the folder itself.
     *
     * @param rightClicked  Whether the click event was a right-click.
     *
     * @return              True if the event should be passed on to the
     *                      MenuComponent, false to prevent it from being passed
     *                      on.
     */
    virtual bool handleMenuClick(const MenuItem clickedItem, bool rightClicked);

    /**
     * @brief  Handles all mouse events over this folder component.  This passes
     *         on all mouse events to the MenuComponent, unless the
     *         FolderComponent implementation chooses to intercept them.
     *
     * @param event  An object describing the mouse event.
     */
    virtual void mouseDown(const juce::MouseEvent& event) override;

    /**
     * @brief  Creates and inserts a new ItemButton when a new child menu button
     *         is created.
     *
     * @param folderItem  This should always be this folder's menu item.
     *
     * @param childIndex  The index of the new menu item.
     */
    virtual void childAdded(MenuItem folderItem, const int childIndex) override;

    /**
     * @brief  Removes the corresponding ItemButton when a child menu item is
     *         removed.
     *
     * @param removedIndex  The index of the item that was just removed.
     */
    virtual void childRemoved(const int removedIndex) override;

    /**
     * @brief  Swaps the positions of two of the folder's ItemButtons when their
     *         corresponding MenuItems are swapped.
     *
     * @param swapIndex1  The index of the first swapped child item.
     *
     * @param swapIndex2  The index of the second swapped child item.
     */
    virtual void childrenSwapped(const int swapIndex1, const int swapIndex2);

    /**
     * @brief  Updates the sizes and positions of all buttons in the menu.
     */
    virtual void updateButtonLayout() = 0;

    /* Holds the button components representing the menu's child folder items.*/
    juce::OwnedArray<ItemButton> folderButtons;

    /* The current selected button index. */
    int selectedIndex = -1;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
