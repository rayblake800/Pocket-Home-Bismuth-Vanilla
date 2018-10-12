#pragma once
#include "IconThread.h"
#include "Localized.h"
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
    AppMenuItem(MenuItemData::Ptr dataSource);

    /**
     * @brief  Creates a menu item copying data from another menu item. 
     *
     * @param toCopy  A menu item to copy when constructing a new menu item.
     */
    AppMenuItem(const AppMenuItem& toCopy);

    /**
     * @brief  Creates a null menu item, holding no data.
     */
    AppMenuItem() { }

    virtual ~AppMenuItem() { }

    /**
     * @brief  Checks if this menu item holds valid menu data.
     *
     * @return  Whether this menu item has menu data.
     */
    bool isNull();

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
     * @brief  Gets any launch command associated with this menu item.
     *
     * @return  An application launch command, or the empty string if the menu
     *          item doesn't launch an application.
     */
    juce::String getCommand() const;

    /**
     * @brief  Checks if this menu item is a terminal application.
     *
     * @return  True if and only if the menu item has a launch command that 
     *          should run within a new terminal window.
     */
    bool getLaunchedInTerm() const;

    /**
     * @brief  Gets all application categories associated with this menu item.
     *
     * @return  Categories of application used to find this menu item's folder
     *          items, or the list of categories that define an application 
     *          launched by this menu item.
     */
    juce::StringArray getCategories() const;

    /**
     * @brief  Gets the folder menu item that contains this menu item.
     *
     * @return   The parent folder, or a null menu item if this menu item is
     *           null or is the root folder item.
     */
    AppMenuItem getParentFolder() const;
    
    /**
     * @brief  Gets the menu item's index within its folder.
     *
     * @return  The menu item index, or -1 if the menu item is null or not
     *          placed within a folder. 
     */
    const MenuIndex& getIndex() const;

    /**
     * @brief  Checks if this menu item represents a folder within the menu.
     *
     * @return  Whether this menu item opens a new menu folder.
     */
    bool isFolder() const;

    /**
     * @brief  Gets the number of menu items in the folder opened by this menu
     *         item.
     *
     * @return  The number of folder items, or zero if this menu item does not
     *          open a folder.
     */
    int getFolderSize() const;
    
    /**
     * @brief  Gets the number of folder items held by this menu item that can
     *         be reordered.
     *
     * Movable child folder items always come before un-movable ones, so any
     * child folder items with an index less than the movable child count can
     * have their positions swapped.
     *
     * @return  The number of child folder items held that can be re-arranged
     *          in any order.
     */
 
    int getMovableChildCount() const; 

    /**
     * @brief  Gets a menu item held within this menu item.
     *
     * @param folderIndex  The index of the menu item within this menu item's
     *                     folder data.
     *
     * @return             The folder item at the given index, or a null menu
     *                     item if this menu item doesn't have a folder item
     *                     at the given index.
     */
    AppMenuItem getFolderItem(const int folderIndex) const;

    /**
     * @brief  Gets all menu items held within this menu item.
     *
     * @return  All menu items within this menu item, or an empty array if this
     *          menu item is not a folder.
     */
    juce::Array<AppMenuItem> getFolderItems() const;

    /**
     * @brief  Compares this menu item with another.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether both menu items store identical data.
     */
    bool operator==(const AppMenuItem& toCompare) const;

    /**
     * @brief  Compares this menu item with another.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether this menu item comes before the other in the
     *                   menu tree.
     */
    bool operator<(const AppMenuItem& toCompare) const;

    /**
     * @brief  An interface for objects that can edit AppMenuItems.
     */
    class Editor
    {
    public:
        Editor() { }
        virtual ~Editor() { }

        /**
         * @brief  Gets a menu item's internal data object.
         *
         * @param menuItem  The menu item to access.
         *
         * @return          The internal menu data object pointer, which
         *                  can be used to edit the menu data.
         */
        MenuItemData::Ptr getMenuItemData(AppMenuItem& menuItem) const;

        /**
         * @brief  Removes a menu item's internal data object.
         *
         * @param menuItem  A menu item that has data that should be
         *                  removed.
         */
        MenuItemData::Ptr clearMenuItemData(AppMenuItem& menuItem) const;
    };

    /**
     * @brief  Gets an appropriate title to use for a deletion confirmation 
     *         window.
     *
     * @return  A localized confirmation title string.
     */
    juce::String getConfirmDeleteTitle() const;

    /**
     * @brief  Gets appropriate descriptive text for a deletion confirmation 
     *         window.
     *
     * @return  A localized confirmation description string.
     */
    juce::String getConfirmDeleteMessage() const;

    /**
     * @brief  Gets a title to use when editing this menu item.
     *
     * @return  The title to display over an editor for this menu item. 
     */
    juce::String getEditorTitle() const;

private:
    /* Stores all menu data. */
    MenuItemData::Ptr dataSource;
};
