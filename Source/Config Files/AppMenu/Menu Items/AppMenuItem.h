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

    virtual ~AppMenuItem() { }

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
    int getFolderSize();

    /**
     * @brief  Gets the menu item's displayed title.
     *
     * @return  The display title of the menu item.
     */
    juce::String getTitle() const;

    /**
     * @brief  Sets the menu item's displayed title.
     *
     * @param title  The new title string to display.
     */
    void setTitle(const juce::String& title);

    /**
     * @brief  Gets the menu item's icon name.
     *
     * @return  The name or path used to load the menu item's icon file. 
     */
    juce::String getIconName() const;

    /**
     * @brief  Sets the name or path used to load the menu item's icon file.
     *
     * @param iconName  The new icon name or path.
     */
    void setIconName(const juce::String& iconName);

    /**
     * @brief  Gets any launch command associated with this menu item.
     *
     * @return  An application launch command, or the empty string if the menu
     *          item doesn't launch an application.
     */
    juce::String getCommand() const;

    /**
     * @brief  Sets the menu item's application launch command.
     *
     * @param newCommand  The new command string to run when this menu item is
     *                    clicked.
     */
    void setCommand(const juce::String& newCommand);

    /**
     * @brief  Gets all application categories associated with this menu item.
     *
     * @return  Categories of application used to find this menu item's folder
     *          items, or the list of categories that define an application 
     *          launched by this menu item.
     */
    juce::StringArray getCategories() const;

    /**
     * @brief  Sets the application categories connected to this menu item.
     *
     * @param categories  The new set of category strings to assign to this menu
     *                    item.
     */
    void setCategories(const juce::StringArray& categories);

    /**
     * @brief  Checks if this menu item is a terminal application.
     *
     * @return  True if and only if the menu item has a launch command that 
     *          should run within a new terminal window.
     */
    bool getLaunchedInTerm() const;

    /**
     * @brief  Sets if this menu item runs its command in a new terminal window.
     *
     * @param termLaunch  True to run any launch command assigned to this
     *                    menu item within a new terminal window.
     */
    void setLaunchedInTerm(const bool termLaunch);

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
     * @brief  Deletes this menu item data from its source.
     */
    void deleteFromSource();

    /**
     * @brief  Writes all changes to this menu item back to its data source.
     */
    void updateSource();
    
    /**
     * @brief  Gets the menu item's index within the menu tree.
     *
     * @return  The menu item index. 
     */
    const MenuIndex& getIndex() const;

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
     * @param offset  The amount to move the menu item's current index, if
     *                possible.
     * 
     * @return        Whether the menu item's index was successfully adjusted.
     */
    bool moveDataIndex(const int offset);

private:
    /* Stores all menu data. */
    MenuItemData::Ptr dataSource;
};
