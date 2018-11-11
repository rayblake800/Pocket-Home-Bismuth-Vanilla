#define APPMENU_IMPLEMENTATION_ONLY
#include "DesktopEntry/Loader.h"
#include "AppMenu/Data/ConfigFile.h"
#include "AppMenu/Data/MenuItem.h"
#include "AppMenu/Data/DesktopEntryData.h"
#include "AppMenu/Data/DesktopEntryUpdater.h"

AppMenu::DesktopEntryUpdater::~DesktopEntryUpdater()
{
    if(initialLoadingID != 0)
    {
        DesktopEntry::Loader entryLoader;
        entryLoader.clearCallback(initialLoadingID);
        initialLoadingID = 0;
    }
}

/*
 * Loads all desktop entry menu items for the first time.
 */
void AppMenu::DesktopEntryUpdater::initialEntryLoad()
{
    AppMenu::ConfigFile appConfig;
    MenuItem rootFolder = appConfig.getRootFolderItem();
    loadFolderEntries(rootFolder);
}

/*
 * Loads and adds all desktop entry menu items that belong in a menu folder 
 * item, or in any of its child folder items.
 */
void AppMenu::DesktopEntryUpdater::loadFolderEntries(MenuItem folderItem) 
{
    DesktopEntry::Loader entryLoader;
    initialLoadingID = entryLoader.waitUntilLoaded([this, folderItem]()
    {
        // The folder should have zero desktop entry items at this point!
        jassert(folderItem.getMovableChildCount() 
                == folderItem.getFolderSize());

        DesktopEntry::Loader entryLoader;
        juce::Array<DesktopEntry::EntryFile> allEntries 
            = entryLoader.getAllEntries();
        recursiveFolderAction(folderItem, [this, &allEntries](MenuItem folder)
        {
            addEntryItems(folder, allEntries);
        });
    });
}

/*
 * Recursively applies a function to a menu folder and all its subfolders.
 */
void AppMenu::DesktopEntryUpdater::recursiveFolderAction(
        MenuItem startingFolder, 
        const std::function<void(MenuItem)> folderAction) const 
{
    folderAction(startingFolder);
    for(int i = 0; i < startingFolder.getMovableChildCount(); i++)
    {
        MenuItem childItem = startingFolder.getFolderItem(i);
        if(childItem.isFolder())
        {
            recursiveFolderAction(childItem, folderAction);
        }
    }
}

/*
 * Applies a function to each desktop entry in a list that shares categories 
 * with a folder menu item.
 */
void AppMenu::DesktopEntryUpdater::foreachMatchingEntry(
        MenuItem folder,
        const juce::Array<DesktopEntry::EntryFile>& entries,
        std::function<void(const DesktopEntry::EntryFile&)> entryAction) const
{
    juce::StringArray folderCategories = folder.getCategories();
    if(folderCategories.isEmpty())
    {
        return;
    }

    // TODO: hardcoding this in at this level is sloppy, find a better option
    if(folderCategories.contains("All"))
    {
        for(const DesktopEntry::EntryFile& entry : entries)
        {
            if(!entry.isMissingData())
            {
                entryAction(entry);
            }
        }
        return;
    }

    for(const DesktopEntry::EntryFile& entry : entries)
    {
        if(entry.isMissingData())
        {
            continue;
        }
        juce::StringArray entryCategories = entry.getCategories();
        for(const juce::String& category : entryCategories)
        {
            if(folderCategories.contains(category))
            {
                entryAction(entry);
                break;
            }
        }
    }
}

/**
 * @brief  Gets an array containing all desktop entry menu items in a folder.
 *
 * @param folder      A folder menu item to scan.
 *
 * @return            All of the folder's desktop entry menu items in order.
 */
static juce::Array<AppMenu::MenuItem> getDesktopEntryItems
(const AppMenu::MenuItem folder)
{
    juce::Array<AppMenu::MenuItem> entryItems;
    for(int i = folder.getMovableChildCount(); 
            i < folder.getFolderSize() && i >= 0;
            i++)
    {
        entryItems.add(folder.getFolderItem(i));
    }
    return entryItems;
}

/**
 * @brief  An anonymous class used to sort desktop entry menu items by title, 
 *         ignoring case.
 */
static class
{
public:
    int compareElements(AppMenu::MenuItem first, AppMenu::MenuItem second)
    {
        juce::String firstTitle = first.getTitle().toUpperCase();
        juce::String secondTitle = second.getTitle().toUpperCase();
        if(firstTitle == secondTitle)
        {
            return 0;
        }
        return (firstTitle < secondTitle) ? -1 : 1;
    }
} entryItemComparator;

/*
 * Adds folder items created from desktop entry files to a folder if the folder 
 * and the entry share application categories.
 */
void AppMenu::DesktopEntryUpdater::addEntryItems(MenuItem folder,
        const juce::Array<DesktopEntry::EntryFile>& entries) const
{
    juce::Array<MenuItem> entryItems = getDesktopEntryItems(folder);
    foreachMatchingEntry(folder, entries, [this, &entryItems, &folder]
    (const DesktopEntry::EntryFile& matchingEntry)
    {
        for(const MenuItem& entryItem : entryItems)
        {
            if(entryItem.getID() == matchingEntry.getDesktopFileID())
            {
                // skip duplicate entries
                return;
            }
        }
        entryItems.add(MenuItem(new DesktopEntryData(matchingEntry)));
    });
    entryItems.sort(entryItemComparator, true);
    const int firstEntryIndex = folder.getMovableChildCount();
    for(int i = 0; i < entryItems.size(); i++)
    {
        MenuItem entryItem = entryItems[i];
        if(entryItem.getIndex() == -1)
        {
            const int insertIndex = i + firstEntryIndex;
            folder.insertChild(entryItem, insertIndex);
        }
        if(entryItem.getIndex() != (i + firstEntryIndex))
        {
            DBG("AppMenu::DesktopEntryUpdater::" << __func__ 
                    << ": Menu item \"" << entryItem.getTitle() 
                    << "\" expected at index " << (i + firstEntryIndex)
                    << ", found at index " << entryItem.getIndex());
            jassertfalse;
        }
    }
}

/*
 * Applies desktop entry updates to all matching menu items in a folder.
 */
void AppMenu::DesktopEntryUpdater::updateEntryItems(MenuItem folder,
        const juce::Array<DesktopEntry::EntryFile>& entries) const
{
    juce::Array<MenuItem> entryItems = getDesktopEntryItems(folder);
    for(const DesktopEntry::EntryFile& entry : entries)
    {
        for(MenuItem& entryItem : entryItems)
        {
            if(entryItem.getID() == entry.getDesktopFileID())
            {
                // Update existing item:
                DBG("AppMenu::DesktopEntryUpdater::" << __func__ 
                        << ": Updating menu item \"" << entryItem.getTitle()
                        << "\" with desktop file ID=" << entryItem.getID());
                DBG("AppMenu::DesktopEntryUpdater::" << __func__ 
                        << ": Updated item is in folder \""
                        << folder.getTitle() << "\" at index " 
                        << entryItem.getIndex());
                entryItem.setTitle(entry.getName());
                entryItem.setIconName(entry.getIcon());
                entryItem.setCommand(entry.getLaunchCommand());
                entryItem.setLaunchedInTerm(entry.getLaunchedInTerm());
                entryItem.setCategories(entry.getCategories());
            }
        }
    }
    // Make sure entries are still sorted
    entryItems.sort(entryItemComparator);
    const int firstEntryIndex = folder.getMovableChildCount();
    for(int i = 0; i < entryItems.size(); i++)
    {
        MenuItem entryItem = entryItems[i];
        const int expectedIndex = i + firstEntryIndex;
        if(entryItem.getIndex() != expectedIndex)
        {
            jassert(expectedIndex > entryItem.getIndex());
            folder.swapChildren(entryItem.getIndex(), expectedIndex);
        }
    }
}

/**
 * @brief  Fills a DesktopEntry::EntryFile array with objects loaded from a list
 *         of desktop entry file ids.
 *
 * @param entryList     The list where EntryFile objects will be added.
 *
 * @param entryFileIds  The list of all desktop entry file IDs to load as 
 *                      EntryFile objects.
 */
static void addEntryFiles(juce::Array<DesktopEntry::EntryFile>& entryList,
        const juce::StringArray entryFileIDs)
{
    DesktopEntry::Loader entryLoader;
    for(const juce::String& entryID : entryFileIDs)
    {
        // All IDs should be valid!
        jassert(entryID.isNotEmpty());
        DesktopEntry::EntryFile newEntry = entryLoader.getDesktopEntry(entryID);
        // All IDs should map to valid entries!
        if(newEntry.isMissingData())
        {
            DBG("AppMenu::DesktopEntryUpdater::" << __func__ 
                    << ": Failed to find data for entry with ID \""
                    << entryID << "\", ignoring desktop entry.");
            continue;
        }
        entryList.add(newEntry);
    }
}

/*
 * Adds menu items for new desktop entries to all relevant folders.
 */
void AppMenu::DesktopEntryUpdater::entriesAdded
(const juce::StringArray entryFileIDs)
{
    juce::Array<DesktopEntry::EntryFile> newEntries;
    addEntryFiles(newEntries, entryFileIDs);
    ConfigFile appConfig;
    recursiveFolderAction(appConfig.getRootFolderItem(), 
    [this, &newEntries](MenuItem folder)
    {
        addEntryItems(folder, newEntries);
    });
}

/*
 * Removes desktop entry menu buttons when their desktop entry files are deleted
 * or hidden.
 */
void AppMenu::DesktopEntryUpdater::entriesRemoved
(const juce::StringArray entryFileIDs)
{
    ConfigFile appConfig;
    recursiveFolderAction(appConfig.getRootFolderItem(), [this, &entryFileIDs]
    (MenuItem folder)
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
void AppMenu::DesktopEntryUpdater::entriesUpdated
(const juce::StringArray entryFileIDs)
{
    juce::Array<DesktopEntry::EntryFile> newEntries;
    addEntryFiles(newEntries, entryFileIDs);
    ConfigFile appConfig;
    recursiveFolderAction(appConfig.getRootFolderItem(), 
    [this, &newEntries](MenuItem folder)
    {
        updateEntryItems(folder, newEntries);
        addEntryItems(folder, newEntries);
    });
}
