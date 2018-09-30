#include <atomic>
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "Utils.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryLoader.h"

DesktopEntryLoader::DesktopEntryLoader() : Thread("DesktopEntryLoader") { }

DesktopEntryLoader::~DesktopEntryLoader()
{
    if (isThreadRunning())
    {
        stopThread(1000);
    }
}

/*
 * return the number of stored DesktopEntry objects 
 */
int DesktopEntryLoader::size()
{
    using namespace juce;
    const ScopedLock readLock(lock);
    return entries.size();
}

/*
 * Get a list of all DesktopEntry objects within several categories
 */
std::set<DesktopEntry> DesktopEntryLoader::getCategoryListEntries
(juce::StringArray categoryList)
{
    using namespace juce;
    const ScopedLock readLock(lock);
    std::set<DesktopEntry> categoryEntries;
    for (String& category : categoryList)
    {
        std::set<DesktopEntry> catEntries = getCategoryEntries(category);
        for (DesktopEntry entry : catEntries)
        {
            categoryEntries.insert(entry);
        }
    }
    return categoryEntries;
}

/*
 * Get all DesktopEntryLoader with a given category name
 */
std::set<DesktopEntry> DesktopEntryLoader::getCategoryEntries
(juce::String category)
{
    using namespace juce;
    const ScopedLock readLock(lock);
    return categories[category];
}

/*
 * Get the list of all categories found in all desktop entries. 
 */
std::set<juce::String> DesktopEntryLoader::getCategories()
{
    using namespace juce;
    const ScopedLock readLock(lock);
    std::set<String> categoryNames;
    for (auto iter = categories.begin(); iter != categories.end(); iter++)
    {
        categoryNames.insert(iter->first);
    }
    return categoryNames;
}

/*
 * Discards any existing entry data and reloads all desktop entries
 * from the file system.
 */
void DesktopEntryLoader::loadEntries(
        std::function<void(juce::String) > notifyCallback,
        std::function<void() > onFinish)
{
    using namespace juce;
    if (!isThreadRunning())
    {
        {
            const ScopedTryLock readLock(lock);
            if (!readLock.isLocked())
            {
                DBG("DesktopEntryLoader::" << __func__
                        << ": Can't load desktop entries, "
                        << "thread is already locked");
                return;
            }
            entries.clear();
            categories.clear();
            this->notifyCallback = notifyCallback;
            this->onFinish = onFinish;
        }
        startThread();
    }
}

/*
 * If entries are currently loading asynchronously, this will signal for
 * them to stop. Unless loading finishes on its own before this has a chance to
 * stop it, the onFinish callback to loadEntries will not be called.
 */
void DesktopEntryLoader::clearCallbacks()
{
    using namespace juce;
    const ScopedLock loadingLock(lock);
    notifyCallback = [](String s)
    {
    };
    onFinish = []()
    {
    };
}

void DesktopEntryLoader::run()
{
    using namespace juce;
    const ScopedLock loadingLock(lock);
    std::atomic<bool> uiCallPending;
    uiCallPending = false;
    //read the contents of all desktop application directories
    DBG("DesktopEntryLoader::" << __func__ << ": finding desktop entries...");
    std::vector<String> dirs = {
                                "~/.local/share/applications",
                                "/usr/share/applications",
                                "/usr/local/share/applications"
    };
    //track entry names and ignore duplicates
    std::set<String> files;
    Array<File> allEntries;
    for (int i = 0; i < dirs.size(); i++)
    {   
        uiCallPending = true;
        MessageManager::callAsync([&i, &dirs, &uiCallPending, this]
        {
            notifyCallback(String("Scanning application directory ") +
                    String(i + 1) + String("/")
                    + String(dirs.size()));
            uiCallPending = false;
            this->notify();
        });
        while (uiCallPending)
        {
            if (threadShouldExit())
            {
                return;
            }
            const ScopedUnlock waitUnlock(lock);
            wait(1);
        }
        File directory(dirs[i]);
        if (directory.isDirectory())
        {
            Array<File> entries = directory.findChildFiles(File::findFiles,
                    true, "*.directory;*.desktop");
            for (File& file : entries)
            {
                if (files.insert(file.getFileName()).second)
                {
                    allEntries.add(file);
                }
            }
        }
    }
    int fileIndex = 0;
    //read in files as DesktopEntry objects
    for (File& file : allEntries)
    {
        fileIndex++;
        uiCallPending = true;
        MessageManager::callAsync([&fileIndex, &files, &uiCallPending, this]
        {
            notifyCallback(String("Reading file ") + String(fileIndex) +
                    String("/") + String(files.size()));
            uiCallPending = false;
            this->notify();
        });   
        while (uiCallPending)
        {
            if (threadShouldExit())
            {
                return;
            }
            const ScopedUnlock waitUnlock(lock);
            wait(1);
        }
        try
        {
            DesktopEntry entry(file);
            if (entry.shouldBeDisplayed())
            {
                StringArray entryCategories = entry.getCategories();
                if (entryCategories.isEmpty())
                {
                    entryCategories.add("Other");
                }
                entryCategories.add("All");
                for (const String& category : entryCategories)
                {
                    categories[category].insert(entry);
                }
                entries.insert(entry);
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

    }
    DBG("DesktopEntryLoader::" << __func__ << ": "
                << entries.size() << "/" << allEntries.size()
                << " loaded successfully.");

    MessageManager::callAsync([&uiCallPending, this]
    {
        const ScopedLock loadingLock(lock);
        notifyCallback("Finished loading applications.");        
        onFinish();
    });
}
