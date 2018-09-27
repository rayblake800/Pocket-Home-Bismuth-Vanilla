#pragma once
#include "DesktopEntry.h"
#include "IconThread.h"
#include "AppConfigFile.h"
#include "Localized.h"
#include "AppMenuPopupEditor.h"

/**
 * @file  AppMenuItem.h
 * 
 * @brief  Defines an item in the application menu.
 */

class AppMenuItem
{
public:
    /**
     * The source of the menu item's data, used to read and write data.
     */
    class DataSource;

    /**
     * @brief 
     *
     * @param dataSource
     */
    AppMenuItem(DataSource* dataSource);

    /**
     * @brief 
     *
     * @param toCopy
     */
    AppMenuItem(const AppMenuItem& toCopy);

    virtual ~AppMenuItem() { }

    /**
     * @brief 
     */
    enum class Type
    {
        /* Loads and displays other menu items. */
        folder,
        /* Launches an application. */
        shortcut
    };

    /**
     * @brief 
     *
     * @return 
     */
    Type getType() const;
 
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
    bool isTerminalApp() const;

    /**
     * @brief  Checks if this menu item has an index that can be moved by a
     *         given amount.
     * 
     * @param offset  The value to add to the menu item index.
     * 
     * @return  True if this menu item has an index value i that can be changed 
     *          to i+offset, false if the menu item has no index value, or
     *          i+offset is out of bounds.
     */
    bool canChangeIndex(const int offset) const;

    /**
     * @brief  Compares this menu item with another.
     *
     * @param toCompare  Another menu item to compare with this one.
     * 
     * @return  True if and only if both menu items store identical data.
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

private:
    /**
     * @return true iff this menu item has categories that can be edited,
     *          defaults to false.
     */
    virtual bool hasEditableCategories() const;

    /**
     * @return true iff this menu item has a command that can be edited,
     *          defaults to false.
     */
    virtual bool hasEditableCommand() const;

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual juce::String getEditorTitle() const;

    /**
     * Gets a PopupEditorComponent callback function that will apply 
     * changes from an AppMenuPopupEditor to this menu item.
     */
    virtual std::function<void(AppMenuPopupEditor*) > getEditorCallback();

    /**
     * Removes the source of this menu item's data
     * 
     * @return true iff the source was removed.
     */
    virtual bool removeMenuItemSource();

    /**
     * If possible, change the index of this menu item by some
     * offset amount.
     * 
     * @param offset  This will be added to the menu item's current index, if
     *                 possible.
     * 
     * @return true iff the operation succeeded.
     */
    virtual bool moveDataIndex(int offset);

    /**
     * Gets the string to add before a launch command to make it launch in the
     * terminal.
     */
    juce::String getTermLaunchPrefix() const;
private:
    //folders will have a factory interface here to create new menu items.
    FactoryInterface* factoryInterface = nullptr;

    /**
     * Using a private Localized member is necessary to allow AppMenuItem
     * subclasses to inherit Localized. 
     */
    class LocaleText : public Localized{
        public:
            LocaleText() : Localized("AppMenuItem") {}
            virtual ~LocaleText() {}
    };
    LocaleText txt;
    //localized text keys;
    static const constexpr char * delete_APP = "delete_APP";
    static const constexpr char * question_mark = "question_mark";
    static const constexpr char * really_delete = "really_delete";
    static const constexpr char * edit_menu_item = "edit_menu_item";

};
