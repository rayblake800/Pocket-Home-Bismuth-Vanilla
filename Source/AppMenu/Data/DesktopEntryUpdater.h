#pragma once
#include "AppMenu/AppMenu.h"
#include "AppMenu/Data/ConfigFile.h"
#include "DesktopEntry/Loader.h"
#include "DesktopEntry/UpdateListener.h"

/**
 * @file  DesktopEntryUpdater.h
 *
 * @brief  Listens for changes to desktop entries, copying all changes over to 
 *         the menu.
 */
class AppMenu::DesktopEntryUpdater : public DesktopEntry::UpdateListener
{         
public:
    DesktopEntryUpdater() { }

    virtual ~DesktopEntryUpdater();

    /**
     * @brief  Loads all desktop entry menu items for the first time.
     */
    void initialEntryLoad();

    /**
     * @brief  Loads and adds all desktop entry menu items that belong in a
     *         menu folder item, or in any of its child folder items.
     *
     * @param folderItem  The folder menu item to update.
     */
    void loadFolderEntries(MenuItem folderItem);

private:
    /**
     * @brief  Recursively applies a function to a menu folder and all its
     *         subfolders.
     *
     * @param startingFolder  The initial folder to search.
     *
     * @param folderAction    An action to apply to each folder item.
     *
     */
    void recursiveFolderAction(MenuItem startingFolder,
            const std::function<void(MenuItem)> folderAction) const;

    /**
     * @brief  Applies a function to each desktop entry in a list that shares
     *         categories with a folder menu item.
     *
     * @param folder        A folder menu item.
     *
     * @param entries       A list of desktop entries.
     *
     * @param entryAction   A function to call for each entry that shares a 
     *                      category with the folder.
     */
    void foreachMatchingEntry(MenuItem folder,
            const juce::Array<DesktopEntry::EntryFile>& entries,
            std::function<void(const DesktopEntry::EntryFile&)> entryAction) 
        const;

    /**
     * @brief  Adds folder items created from desktop entry files to a folder
     *         if the folder and the entry share application categories.
     *
     * @param folder   A folder menu item.
     *
     * @param entries  A list of entries that may need to be added to the folder
     *                 as new menu items.
     */
    void addEntryItems(MenuItem folder,
            const juce::Array<DesktopEntry::EntryFile>& entries) const;
       
    
    /**
     * @brief  Applies desktop entry updates to all matching menu items in a
     *         folder.
     *
     * @param folder   A folder menu item.
     *
     * @param entries  A list of entries to attempt to find and update within 
     *                 the folder.
     */
    void updateEntryItems(MenuItem folder,
        const juce::Array<DesktopEntry::EntryFile>& entries) const;

    /**
     * @brief  Checks new desktop entries to see if they should be added to the
     *         menu.
     *
     * @param entryFileIDs  The desktop file IDs of all new desktop entries.
     */
    virtual void entriesAdded(const juce::StringArray entryFileIDs) override;
    
    /**
     * @brief  Removes desktop entry menu buttons when their desktop entry files
     *         are deleted or hidden.
     *
     * @param entryFileIDs  The desktop file IDs of all removed or hidden
     *                      desktop entries.
     */
    virtual void entriesRemoved(const juce::StringArray entryFileIDs) override;
    
    /**
     * @brief  Refreshes desktop entry menu buttons when their desktop entry
     *         files change.
     *
     * @param entryFileIDs  The desktop file IDs of all updated desktop entries.
     */
    virtual void entriesUpdated(const juce::StringArray entryFileIDs) override;
    
    /* Holds the loading callback ID if waiting for the desktop entry thread to
       load entry files. */
    DesktopEntry::CallbackID initialLoadingID = 0;
};
