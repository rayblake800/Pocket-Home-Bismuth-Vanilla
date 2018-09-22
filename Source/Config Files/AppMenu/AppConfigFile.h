#pragma once
#include "ConfigFile.h"
#include "AppJSON.h"

/**
 * @file AppConfigFile.h
 * 
 * @brief  Reads and edits the list of pinned application shortcuts and folders 
 *         displayed in the AppMenuComponent.
 *
 * The apps.json file contains two types of custom objects: shortcuts, and
 * folders.  Both types provide a display name, and an icon name.  Shortcuts
 * provide an application launch command, and folders provide a list of
 * application categories.  Each of these objects represents a menu item in the
 * main folder of the AppMenuComponent.
 *
 * AppConfigFile reads all of these custom objects, so they can be used to
 * create the AppMenuComponent.  It also provides methods for adding, editing,
 * and deleting shortcuts and folders, so that the user can edit the application
 * menu from within pocket-home.
 * 
 * @see  AppMenuComponent.h
 */

class AppConfigFile : public ConfigFile<AppJSON>
{
public:
    AppConfigFile() { }

    virtual ~AppConfigFile() { }

    /**
     * @brief  Gets the main list of application shortcuts. 
     * 
     * @return  The list of AppShortcuts shown in the main folder of the 
     *          AppMenu.
     */
    juce::Array<AppShortcut> getShortcuts();

    /**
     * @brief  Adds a new shortcut to the list of shortcuts shown in the 
     *         AppMenu's main folder.
     * 
     * @param shortcut          The new application shortcut object.
     * 
     * @param index             The position to insert the new application.
     * 
     * @param writeChangesNow   Sets if the change should be written to the 
     *                          config file immediately.
     */
    void addShortcut(const AppShortcut& shortcut, const int index,
            const bool writeChangesNow = true);

    /**
     * @brief  Removes a shortcut from the list of application shortcuts.
     * 
     * @param index            The position of the shortcut to remove.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         config file immediately.
     */
    void removeShortcut(const int index, const bool writeChangesNow = true);

    /**
     * @brief  Finds the index of an application shortcut in the list.
     * 
     * @param toFind  The application shortcut to search for in the list.
     * 
     * @return        The index of toFind, or -1 if it was not found in the 
     *                list.
     */
    int getShortcutIndex(const AppShortcut& toFind);
    
    
    /**
     * @brief  Gets the list of application menu folders.
     *
     * @return  A list of folders to display in the AppMenu.
     */
    juce::Array<AppFolder> getFolders();

    /**
     * @brief  Adds a new folder to show in the AppMenu.
     * 
     * @param newFolder        The new application folder.
     * 
     * @param index            Where to insert the new folder.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         JSON config file immediately.
     */
    void addFolder(const AppFolder& newFolder, const int index,
            const bool writeChangesNow = true);

    /**
     * @brief  Removes a folder from the list of AppFolders.
     * 
     * @param index            The position of the folder to remove.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         config file immediately.
     */
    void removeFolder(const int index, const bool writeChangesNow = true);

    /**
     * @brief  Finds the index of an AppFolder in the list of folders.
     * 
     * @param toFind   The folder to search for in the folder list.
     * 
     * @return         The index of toFind, or -1 if it isn't in the list.
     */
    int getFolderIndex(const AppFolder& toFind);
};
