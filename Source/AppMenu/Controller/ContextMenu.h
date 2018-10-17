#pragma once
#include "JuceHeader.h"
#include "AppMenu.h"
#include "MenuData/MenuItem.h"
#include "AbstractComponents/PopupEditor.h"

/**
 * @file  ContextMenu.h
 *
 * @brief  Creates popup context menus used to edit the AppMenu.
 */
class AppMenu::ContextMenu : public juce::PopupMenu
{
public:
    ContextMenu() { }

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
    virtual void insertMenuItem
    (MenuItem folder, MenuItem newItem, const int index = 0) = 0;

    /**
     * @brief   Swaps two menu items, updating the menu component.
     *
     * @param folder      The folder containing the items to swap.
     *
     * @param swapIndex1  The folder index of the first item to move. 
     *
     * @param swapIndex2  The folder index of the second item to move.
     */
    virtual void swapMenuItems
    (MenuItem folder, const int swapIndex1, const int swapIndex2) = 0;

    /**
     * @brief  Removes a menu item, updating the menu component.
     *
     * @param toRemove  The menu item to remove.
     */
    virtual void removeMenuItem(MenuItem toRemove) = 0;

    /**
     * @brief  Shows an editor component above the MenuComponent.
     *
     * @param editor  The new editor to show.
     */
    virtual void showPopupEditor(PopupEditor* editor) = 0;
};
