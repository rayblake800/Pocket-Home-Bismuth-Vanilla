#define DESKTOP_ENTRY_IMPLEMENTATION
#include "DesktopEntry_LoadingThread.h"
#include "DesktopEntry_FileError.h"
#include "DesktopEntry_FormatError.h"
#include "DesktopEntry_UpdateInterface.h"
#include "SharedResource_Thread_ScopedWriteLock.h"
#include "Util/Files/XDGDirectories.h"

/* SharedResource object class key: */ 
const juce::Identifier DesktopEntry::LoadingThread::resourceKey
    = "DesktopEntry_LoadingThread";

/* Resource thread name: */
static const juce::String threadName = "DesktopEntry_LoadingThread";

/* The desktop entry subdirectory within the data directory: */
static const constexpr char* entryDirectory = "/applications/";

/* Name of the desktop entry category holding every entry: */
static const constexpr char* everyEntryCategory = "All";

/* Name of the desktop entry category holding uncategorized entries: */
static const constexpr char* miscEntryCategory = "Other";

/* Desktop entry file extension: */
static const constexpr char* fileExtension = ".desktop";

/*
 * Creates and starts the thread resource.
 */
DesktopEntry::LoadingThread::LoadingThread() : 
SharedResource::Thread::Resource(resourceKey, ::threadName)
{
    startResourceThread();
}

/*
 * Finds a desktop entry from its desktop file ID. 
 */
DesktopEntry::EntryFile DesktopEntry::LoadingThread::getDesktopEntry
    (const juce::String entryFileID) const
{
    try
    {
        return entries.at(entryFileID);
    }
    catch(std::out_of_range e)
    {
        return EntryFile();
    }
}


/*
 * Gets all loaded desktop entry file objects.
 */
juce::Array<DesktopEntry::EntryFile> 
DesktopEntry::LoadingThread::getAllEntries() const
{
    juce::Array<EntryFile> entryList;
    for(const auto& entryIter : entries)
    {
        entryList.add(entryIter.second);
    }
    return entryList;
}

/*
 * Gets all desktop entries with a specific application category.
 */
juce::Array<DesktopEntry::EntryFile> 
DesktopEntry::LoadingThread::getCategoryEntries
(const juce::String category) const
{
    try
    {
        juce::Array<EntryFile> entryList;
        for(const juce::String& entryID : categories.at(category))
        {
            try
            {
                entryList.add(entries.at(entryID));
            }
            catch(std::out_of_range e)
            {
                DBG("DesktopEntry::LoadingThread::" << __func__ 
                        << ": Failed to find entry with ID " << entryID);
            }
        }
        return entryList;
    }
    catch(std::out_of_range e)
    {
        return juce::Array<EntryFile>();
    }
}

/*
 * Gets all desktop entries with one or more specific application categories.
 */
juce::Array<DesktopEntry::EntryFile> 
DesktopEntry::LoadingThread::getCategoryEntries
(const juce::StringArray categoryList) const
{
    juce::StringArray entryIDs;
    for(const juce::String& category : categoryList)
    {
        if(categories.count(category))
        {
            entryIDs.addArray(categories.at(category));
        }
    }
    entryIDs.removeDuplicates(false);
    juce::Array<EntryFile> entryList;
    for(const juce::String& entryID : entryIDs)
    {
        try
        {
            entryList.add(entries.at(entryID));
        }
        catch(std::out_of_range e)
        {
            DBG("DesktopEntry::LoadingThread::" << __func__ 
                    << ": Failed to find entry with ID " << entryID);
        }
    }
    return entryList;
}

/*
 * Adds a callback function to run once when the thread finishes loading or 
 * updating desktop entries.
 */
DesktopEntry::CallbackID DesktopEntry::LoadingThread::addLoadingCallback
(const std::function<void()> loadingCallback)
{
    if(loadingCallback) // Ignore invalid callback functions
    {
        const CallbackID callbackID = generateCallbackID();
        onFinish[callbackID] = loadingCallback;
        return callbackID;
    }
    return 0;
}


/*
 * Cancels a pending callback ID.
 */
void DesktopEntry::LoadingThread::cancelCallback(const CallbackID toCancel)
{
    onFinish.erase(toCancel);
}

/*
 * Finds all relevant changes to the set of loaded desktop entry files, and 
 * prepares to fully update desktop entry data.
 */
void DesktopEntry::LoadingThread::findUpdatedFiles()
{
    using juce::String;
    using juce::File;
    juce::StringArray dirs = XDGDirectories::getDataSearchPaths();
    std::map<String, File> oldFiles = entryFiles;
    entryFiles.clear();
    for(const String& dir : dirs)
    {
        File directory(dir + entryDirectory);
        if(directory.isDirectory())
        {
            juce::Array<File> desktopFiles = directory.findChildFiles
                (File::findFiles, true, "*.desktop");
            for (File& file : desktopFiles)
            {
                String desktopID = file.getRelativePathFrom(directory);
                if(entryFiles.count(desktopID) == 0 
                        && pendingFiles.count(desktopID) == 0)
                {
                    entryFiles[desktopID] = file;
                    /* Skip updates to files that have not changed since the
                       last scan: */
                    if(file.getLastModificationTime() > lastScanTime
                            || file != oldFiles[desktopID])
                    {
                        pendingFiles[desktopID] = file;
                        if(!oldFiles.count(desktopID))
                        {
                            lastAddedIDs.add(desktopID);
                        }
                        else
                        {
                            lastChangedIDs.add(desktopID);
                        }
                    }
                }
            }
        }
    }
    if(!oldFiles.empty()) // Look for removed entries
    {
        for(auto iter : oldFiles)
        {
            String desktopID = iter.first;
            if(!entryFiles.count(desktopID))
            {
                lastRemovedIDs.add(desktopID);
            }
        }
    }
    lastScanTime = juce::Time::getCurrentTime();
    DBG("DesktopEntry::LoadingThread::" << __func__ << ": Found " 
            << String(entryFiles.size()) 
            << " unique desktop entry files in "
            << dirs.size() << " data directories.");
    DBG("DesktopEntry::LoadingThread::" << __func__ << ": Reloading "
            << ((int) pendingFiles.size()) << " entry files.");
}


/*
 * Checks if the thread has finished loading desktop entry files, and is either 
 * running cleanup or waiting for another request.
 */
bool DesktopEntry::LoadingThread::isFinishedLoading()
{
    return finishedLoading;
}

/*
 * Finds all unloaded or updated desktop entry files within the application data
 * directories, ignoring files with duplicate desktop file IDs.
 */
void DesktopEntry::LoadingThread::init(SharedResource::Thread::Lock& threadLock) 
{
    const SharedResource::Thread::ScopedWriteLock writeLock(threadLock);
    finishedLoading = false;
    lastAddedIDs.clear();
    lastChangedIDs.clear();
    lastRemovedIDs.clear();
    findUpdatedFiles();
}

/*
 * Loads or updates a single desktop entry file in the list of pending files.
 */
void DesktopEntry::LoadingThread::runLoop
(SharedResource::Thread::Lock& threadLock) 
{
    using juce::String;
    threadLock.enterWrite();
    if(pendingFiles.empty())
    {
        signalThreadShouldExit();
        threadLock.exitWrite();
        return;
    }
    String entryID = pendingFiles.begin()->first;
    juce::File entryFile = pendingFiles.begin()->second;
    pendingFiles.erase(pendingFiles.begin());
    try
    {
        EntryFile entry(entryFile, entryID);
        if (entry.shouldBeDisplayed())
        {
            juce::StringArray entryCategories = entry.getCategories();
            if (entryCategories.isEmpty())
            {
                // Categorize as "Other"
                entryCategories.add(miscEntryCategory);
            }
            // Add to list of all entries
            entryCategories.add(everyEntryCategory);
            for(const String& category : entryCategories)
            {
                categories[category].add(entryID);
            }
            entries[entryID] = entry;
        }
        /* If an updated entry is hidden, mark it as removed in the change 
           list.  If a new entry was hidden, don't mention in in the change
           list at all. */
        else
        {
            if(entries.count(entryID) > 0)
            {
                lastChangedIDs.removeString(entryID);
                lastRemovedIDs.add(entryID);
                entries.erase(entryID);
            }
            else
            {
                lastAddedIDs.removeString(entryID);
            }
        }
    }
    catch(FileError e)
    {
        DBG("DesktopEntry::LoadingThread::" << __func__ << ": File error: "
                << e.what());
    }
    catch(FormatError e)
    {
        DBG("DesktopEntry::LoadingThread::" << __func__ << ": Format error: "
                << e.what());
    }
    threadLock.exitWrite();
}

/*
 * Runs loading and update callback functions once all desktop entries are 
 * loaded or updated.
 */
void DesktopEntry::LoadingThread::cleanup
(SharedResource::Thread::Lock& threadLock) 
{
    const SharedResource::Thread::ScopedWriteLock writeLock(threadLock);
    finishedLoading = true;
    DBG("DesktopEntry::LoadingThread::" << __func__ << ": "
            << lastAddedIDs.size() << " added, "
            << lastChangedIDs.size() << " updated, "
            << lastRemovedIDs.size() << " removed.");
    juce::MessageManager::callAsync(buildAsyncFunction(
                SharedResource::LockType::read, [this] 
    {
        for(const auto& callback : onFinish)
        {
            callback.second();
        }
        onFinish.clear();

        foreachHandler<UpdateInterface>([this](UpdateInterface* updateListener)
        {
        if(!lastAddedIDs.isEmpty())
        {
                updateListener->entriesAdded(lastAddedIDs);
            }
            if(!lastChangedIDs.isEmpty())
            {
                updateListener->entriesUpdated(lastChangedIDs);
            }
            if(!lastRemovedIDs.isEmpty())
            {
                updateListener->entriesRemoved(lastRemovedIDs);
            }
        });
    }));
}


/*
 * Makes the thread sleep after loading or updating all desktop files.
 */
bool DesktopEntry::LoadingThread::threadShouldWait()
{
    return pendingFiles.empty();
}

/*
 * Generates a unique callback ID the thread can assign to a callback function.
 */
DesktopEntry::CallbackID DesktopEntry::LoadingThread::generateCallbackID() const
{
    static CallbackID nextID = 0;
    nextID++;
    // I really doubt this will ever assign enough callback IDs to overflow to
    // zero, but it's better to check anyway.
    if(nextID == 0)
    {
        nextID++;
    }
    return nextID;
}

/*
 * Finds a desktop entry file using its desktop file ID.
 */
juce::File DesktopEntry::LoadingThread::findEntryFile
(const juce::String entryFileID)
{
    juce::StringArray dirs = XDGDirectories::getDataSearchPaths();
    for(const juce::String& dir : dirs)
    {
        juce::File entryFile(dir + entryDirectory + entryFileID + fileExtension);
        if(entryFile.existsAsFile())
        {
            return entryFile;
        }
    }
    return juce::File();
}
