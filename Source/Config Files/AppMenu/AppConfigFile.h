#pragma once
#include "ConfigFile.h"
#include "AppJSON.h"

/**
 * @file AppConfigFile.h
 * 
 * @brief Loads the pinned application shortcuts and folders displayed in the 
 *        AppMenuComponent.
 * 
 * @see AppMenuComponent.h
 */

class AppConfigFile : public ConfigFile<AppJSON>
{
public:
    AppConfigFile() { }

    virtual ~AppConfigFile() { }

    /**
     * Gets the main list of application shortcuts. 
     * 
     * @return  A list of AppShortcuts to be pinned to the main folder
     *          of the AppMenu.
     */
    juce::Array<AppShortcut> getShortcuts();

    /**
     * Add a new app to the list of pinned shortcuts in the config file.
     * 
     * @param newApp            The new application data object.
     * 
     * @param index             The position to insert the new application.
     * 
     * @param writeChangesNow   Sets if the change should be written to the 
     *                           config file immediately.
     */
    void addShortcut
    (AppShortcut newApp, int index, bool writeChangesNow = true);

    /**
     * Remove a shortcut from the list of application shortcuts.
     * 
     * @param index            The position of the shortcut to remove.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         config file immediately.
     */
    void removeShortcut(int index, bool writeChangesNow = true);

    /**
     * Find the index of an application shortcut in the list.
     * 
     * @param toFind  The application shortcut to search for in the list.
     * 
     * @return  The index of toFind, or -1 if it was not found in the list.
     */
    int getShortcutIndex(const AppShortcut& toFind);
    
    
    /**
     * Gets the list of application menu folders.
     *
     * @return A list of folders to display in the AppMenu.
     */
    juce::Array<AppFolder> getFolders();

    /**
     * Add a new folder to the list of AppFolders in the config file.
     * 
     * @param newFolder        The new application folder.
     * 
     * @param index            Where to insert the new folder.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         JSON config file immediately.
     */
    void addFolder
    (AppFolder newFolder, int index, bool writeChangesNow = true);

    /**
     * Remove a folder from the list of AppFolders.
     * 
     * @param index            The position of the folder to remove.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         config file immediately.
     */
    void removeFolder(int index, bool writeChangesNow = true);

    /**
     * Find the index of an AppFolder in the list of folders.
     * 
     * @param toFind   The folder to search for in the folder list.
     * 
     * @return  The index of toFind, or -1 if it isn't in the list.
     */
    int getFolderIndex(AppFolder toFind);
};
