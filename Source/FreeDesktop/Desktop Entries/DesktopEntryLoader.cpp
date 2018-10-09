#include <atomic>
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "Utils.h"
#include "Localized.h"
#include "ThreadResource.h"
#include "XDGDirectories.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryLoader.h"

/* SharedResource object class key */ 
static const juce::Identifier desktopEntryThreadKey("DesktopEntryThread");

/* The desktop entry subdirectory within the data directory: */
static const constexpr char* entryDirectory = "/applications/";

/* Name of the desktop entry category holding every entry: */
static const constexpr char* everyEntryCategory = "All";

/* Name of the desktop entry category holding uncategorized entries: */
static const constexpr char* miscEntryCategory = "Other";

/* Localized text keys: */
static const constexpr char* reading_entry = "reading_entry";

/**
 * @brief  Asynchronously loads and caches desktop entry data.
 */
class DesktopEntryThread : public ThreadResource, private Localized
{
    /* Only DesktopEntryLoader has direct access. */
    friend class DesktopEntryLoader;
    
    /* Maps desktop ID strings to unloaded desktop entry files. */
    std::map<juce::String, juce::File> pendingEntryFiles;

    /* Maps desktop ID strings to desktop entry objects. */
    std::map<juce::String, DesktopEntry> entries;

    /* Maps category names to lists of desktop file Ids. */
    std::map<juce::String, juce::StringArray> categories;

    /* Callback function to send loading progress update strings. */
    std::function<void(juce::String) > notifyCallback;
    
    /* Callback to run when desktop entries finish loading. */
    std::function<void() > onFinish;

    DesktopEntryThread() : ThreadResource(desktopEntryThreadKey),
    Localized("DesktopEntryThread") { }

public:
    virtual ~DesktopEntryThread() { }

private:
    /**
     * @brief  Find a desktop entry file using its desktop file ID.
     *
     * @param entryFileID  The desktop file ID of a desktop entry.
     *
     * @return             The first matching file found within application data
     *                     directories, or an empty file object if no matching
     *                     file is found.
     */
    juce::File findEntryFile(const juce::String& entryFileID)
    {
        using namespace juce;
        StringArray dirs = XDGDirectories::getDataSearchPaths();
        for(const String& dir : dirs)
        {
            File entryFile(dir + entryDirectory + entryFileID + ".desktop");
            if(entryFile.existsAsFile())
            {
                return entryFile;
            }
        }
        return File();
    }

    /**
     * @brief  Discards any existing entry data and asynchronously reloads all
     *         desktop entries from the file system.
     * 
     * @param notifyCallback  An optional callback function to send loading
     *                        progress update strings.  This will be called 
     *                        asynchronously on the message thread.
     *
     * @param onFinish        An optional callback function to run on 
     *                        completion.  This will be called asynchronously on
     *                        the message thread.
     */
    void loadEntries(
            std::function<void(juce::String) > notifyCallback,
            std::function<void() > onFinish)
    {
        using namespace juce;
        if (!isThreadRunning())
        {
            entries.clear();
            this->notifyCallback = notifyCallback;
            this->onFinish = onFinish;
            startThread();
        }
    }

    /**
     * @brief  Finds all desktop entry files within the application data
     *         directories, ignoring all files with duplicate desktop file IDs.
     *
     * This function runs once on the desktop entry thread when it first starts
     * running.
     */
    virtual void init() override
    {
        using namespace juce;
        StringArray dirs = XDGDirectories::getDataSearchPaths();
        for(const String& dir : dirs)
        {
            File directory(dir + entryDirectory);
            if(directory.isDirectory())
            {
                Array<File> entryFiles = directory.findChildFiles
                    (File::findFiles, true, "*.directory;*.desktop");
                for (File& file : entryFiles)
                {
                    String desktopID = file.getRelativePathFrom(directory);
                    if(!pendingEntryFiles.count(desktopID))
                    {
                        pendingEntryFiles[desktopID] = file;
                    }
                }
            }
        }
        DBG("DesktopEntryThread::" << __func__ << ": Found " 
                << String(pendingEntryFiles.size()) 
                << " unique desktop entry files in "
                << dirs.size() << " data directories.");
    }

    /**
     * @brief  Loads a DesktopEntry object from file data, calling the
     *         notification callback if the object loaded successfully.
     *
     * This function will be called repeatedly on the desktop entry thread until
     * all entries are loaded.
     *
     * @param threadLock  An object used to access the desktop entry thread's
     *                    SharedResource lock.
     */
    virtual void runLoop(ThreadResource::ThreadLock& threadLock) override
    {
        using namespace juce;
        threadLock.takeWriteLock();
        if(pendingEntryFiles.empty())
        {
            signalThreadShouldExit();
            threadLock.releaseLock();
            return;
        }
        File entryFile = pendingEntryFiles.begin()->second;
        pendingEntryFiles.erase(pendingEntryFiles.begin());;   
        try
        {
            DesktopEntry entry(entryFile);
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
                    categories[category].add(entry.getDesktopFileId());
                }
                entries[entry.getDesktopFileId()] = entry;
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
        int entriesRead = entries.size();
        int entriesPending = pendingEntryFiles.size();
        String notifyText = localeText(reading_entry) + String(entriesRead)
            + "/" + String(entriesRead + entriesPending);
        MessageManager::callAsync(buildAsyncFunction(LockType::write,
            [this, notifyText]
            {
                notifyCallback(notifyText);
            }));   
        threadLock.releaseLock();
    }

    /**
     * @brief  Runs the loading finished callback function once all desktop
     *         entries are loaded.
     *
     * This function will run once on the desktop entry thread just before the
     * thread stops running.
     */
    void cleanup() override
    {
        using namespace juce;
        DBG("DesktopEntryThread::" << __func__ << ": Loaded "
                << String(entries.size()) << " desktop entries.");
        MessageManager::callAsync(buildAsyncFunction(LockType::read,
                    [this] { onFinish(); }));
    }
};

DesktopEntryLoader::DesktopEntryLoader() : 
ResourceHandler<DesktopEntryThread>(desktopEntryThreadKey,
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
       DesktopEntry entry(entryFile);
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
 * Discards any existing entry data and asynchronously reloads all desktop 
 * entries from the file system.
 */
void DesktopEntryLoader::loadEntries(
        std::function<void(juce::String) > notifyCallback,
        std::function<void() > onFinish)
{
    auto deThread = getWriteLockedResource();
    if (!deThread->isThreadRunning())
    {
        deThread->entries.clear();
        deThread->categories.clear();
        deThread->notifyCallback = notifyCallback;
        deThread->onFinish = onFinish;
        deThread->startThread();
    }
}

/*
 * Removes the notifyCallback and onFinish callback functions that are currently
 * set to run when the thread finishes loading. 
 */
void DesktopEntryLoader::clearCallbacks()
{
    using namespace juce;
    auto deThread = getWriteLockedResource();
    deThread->notifyCallback = [](String s){};
    deThread->onFinish = [](){};
}
