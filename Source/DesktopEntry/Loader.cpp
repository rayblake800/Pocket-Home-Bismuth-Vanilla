#define DESKTOP_ENTRY_LOADER_IMPLEMENTATION
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "Utils.h"
#include "Locale/TextUser.h"
#include "ThreadResource.h"
#include "XDGDirectories.h"
#include "DesktopEntry/LoadingThread.h"
#include "DesktopEntry/Loader.h"


DesktopEntry::Loader::Loader() : 
ResourceHandler<LoadingThread>(LoadingThread::resourceKey,
        []()->SharedResource* { return new LoadingThread(); }) { }

/*
 * Finds a single EntryFile object using its desktop file ID.
 */
DesktopEntry::EntryFile DesktopEntry::Loader::getDesktopEntry
(const juce::String& entryFileID) const
{
    auto loadingThread = getReadLockedResource();
    return loadingThread->getDesktopEntry(entryFileID);
}

/*
 * Gets all EntryFile objects in a single category.
 */
juce::Array<DesktopEntry::EntryFile> DesktopEntry::Loader::getCategoryEntries
(const juce::String& category) const
{
    auto loadingThread = getReadLockedResource();
    return loadingThread->getCategoryEntries(category);
}

/*
 * Finds all EntryFile objects within several categories.
 */
juce::Array<DesktopEntry::EntryFile> DesktopEntry::Loader::getCategoryEntries
(const juce::StringArray& categoryList) const
{
    auto loadingThread = getReadLockedResource();
    return loadingThread->getCategoryEntries(categoryList);
}

/*
 * Scans all desktop entry files for any changes made since the last time the 
 * DesktopEntry::Loader read the entry files.
 */
DesktopEntry::CallbackID DesktopEntry::Loader::scanForChanges
(const std::function<void(std::map<juce::String, UpdateType>)> handleChanges)
{
   auto loadingThread = getWriteLockedResource();
   CallbackID callbackID = loadingThread->addUpdateCallback(handleChanges);
   if(callbackID > 0 && !loadingThread->isThreadRunning())
   {
       loadingThread->startThread();
   }
   return callbackID;
}

/*
 * Schedules an action to run once all entries have been loaded.
 */
DesktopEntry::CallbackID DesktopEntry::Loader::waitUntilLoaded
(std::function<void()> onFinish)
{
    auto loadingThread = getWriteLockedResource();
    if(!loadingThread->isThreadRunning())
    {
        onFinish();
        return 0;
    }
    return loadingThread->addLoadingCallback(onFinish);
}


/*
 * Removes an onFinish callback functions that is set to run when the thread 
 * finishes loading.
 */
void DesktopEntry::Loader::clearCallback(CallbackID callbackID)
{
    if(callbackID != 0)
    {
        auto loadingThread = getWriteLockedResource();
        loadingThread->cancelCallback(callbackID);
    }
}
