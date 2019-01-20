#define DESKTOP_ENTRY_IMPLEMENTATION
#include "DesktopEntry_Loader.h"
#include "DesktopEntry_LoadingThread.h"
#include "SharedResource_ThreadResource.h"
#include "Locale/TextUser.h"
#include "XDGDirectories.h"
#include "Utils.h"
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>


DesktopEntry::Loader::Loader() : 
SharedResource::Handler<LoadingThread>() { }

/*
 * Finds a single EntryFile object using its desktop file ID.
 */
DesktopEntry::EntryFile DesktopEntry::Loader::getDesktopEntry
(const juce::String& entryFileID) const
{
    SharedResource::LockedPtr<const LoadingThread> loadingThread 
        = getReadLockedResource();
    return loadingThread->getDesktopEntry(entryFileID);
}

/*
 * Gets all loaded desktop entry file objects.
 */
juce::Array<DesktopEntry::EntryFile> 
DesktopEntry::Loader::getAllEntries() const
{
    SharedResource::LockedPtr<const LoadingThread> loadingThread 
        = getReadLockedResource();
    return loadingThread->getAllEntries();
}

/*
 * Gets all EntryFile objects in a single category.
 */
juce::Array<DesktopEntry::EntryFile> DesktopEntry::Loader::getCategoryEntries
(const juce::String& category) const
{
    SharedResource::LockedPtr<const LoadingThread> loadingThread 
        = getReadLockedResource();
    return loadingThread->getCategoryEntries(category);
}

/*
 * Finds all EntryFile objects within several categories.
 */
juce::Array<DesktopEntry::EntryFile> DesktopEntry::Loader::getCategoryEntries
(const juce::StringArray& categoryList) const
{
    SharedResource::LockedPtr<const LoadingThread> loadingThread 
        = getReadLockedResource();
    return loadingThread->getCategoryEntries(categoryList);
}

/*
 * Scans all desktop entry files for any changes made since the last time the 
 * DesktopEntry::Loader read the entry files.
 */
void DesktopEntry::Loader::scanForChanges()
{
   SharedResource::LockedPtr<LoadingThread> loadingThread 
       = getWriteLockedResource();
   if(!loadingThread->isThreadRunning())
   {
       loadingThread->startThreadResource();
   }
   else if(loadingThread->isFinishedLoading())
   {
       loadingThread->notify();
   }
}

/*
 * Schedules an action to run once all entries have been loaded.
 */
DesktopEntry::CallbackID DesktopEntry::Loader::waitUntilLoaded
(std::function<void()> onFinish)
{
    SharedResource::LockedPtr<LoadingThread> loadingThread 
        = getWriteLockedResource();
    if(loadingThread->isFinishedLoading())
    {
        onFinish();
        return 0;
    }
    CallbackID callbackID = loadingThread->addLoadingCallback(onFinish);
    if(loadingThread->isThreadRunning())
    {
        loadingThread->notify();
    }
    else
    {
        loadingThread->startThreadResource();
    }
    return callbackID;
}


/*
 * Removes an onFinish callback functions that is set to run when the thread 
 * finishes loading.
 */
void DesktopEntry::Loader::clearCallback(CallbackID callbackID)
{
    if(callbackID != 0)
    {
        SharedResource::LockedPtr<LoadingThread> loadingThread
            = getWriteLockedResource();
        loadingThread->cancelCallback(callbackID);
    }
}
