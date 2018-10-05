#pragma once
#include "DesktopEntry.h"
#include "IconThread.h"
#include "AppConfigFile.h"
#include "Localized.h"
#include "AppMenuPopupEditor.h"
#include "MenuItemData.h"

/**
 * @file  AppMenuItem.h
 * 
 * @brief  Defines an item in the application menu.
 */

class AppMenuItem : private Localized
{
public:
    /**
     * @brief  Creates a menu item from some source of menu data. 
     *
     * @param dataSource  A menu data source the new AppMenuItem will clone.
     */
    AppMenuItem(MenuItemData* dataSource);

    /**
     * @brief  Creates a menu item copying data from another menu item. 
     *
     * @param toCopy  A menu item to copy when constructing a new menu item.
     */
    AppMenuItem(const AppMenuItem& toCopy);

    virtual ~AppMenuItem() { }
 
    /**
     * @brief  Gets the menu item's displayed title.
     *
     * @return  The display title of the menu item.
     */
    juce::String getTitle() const;

    /**
     * @brief  Gets the menu item's icon name.
     *
     * @return  The name or path used to load the menu item's icon file. 
     */
    juce::String getIconName() const;

    /**
     * @brief  Gets all items within a folder menu item.
     *
     * @return  All menu items in this folder, or an empty array if this isn't
     *          a folder.
     */
    juce::Array<AppMenuItem> getFolderItems() const;

    /**
     * @brief  Gets any launch command associated with this menu item.
     *
     * @return  An application launch command, or the empty string if the menu
     *          item doesn't launch an application.
     */
    juce::String getCommand() const;

    /**
     * @brief  Gets all application categories associated with this menu item.
     *
     * @return  Categories of application used to find this menu item's folder
     *          items, or the list of categories that define an application 
     *          launched by this menu item.
     */
    juce::StringArray getCategories() const;

    /**
     * @brief  Checks if this menu item is a terminal application.
     *
     * @return  True if and only if the menu item has a launch command that 
     *          should run within a new terminal window.
     */
    bool getLaunchedInTerm() const;

    /**
     * @brief  Compares this menu item with another.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether both menu items store identical data.
     */
    bool operator==(const AppMenuItem& toCompare) const;

    /**
     * @brief  Displays an alert to the user asking if this item should be
     *         removed from the menu, and deletes the menu item if the user
     *         confirms.
     *
     * @param onConfirm  An optional callback function to run if the menu item
     *                   is deleted.
     */
    void deleteOnConfim(const std::function<void()> onConfirm = [](){} );

    /**
     * @brief  Checks if this menu item has categories that may be edited.
     *
     * @return  Whether menu item categories are editable.
     */
    bool hasEditableCategories() const;

    /**
     * @brief  Checks if the menu item has an editable command field.
     *
     * @return  Whether this menu item has a command that can be edited.
     */
    bool hasEditableCommand() const;

    /**
     * @brief  Gets a title to use when editing this menu item.
     *
     * @return  The title to display over an editor for this menu item. 
     */
    juce::String getEditorTitle() const;

    /**
     * @brief  Gets a PopupEditorComponent callback function that will apply 
     *         changes from an AppMenuPopupEditor to this menu item.
     *
     * @return  A function that reads data from a menu item editor to edit this
     *          menu item.
     */
    std::function<void(AppMenuPopupEditor*) > getEditorCallback();

    /**
     * @brief  Removes the source of this menu item's data
     */
    void removeMenuItemSource();
    
    /**
     * @brief  Gets the menu item's index within its folder.
     *
     * @return  The menu item index. 
     */
    int getIndex() const;
    
    /**
     * @brief  Gets the indices of this menu item and all its parents within
     *         the application menu.
     *
     * @return  The indices of this menu item and all parents, listed in parent
     *          ->child order, with this item's index as the last element.
     */
    juce::Array<int> getFullIndex() const;

    /**
     * @brief  Checks if this menu item has an index that can be moved by a
     *         given amount.
     * 
     * @param offset  The value to add to the menu item index.
     * 
     * @return        True if this menu item has an index value i that can be
     *                changed to i+offset, false if the menu item has no index
     *                value, or i+offset is out of bounds.
     */
    bool canMoveIndex(const int offset) const;

    /**
     * @brief  Attempts to change the index of this menu item by some
     *         offset amount.
     * 
     * @param offset  This will be added to the menu item's current index, if
     *                possible.
     * 
     * @return        Whether the menu item's index was successfully adjusted.
     */
    bool moveDataIndex(int offset);

private:
    /* Stores all menu data. */
    juce::ScopedPointer<MenuItemData> dataSource;
};
