#include <atomic>
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "Utils.h"
#include "Locale/TextUser.h"
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

/* Desktop entry file extension: */
static const constexpr char* fileExtension = ".desktop";

/* Localized text keys: */
static const constexpr char* reading_entry = "reading_entry";

/**
 * @brief  Asynchronously loads and caches desktop entry data.
 */
class DesktopEntryThread : public ThreadResource, public Locale::TextUser
{
private:
    /* Only DesktopEntryLoader has direct access. */
    friend class DesktopEntryLoader;

    /* The last time desktop entry files were scanned. */
    juce::Time lastScanTime;
    
    /* Maps desktop ID strings to desktop entry files. */
    std::map<juce::String, juce::File> entryFiles;

    /* All <Desktop File ID, Desktop File> pairs waiting to be loaded. */
    juce::Array<std::pair<juce::String, juce::File>> pendingFiles;

    /* Maps desktop ID strings to desktop entry objects. */
    std::map<juce::String, DesktopEntry> entries;

    /* Maps category names to lists of desktop file Ids. */
    std::map<juce::String, juce::StringArray> categories;

    /* Callbacks to run when desktop entries finish loading. */
    std::map<DesktopEntryLoader::CallbackID, std::function<void()>> onFinish;

    DesktopEntryThread() : ThreadResource(desktopEntryThreadKey),
    Locale::TextUser(desktopEntryThreadKey) 
    {
        startThread();
    }

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
            File entryFile(dir + entryDirectory + entryFileID + fileExtension);
            if(entryFile.existsAsFile())
            {
                return entryFile;
            }
        }
        return File();
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
        std::map<String, File> oldFiles = entryFiles;
        entryFiles.clear();
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
                        if(file.getLastModificationTime() > lastScanTime
                                || file != oldFiles[desktopID])
                        {
                            pendingFiles.add({desktopID, file});
                        }
                    }
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

    /**
     * @brief  Loads a DesktopEntry object from file data.
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
        threadLock.enterWrite();
        if(pendingFiles.isEmpty())
        {
            signalThreadShouldExit();
            threadLock.exitWrite();
            return;
        }
        String entryID = pendingFiles.getReference(0).first;
        File entryFile = pendingFiles.getReference(0).second;
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
        MessageManager::callAsync(buildAsyncFunction(LockType::read, [this] 
        {
            for(const auto& callback : onFinish)
            {
                callback.second();
            }
            onFinish.clear();
        }));
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
(const std::function<void(const juce::StringArray)> handleChanges)
{
    // TODO: Implement a system for properly recording and sharing all changes.
    DBG("DesktopEntryLoader::" << __func__
            << ": Scanning for changes is not implemented yet!");
    return 0;
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
