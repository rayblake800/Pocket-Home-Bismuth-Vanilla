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
     * @param folderItem     The menu data object used to construct the
     *                       folder.
     *
     * @param menuComponent  The menuComponent that created this folder
     *                       component.
     */
    FolderComponent(MenuItem folderItem, MenuComponent& menuComponent) ;

    virtual ~FolderComponent() { }

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
        /* Buttons may only be selected and unselected through the folder
           component. */
        friend class FolderComponent;

        /**
         * @brief  Creates a new menu item component.
         *
         * @param menuItem       Menu item data that will define this 
         *                       ItemButton.
         *
         * @param parentFolder   The folder component that creates and owns this
         *                       ItemButton.
         */
        ItemButton(const MenuItem menuItem, FolderComponent& parentFolder);

        virtual ~ItemButton() { }

        /**
         * @brief  Gets the menu item that defines this ItemButton.
         *
         * @return  The menu data object used to construct the button.
         */
        MenuItem getMenuItem() const;

    protected:
        /**
         * @brief  Checks if this is the current selected button within its
         *         folder component.
         *
         * @return   Whether this button is the selected button.
         */
        bool isSelected() const;

    private:
        /* The FolderComponent that created this button. */
        FolderComponent& parentFolder;
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
     * @brief  Finds the closest menu index to a place where the menu was
     *         clicked.
     *
     * @param event  A mouse click event on the folder component.
     *
     * @return       The closest menu index to the clicked spot, to use when
     *               inserting new menu items.
     */
    virtual int closestIndex(const juce::MouseEvent& event) = 0;

    /**
     * @brief  Called whenever a folder menu item is clicked, allowing the
     *         folder to handle the event.
     *
     * @param clickedItem   The child menu item that was clicked.
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
    virtual void mouseDown(const juce::MouseEvent& event) final override;

    /**
     * @brief  Creates and inserts a new ItemButton when a new child menu button
     *         is created.
     *
     * @param childIndex  The index of the new menu item.
     */
    virtual void childAdded(const int childIndex) final override;

    /**
     * @brief  Removes the corresponding ItemButton when a child menu item is
     *         removed.
     *
     * @param removedIndex  The index of the item that was just removed.
     */
    virtual void childRemoved(const int removedIndex) final override;

    /**
     * @brief  Swaps the positions of two of the folder's ItemButtons when their
     *         corresponding MenuItems are swapped.
     *
     * @param swapIndex1  The index of the first swapped child item.
     *
     * @param swapIndex2  The index of the second swapped child item.
     */
    virtual void childrenSwapped(const int swapIndex1, const int swapIndex2)
        final override;

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
