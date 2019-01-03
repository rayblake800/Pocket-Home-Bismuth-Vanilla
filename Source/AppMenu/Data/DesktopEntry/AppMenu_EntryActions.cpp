#define APPMENU_IMPLEMENTATION
#include "AppMenu_EntryActions.h"
#include "AppMenu_EntryData.h"
#include "AppMenu_MenuItem.h"
#include "DesktopEntry/EntryFile.h"

/*
 * Recursively applies a function to a menu folder and all its subfolders.
 */
void AppMenu::EntryActions::recursiveFolderAction(        
        MenuItem startingFolder, 
        const std::function<void(MenuItem)> folderAction) 
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
void AppMenu::EntryActions::foreachMatchingEntry(
        MenuItem folder,
        const juce::Array<DesktopEntry::EntryFile>& entries,
        std::function<void(const DesktopEntry::EntryFile&)> entryAction)
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
void AppMenu::EntryActions::addEntryItems(MenuItem folder,
        const juce::Array<DesktopEntry::EntryFile>& entries) 
{
    juce::Array<MenuItem> entryItems = getDesktopEntryItems(folder);
    foreachMatchingEntry(folder, entries, [&entryItems, &folder]
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
        entryItems.add(MenuItem(new EntryData(matchingEntry)));    
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
            DBG("AppMenu::EntryActions::" << __func__
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
void AppMenu::EntryActions::updateEntryItems(MenuItem folder,
        const juce::Array<DesktopEntry::EntryFile>& entries) 
{
    juce::Array<MenuItem> entryItems = getDesktopEntryItems(folder);
    for(const DesktopEntry::EntryFile& entry : entries)
    {
        for(MenuItem& entryItem : entryItems)
        {
            if(entryItem.getID() == entry.getDesktopFileID())
            {
                // Update existing item:
                DBG("AppMenu::EntryActions::" << __func__
                        << ": Updating menu item \"" << entryItem.getTitle()
                        << "\" with desktop file ID=" << entryItem.getID());
                DBG("AppMenu::EntryLoader::" << __func__                         
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

/*
 * Gets an array containing all desktop entry menu items in a folder.
 */
juce::Array<AppMenu::MenuItem> AppMenu::EntryActions::getDesktopEntryItems
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
