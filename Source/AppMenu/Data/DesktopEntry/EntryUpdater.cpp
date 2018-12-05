#define APPMENU_IMPLEMENTATION
#include "DesktopEntry/Loader.h"
#include "AppMenu/Data/JSON/MenuFile.h"
#include "AppMenu/Data/MenuItem.h"
#include "AppMenu/Data/DesktopEntry/EntryData.h"
#include "AppMenu/Data/DesktopEntry/EntryActions.h"
#include "AppMenu/Data/DesktopEntry/EntryUpdater.h"

/**
 * @brief  Loads a list of DesktopEntry::EntryFile objects from their desktop
 *         file IDs.
 *
 * @param entryFileIDs  A list of desktop file IDs for new or updated desktop
 *                      entry files.
 *
 * @return              The resulting list of EntryFile objects.
 */
static juce::Array<DesktopEntry::EntryFile> loadEntryFiles
(const juce::StringArray entryFileIDs)
{
    juce::Array<DesktopEntry::EntryFile> entries;
    DesktopEntry::Loader entryLoader;
    for(const juce::String& entryID : entryFileIDs)
    {
        entries.add(entryLoader.getDesktopEntry(entryID));
        if(entries.getLast().isMissingData())
        {
            entries.removeLast();
        }
    }
    return entries;
}

/*
 * Adds menu items for new desktop entries to all relevant folders.
 */
void AppMenu::EntryUpdater::entriesAdded(const juce::StringArray entryFileIDs)
{
    DBG("AppMenu::EntryUpdater::" << __func__ << ": Updating menu with "
            << entryFileIDs.size() << " new entry files.");
    juce::Array<DesktopEntry::EntryFile> newEntries
        = loadEntryFiles(entryFileIDs);
    MenuFile appConfig;
    EntryActions::recursiveFolderAction(appConfig.getRootFolderItem(), 
    [this, &newEntries](MenuItem folder)
    {
        EntryActions::addEntryItems(folder, newEntries);
    });
}

/*
 * Removes desktop entry menu buttons when their desktop entry files are deleted
 * or hidden.
 */
void AppMenu::EntryUpdater::entriesRemoved(const juce::StringArray entryFileIDs)
{
    DBG("AppMenu::EntryUpdater::" << __func__ << ": Updating menu with "
            << entryFileIDs.size() << " removed entry files.");
    MenuFile appConfig;
    EntryActions::recursiveFolderAction(appConfig.getRootFolderItem(), 
    [this, &entryFileIDs] (MenuItem folder)
    {
        int firstEntryIndex = folder.getMovableChildCount() - 1;
        for(int i = firstEntryIndex; i < folder.getFolderSize(); i++)
        {
            MenuItem entryItem = folder.getFolderItem(i);
            if(entryFileIDs.contains(entryItem.getID()))
            {
                entryItem.remove(false);
            }
        }
    });
}
    
/*
 * Refreshes desktop entry menu buttons when their desktop entry files change.
 */
void AppMenu::EntryUpdater::entriesUpdated(const juce::StringArray entryFileIDs)
{
    DBG("AppMenu::EntryUpdater::" << __func__ << ": Updating menu with "
            << entryFileIDs.size() << " changed entry files.");
    juce::Array<DesktopEntry::EntryFile> newEntries
        = loadEntryFiles(entryFileIDs);
    MenuFile appConfig;
    EntryActions::recursiveFolderAction(appConfig.getRootFolderItem(), 
    [this, &newEntries](MenuItem folder)
    {
        // Find and remove any entries that no longer have matching categories.
        juce::Array<DesktopEntry::EntryFile> toRemove = newEntries;
        EntryActions::foreachMatchingEntry(folder, newEntries,
        [&toRemove](const DesktopEntry::EntryFile& matchingEntry)
        {
           toRemove.removeAllInstancesOf(matchingEntry); 
        });
        for(const DesktopEntry::EntryFile& removedEntry : toRemove)
        {
            int firstEntryIndex = folder.getMovableChildCount() - 1;
            juce::String removedID = removedEntry.getDesktopFileID();
            for(int i = firstEntryIndex; i < folder.getFolderSize(); i++)
            {
                MenuItem entryItem = folder.getFolderItem(i);
                if(removedID == entryItem.getID())
                {
                    entryItem.remove(false);
                }
            }
        }

        // Update existing entries with changed data.
        EntryActions::updateEntryItems(folder, newEntries);

        // Add existing entries that were changed to share a category with the
        // folder.
        EntryActions::addEntryItems(folder, newEntries);
    });
}
