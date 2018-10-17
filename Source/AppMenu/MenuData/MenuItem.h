#pragma once
#include "IconThread.h"
#include "Localized.h"
#include "ItemData.h"
#include "Nullable.h"
#include "AppMenu.h"

/**
 * @file  MenuItem.h
 * 
 * @brief  Defines an item in the application menu.
 */

class AppMenu::MenuItem : public Nullable<AppMenu::ItemData::Ptr>
{
public:
    /**
     * @brief  Creates a menu item from some source of menu data. 
     *
     * @param dataSource  A menu data source the new MenuItem will clone.
     */
    MenuItem(ItemData::Ptr dataSource);

    /**
     * @brief  Creates a menu item copying data from another menu item. 
     *
     * @param toCopy  A menu item to copy when constructing a new menu item.
     */
    MenuItem(const MenuItem& toCopy);

    /**
     * @brief  Creates a null menu item, holding no data.
     */
    MenuItem() { }
    
    virtual ~MenuItem() { }

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
    MenuItem getParentFolder() const;
    
    /**
     * @brief  Gets the menu item's index within its folder.
     *
     * @return  The menu item index, or -1 if the menu item is null or not
     *          placed within a folder. 
     */
    const int getIndex() const;

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
    MenuItem getFolderItem(const int folderIndex) const;
    juce::Array<MenuItem> getFolderItems() const;

    /**
     * @brief  Gets all menu items held within this menu item.
     *
     * @return  All menu items within this menu item, or an empty array if this
     *          menu item is not a folder.
     */
    
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
     * @brief  Gets an appropriate title to use for a menu item editor window.
     *
     * @return  A localized editor title string.
     */
    juce::String getEditorTitle() const;


    /**
     * @brief  Compares this menu item with another.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether both menu items store identical data.
     */
    bool operator==(const MenuItem& toCompare) const;

    /**
     * @brief  Compares this menu item with another.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether this menu item comes before the other in the
     *                   menu tree.
     */
    bool operator<(const MenuItem& toCompare) const;

    /**
     * @brief  An interface for objects that can edit MenuItems.
     */
    class Editor
    {
    public:
        Editor() { }
        virtual ~Editor() { }
        
        typedef ItemData::DataField DataField; 

        /**
         * @brief  Checks if a data field within a menu item can be edited.
         *
         * @param menuItem   The menu item to check.
         *
         * @param dataField  The type of data being checked.
         *
         * @return           True if and only if the data field is editable.
         */
        bool isEditable(MenuItem menuItem, const DataField dataField)
            const;

        /**
         * @brief  Sets a menu item's displayed title.
         *
         * This will do nothing if the menu item's title field is not editable.
         *
         * @param menuItem  The menu item being edited.
         *
         * @param title     The new display title to assign to the menu item.
         */
        void setTitle(MenuItem menuItem, const juce::String& title) const;

        /**
         * @brief  Sets the name or path used to load a menu item's icon file.
         *
         * This will do nothing if the menu item's icon field is not editable.
         *
         * @param menuItem  The menu item being edited.
         *
         * @param iconName  The new icon name or path.
         */
        void setIconName(MenuItem menuItem, const juce::String& iconName)
            const;

        /**
         * @brief   Sets a menu item's application launch command.
         *
         * This will do nothing if the menu item's command field is not
         * editable.
         *
         * @param menuItem  The menu item being edited.
         *
         * @param command   The new command string to run when this menu item is
         *                  activated.
         */
        void setCommand(MenuItem menuItem, const juce::String command)
            const;

        /**
         * @brief  Sets if a menu item runs its command in a new terminal 
         *         window.
         *
         * This will do nothing if the menu item's launchInTerm option is not
         * editable.
         *
         * @param menuItem       The menu item being edited.
         *
         * @param launchInTerm   True to run any launch command assigned to this
         *                       menu item within a new terminal window, false
         *                       to run menu commands normally.
         */
        void setLaunchedInTerm(MenuItem menuItem, const bool launchInTerm)
            const;

        /**
         * @brief  Sets the application categories connected to a menu item.
         *
         * This will do nothing if the menu item's categories are not editable.
         *
         * @param menuItem     The menu item being edited.
         *
         * @param categories   The new set of category strings to assign to this
         *                     menu item.
         */
        void setCategories(MenuItem menuItem, 
                const juce::StringArray categories) const;

        /**
         * @brief  Attempts to replace a menu item in its parent folder, saving 
         *         the change to the menu item's data source.
         *
         * This will fail if toReplace is not located at a movable index in the
         * menu, or if either it or replacementItem are null.
         *
         * @param toReplace         The menu item being replaced.
         *
         * @param replacementItem   The replacement menu item.
         *
         * @return                  Whether the menu item was successfully 
         *                          replaced.
         */
        bool replace(MenuItem toReplace, MenuItem replacementItem)
            const;

        /**
         * @brief  Attempts to insert a menu item into a folder menu item,
         *         saving the change to the folder menu item's data source.
         *
         * This will fail if folderItem is not a folder, the index is outside of
         * the bounds of the folder item's movable child items, or if either
         * MenuItem parameter is null.
         *
         * @param folderItem  A folder menu item within the application menu.
         *
         * @param childItem   The new item to insert into the folder.
         *
         * @param index       The index in the folder where the new menu item
         *                    should be inserted.
         *
         * @return            Whether the new item was inserted successfully.
         */
        bool insertChild(MenuItem folderItem, MenuItem childItem,
                const int index) const;

        /**
         * @brief  Attempts to remove a menu item from the menu, saving the
         *         change to the menu item's data source.
         *
         * @param toRemove  The menu item to remove from the application menu.
         *
         * @return          True if the menu item was removed, false if the menu
         *                  item was null or otherwise not located in the menu.
         */
        bool remove(MenuItem toRemove) const;

        /**
         * @brief  Swaps the positions of two folder items, saving the change to 
         *         the folder menu item's data source.
         *
         * @param folderItem  A folder menu item within the application menu.
         *
         * @param childIdx1   The index within the folder item of the first 
         *                    child item to move.
         *
         * @param childIdx2   The index within the folder item of the second 
         *                    child item to move.
         *
         * @return            True if the child menu items were swapped, false 
         *                    if the indices did not specify two valid menu 
         *                    items within the folder's movable child menu 
         *                    items, or if the folder item was null.
         */
        bool swapChildren
        (MenuItem folderItem, const int childIdx1, const int childIdx2);

        /**
         * @brief  Saves all changes made to a menu item back to the menu item's
         *         data source.
         *
         * @param menuItem  A menu item that has been changed by an 
         *                  MenuItem::Editor.
         */
        void saveChanges(MenuItem menuItem) const;
    };
};
