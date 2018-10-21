/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "JuceHeader.h"
#include "Localized.h"
#include "AppMenu.h"
#include "MenuData/MenuItem.h"
#include "Editors/PopupEditor.h"

/**
 * @file  ContextMenu.h
 *
 * @brief  Creates popup context menus used to edit the AppMenu.
 *
 * When given an application menu item through setMenuItemOptions, a folder
 * through setFolderOptions, or when called with setGenericOptions, the
 * ContextMenu will update its contents with relevant options for editing the
 * given menu item, adding new items to the folder, or adding items to the root
 * menu folder.
 */
class AppMenu::ContextMenu : private juce::PopupMenu, private Localized
{
public:
    ContextMenu();
    virtual ~ContextMenu() { }

    /**
     * @brief  Sets popup menu options for editing a menu item.
     *
     * @param menuItem  The item the menu options should edit.
     */
    void setMenuItemOptions(MenuItem menuItem);

    /**
     * @brief  Set popup menu options for editing an open folder.
     *
     * @param folderItem    The folder menu item the menu options should edit.
     *
     * @param insertIndex   A menu index to use when inserting new items into
     *                      the menu, or zero by default.
     */
    void setFolderOptions(MenuItem folderItem, const int insertIndex = 0);


    /**
     * @brief  Set generic popup menu options, when no specific folder or menu
     *         item is clicked or selected.
     */
    void setGenericOptions();

    /**
     * @brief  Shows the popup menu, and handles the user's selection.
     */
    void showAndHandleSelection();

private:
    /**
     * @brief  Insert a new menu item into the menu, updating the menu
     *         component.
     *
     * @param folder   The folder where the new item will be added.
     *
     * @param newItem  The new menu item to insert into the folder.
     *
     * @param index    The index in the folder where the new item will be
     *                 inserted.
     */
    void insertMenuItem
    (MenuItem folder, MenuItem newItem, const int index = 0);

    /**
     * @brief   Swaps two menu items, updating the menu component.
     *
     * @param folder      The folder containing the items to swap.
     *
     * @param swapIndex1  The folder index of the first item to move. 
     *
     * @param swapIndex2  The folder index of the second item to move.
     */
    void swapMenuItems
    (MenuItem folder, const int swapIndex1, const int swapIndex2);

    /**
     * @brief  Removes a menu item, updating the menu component.
     *
     * @param toRemove  The menu item to remove.
     */
    virtual void removeMenuItem(MenuItem toRemove);

    /**
     * @brief  Shows an editor component above the MenuComponent.
     *
     * @param editor  The new editor to show.
     */
    virtual void showPopupEditor(PopupEditor* editor) = 0;

    /* Last menu item used to set context menu options: */
    MenuItem editedItem;
    /* Last saved index where menu items will be inserted into a folder: */
    int insertIndex = 0;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
