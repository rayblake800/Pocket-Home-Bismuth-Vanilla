/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "IconThread.h"
#include "Nullable.h"
#include "AppMenu/AppMenu.h"
#include "AppMenu/Data/ItemData.h"

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

    /**
     * @brief  Gets all child menu items held within this menu item.
     *
     * @return  The list of all child menu items within this folder item, or the
     *          empty list if this MenuItem is null or not a folder.
     */
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
     * @brief  Checks if two menu items represent the same item in the menu.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether both menu items store the same data object.
     */
    bool operator==(const MenuItem& toCompare) const;
    
    /**
     * @brief  Checks if two menu items do not represent the same item in the 
     *         menu.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether the menu items do not store the same data 
     *                   object.
     */
    bool operator!=(const MenuItem& toCompare) const;

    /**
     * @brief  Compares this menu item with another.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return           Whether this menu item's title comes before toCompare's
     *                   title alphabetically.
     */
    bool operator<(const MenuItem& toCompare) const;
        
    /* Describes the different data items held by the menu item. */
    typedef ItemData::DataField DataField; 

    /**
     * @brief  Checks if a data field within the menu item can be edited.
     *
     * @param dataField  The type of data being checked.
     *
     * @return           True if and only if the data field is editable.
     */
    bool isEditable(const DataField dataField) const;

    /**
     * @brief  Sets the menu item's displayed title.
     *
     * This will do nothing if the menu item's title field is not editable.
     *
     * @param title  The new display title to assign to the menu item.
     */
    void setTitle(const juce::String& title);

    /**
     * @brief  Sets the name or path used to load the menu item's icon file.
     *
     * This will do nothing if the menu item's icon field is not editable.
     *
     * @param iconName  The new icon name or path.
     */
    void setIconName(const juce::String& iconName);

    /**
     * @brief   Sets the menu item's application launch command.
     *
     * This will do nothing if the menu item's command field is not
     * editable.
     *
     * @param command   The new command string to run when this menu item is
     *                  activated.
     */
    void setCommand(const juce::String& command);

    /**
     * @brief  Sets if the menu item runs its command in a new terminal 
     *         window.
     *
     * This will do nothing if the menu item's launchInTerm option is not
     * editable.
     *
     * @param launchInTerm   True to run any launch command assigned to this
     *                       menu item within a new terminal window, false
     *                       to run menu commands normally.
     */
    void setLaunchedInTerm(const bool launchInTerm);

    /**
     * @brief  Sets the application categories connected to the menu item.
     *
     * This will do nothing if the menu item's categories are not editable.
     *
     * @param categories   The new set of category strings to assign to this
     *                     menu item.
     */
    void setCategories(const juce::StringArray& categories);

    /**
     * @brief  Attempts to insert a menu item into this menu item,
     *         saving the change to the folder menu item's data source.
     *
     * This will fail if folderItem is not a folder, the index is outside of
     * the bounds of the folder item's movable child items, or if the child
     * MenuItem parameter is null.
     *
     * @param childItem   The new item to insert into the folder.
     *
     * @param index       The index in the folder where the new menu item
     *                    should be inserted.
     *
     * @return            Whether the new item was inserted successfully.
     */
    bool insertChild(const MenuItem childItem, const int index);

    /**
     * @brief  Attempts to remove a menu item from the menu, saving the
     *         change to the menu item's data source.
     *
     * @param toRemove  The menu item to remove from the application menu.
     *
     * @return          True if the menu item was removed, false if the menu
     *                  item was null or otherwise not located in the menu.
     */
    bool remove();

    /**
     * @brief  Swaps the positions of two folder items, saving the change to 
     *         the folder menu item's data source.
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
    bool swapChildren(const int childIdx1, const int childIdx2);

    /**
     * @brief  Saves all changes made to the menu item back to the menu item's
     *         data source.
     */
    void saveChanges();

    /**
     * @brief  A wrapper for the ItemData::Listener class, used to track menu
     *         data without directly interacting with ItemData objects.  Each
     *         listener may only track a single ItemData object.
     */
    class Listener : protected ItemData::Listener
    {
    public:
        /* Allow MenuItem to cast Listeners to their protected base when adding
           or removing them. */
        friend class MenuItem;

        Listener() { }
        virtual ~Listener() { }
    };

    /**
     * @brief  If this MenuItem is not null, add a Listener object to its menu
     *         data.
     *
     * @param toAdd  The listener that will track changes to the MenuItem's
     *               data.
     */
    void addListener(Listener* toAdd);

    /**
     * @brief  If this menuItem is not null, make a Listener object stop
     *         tracking its menu data.
     *
     * @param toRemove  The listener that will no longer track this menu item's
     *                  data.
     */
    void removeListener(Listener* toRemove);
    
    JUCE_LEAK_DETECTOR(MenuItem);
};

/* Only include this file directly in the AppMenu implementation! */
#endif
