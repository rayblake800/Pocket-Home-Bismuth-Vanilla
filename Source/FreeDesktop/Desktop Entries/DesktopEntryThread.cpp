#define DESKTOP_ENTRY_LOADER_IMPLEMENTATION
#include "XDGDirectories.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryThread.h"

/* SharedResource object class key */ 
const juce::Identifier DesktopEntryThread::resourceKey
    = "DesktopEntryThread";

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
DesktopEntryThread::DesktopEntryThread() : ThreadResource(resourceKey)
{
    startThread();
}

/*
 * Finds a desktop entry from its desktop file ID. 
 */
DesktopEntry DesktopEntryThread::getDesktopEntry
    (const juce::String entryFileID) const
{
    try
    {
        return entries.at(entryFileID);
    }
    catch(std::out_of_range e)
    {
        return DesktopEntry();
    }
}

/*
 * Gets all desktop entries with a specific application category.
 */
juce::Array<DesktopEntry> DesktopEntryThread::getCategoryEntries
(const juce::String category) const
{
    try
    {
        juce::Array<DesktopEntry> entryList;
        for(const juce::String& entryID : categories.at(category))
        {
            try
            {
                entryList.add(entries.at(entryID));
            }
            catch(std::out_of_range e)
            {
                DBG("DesktopEntryThread::" << __func__ 
                        << ": Failed to find entry with ID " << entryID);
            }
        }
        return entryList;
    }
    catch(std::out_of_range e)
    {
        return juce::Array<DesktopEntry>();
    }
}

/*
 * Gets all desktop entries with one or more specific application categories.
 */
juce::Array<DesktopEntry> DesktopEntryThread::getCategoryEntries
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
    juce::Array<DesktopEntry> entryList;
    for(const juce::String& entryID : entryIDs)
    {
        try
        {
            entryList.add(entries.at(entryID));
        }
        catch(std::out_of_range e)
        {
            DBG("DesktopEntryThread::" << __func__ 
                    << ": Failed to find entry with ID " << entryID);
        }
    }
    return entryList;
}

/*
 * Adds a callback function to run once when the thread finishes loading or 
 * updating desktop entries.
 */
DesktopEntryThread::CallbackID DesktopEntryThread::addLoadingCallback
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
 * Adds a callback function to run once when the thread finishes updating 
 * desktop entries.
 */
DesktopEntryThread::CallbackID DesktopEntryThread::requestUpdate
(const std::function<void(const ChangeMap)> updateCallback)
{
    if(updateCallback) // Ignore invalid callback functions
    {
        const CallbackID callbackID = generateCallbackID();
        onUpdate[callbackID] = updateCallback;
        return callbackID;
    }
    return 0;
}

/*
 * Cancels a pending callback ID.
 */
void DesktopEntryThread::cancelCallback(const CallbackID toCancel)
{
    onFinish.erase(toCancel);
    onUpdate.erase(toCancel);
}

/*
 * Finds all desktop entry files within the application data directories, 
 * ignoring all files with duplicate desktop file IDs.
 */
void DesktopEntryThread::init() 
{
    using namespace juce;
    StringArray dirs = XDGDirectories::getDataSearchPaths();
    std::map<String, File> oldFiles = entryFiles;
    entryFiles.clear();
    latestChanges.clear();
    for(const String& dir : dirs)
    {
        File directory(dir + entryDirectory);
        if(directory.isDirectory())
        {
            Array<File> desktopFiles = directory.findChildFiles
                (File::findFiles, true, "*.desktop");
            for (File& file : desktopFiles)
            {
                String desktopID = file.getRelativePathFrom(directory);
                if(!entryFiles.count(desktopID))
                {
                    entryFiles[desktopID] = file;
                    /* Skip updates to files that have not changed since the
                       last scan: */
                    if(file.getLastModificationTime() > lastScanTime
                            || file != oldFiles[desktopID])
                    {
                        pendingFiles.add({desktopID, file});
                        if(!oldFiles.count(desktopID))
                        {
                            latestChanges[desktopID] 
                                = EntryChange::entryCreated;
                        }
                        else
                        {
                            latestChanges[desktopID]
                                = EntryChange::entryChanged;
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
                latestChanges[desktopID] = EntryChange::entryRemoved;
                entries.erase(desktopID);
            }
        }
    }
    DBG("DesktopEntryThread::" << __func__ << ": Found " 
            << String(entryFiles.size()) 
            << " unique desktop entry files in "
            << dirs.size() << " data directories.");
    DBG("DesktopEntryThread::" << __func__ << ": Reloading "
            << pendingFiles.size() << " entry files.");
}

/*
 * Loads a DesktopEntry object from file data.
 */
void DesktopEntryThread::runLoop(ThreadResource::ThreadLock& threadLock) 
{
    using namespace juce;
    threadLock.enterWrite();
    if(pendingFiles.isEmpty())
    {
        signalThreadShouldExit();
        threadLock.exitWrite();
        return;
    }
    String entryID = pendingFiles.getReference(0).first;
    juce::File entryFile = pendingFiles.getReference(0).second;
    pendingFiles.remove(0);   
    try
    {
        DesktopEntry entry(entryFile, entryID);
        if (entry.shouldBeDisplayed())
        {
            StringArray entryCategories = entry.getCategories();
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
            if(entries.count(entryID))
            {
                latestChanges[entryID] = EntryChange::entryRemoved;
                entries.erase(entryID);
            }
            else
            {
                latestChanges.erase(entryID);
            }
        }
    }
    catch(DesktopEntryFileError e)
    {
        DBG("DesktopEntryLoader::" << __func__ << ": File error: "
                << e.what());
    }
    catch(DesktopEntryFormatError e)
    {
        DBG("DesktopEntryLoader::" << __func__ << ": Format error: "
                << e.what());
    }
    threadLock.exitWrite();
}

/*
 * Runs loading and update callback functions once all desktop entries are 
 * loaded or updated.
 */
void DesktopEntryThread::cleanup() 
{
    DBG("DesktopEntryThread::" << __func__ << ": Loaded "
            << juce::String(entries.size()) << " desktop entries.");
    juce::MessageManager::callAsync(buildAsyncFunction(LockType::read, [this] 
    {
        for(const auto& callback : onFinish)
        {
            callback.second();
        }
        onFinish.clear();
        for(const auto& callback : onUpdate)
        {
            callback.second(latestChanges);
        }
        onUpdate.clear();
    }));
}

/*
 * Generates a unique callback ID the thread can assign to a callback function.
 */
DesktopEntryThread::CallbackID DesktopEntryThread::generateCallbackID() const
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
juce::File DesktopEntryThread::findEntryFile(const juce::String entryFileID)
{
    using namespace juce;
    StringArray dirs = XDGDirectories::getDataSearchPaths();
    for(const String& dir : dirs)
    {
        File entryFile(dir + entryDirectory + entryFileID + fileExtension);
        if(entryFile.existsAsFile())
        {
            return entryFile;
        }
    }
    return File();
}



