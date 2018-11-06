#define DESKTOP_ENTRY_LOADER_IMPLEMENTATION
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "Utils.h"
#include "Locale/TextUser.h"
#include "ThreadResource.h"
#include "XDGDirectories.h"
#include "DesktopEntryThread.h"
#include "DesktopEntryLoader.h"


DesktopEntryLoader::DesktopEntryLoader() : 
ResourceHandler<DesktopEntryThread>(DesktopEntryThread::resourceKey,
        []()->SharedResource* { return new DesktopEntryThread(); }) { }

/*
 * Gets the number of cached desktop entries.
 */
int DesktopEntryLoader::size()
{
    auto deThread = getReadLockedResource();
    return deThread->entries.size();
}

/*
 *  Gets the desktop file IDs of all entries within a category.
 */
juce::StringArray DesktopEntryLoader::getCategoryEntryIDs
(const juce::String& category)
{
    using namespace juce;
    auto deThread = getReadLockedResource();
    return deThread->categories.at(category);
}

/*
 * Gets the desktop file IDs of all entries within a list of several categories.
 */
juce::StringArray DesktopEntryLoader::getCategoryListEntryIDs
    (const juce::StringArray& categoryList)
{
    using namespace juce;
    auto deThread = getReadLockedResource();
    StringArray entryIDs;
    for(const String& category : categoryList)
    {
        entryIDs.addArray(deThread->categories[category]);
    }
    if(categoryList.size() > 1)
    {
        entryIDs.removeDuplicates(false);
    }
    return entryIDs;
}

/*
 * Finds a single DesktopEntry object using its desktop file ID.
 */
DesktopEntry DesktopEntryLoader::getDesktopEntry
(const juce::String& entryFileID)
{
    auto deThread = getReadLockedResource();
    return deThread->entries.at(entryFileID);
}

/*
 * Gets all DesktopEntry objects in a single category.
 */
juce::Array<DesktopEntry> DesktopEntryLoader::getCategoryEntries
(const juce::String& category)
{
    using namespace juce;
    auto deThread = getReadLockedResource();
    const StringArray& entryIDs = deThread->categories.at(category);
    Array<DesktopEntry> entries;
    for(const String& entryID : entryIDs)
    {
        entries.add(deThread->entries.at(entryID));
    }
    return entries;
}

/*
 * Finds all DesktopEntry objects within several categories.
 */
juce::Array<DesktopEntry> DesktopEntryLoader::getCategoryListEntries
(const juce::StringArray& categoryList)
{
    using namespace juce;
    auto deThread = getReadLockedResource();
    StringArray entryIDs = getCategoryListEntryIDs(categoryList);
    Array<DesktopEntry> categoryEntries;
    for (const String& entryID : entryIDs)
    {
        categoryEntries.add(deThread->entries.at(entryID));
    }
    return categoryEntries;
}

/*
 * Reloads a single desktop entry from the file system.
 */
void DesktopEntryLoader::reloadEntry(const juce::String& entryFileID)
{
   using namespace juce;
   auto deThread = getWriteLockedResource();
   // Remove old references to entry:
   deThread->entries.erase(entryFileID);
   for(auto& category : deThread->categories)
   {
       category.second.removeString(entryFileID);
   }
   try
   {
       File entryFile = deThread->findEntryFile(entryFileID);
       DesktopEntry entry(entryFile, entryFileID);
       if(entry.shouldBeDisplayed())
       {
           deThread->entries[entryFileID] = entry;
           StringArray newCategories = entry.getCategories();
           if(newCategories.isEmpty())
           {
               newCategories.add(miscEntryCategory);
           }
           newCategories.add(everyEntryCategory);
           for(const String& category : newCategories)
           {
               deThread->categories[category].add(entryFileID);
           }
       }
   }
   catch(std::out_of_range e)
   {
       DBG("DesktopEntryLoader::" << __func__ << ": No entry found with ID "
               << entryFileID);
   }
   catch(DesktopEntryFileError e)
   {
       DBG("DesktopEntryLoader::" << __func__ << ": File error in entry "
               << entryFileID << ": " << e.what());
   }
   catch(DesktopEntryFormatError e)
   {
       DBG("DesktopEntryLoader::" << __func__ << ": Format error in entry "
               << entryFileID << ": " << e.what());
   }
}


/*
 * Scans all desktop entry files for any changes made since the last time the 
 * DesktopEntryLoader read the entry files.
 */
DesktopEntryLoader::CallbackID DesktopEntryLoader::scanForChanges
(const std::function<void(std::map<juce::String, EntryChange>)> handleChanges)
{
}

/*
 * Schedules an action to run once all entries have been loaded.
 */
DesktopEntryLoader::CallbackID DesktopEntryLoader::waitUntilLoaded
(std::function<void()> onFinish)
{
    auto deThread = getReadLockedResource();
    if(!deThread->isThreadRunning())
    {
        onFinish();
        return 0;
    }
    else
    {
        static CallbackID newID = 0;
        newID++;
        // I sincerely doubt that this will ever assign enough CallbackIDs for
        // newID to overflow back to zero, but it doesn't hurt to check.
        if(newID == 0)
        {
            newID++;
        }
        deThread->onFinish[newID] = onFinish;
        return newID;
    }
}


/*
 * Removes an onFinish callback functions that is set to run when the thread 
 * finishes loading.
 */
void DesktopEntryLoader::clearCallback(CallbackID callbackID)
{
    using namespace juce;
    if(callbackID != 0)
    {
        auto deThread = getWriteLockedResource();
        deThread->onFinish.erase(callbackID);
    }
}
