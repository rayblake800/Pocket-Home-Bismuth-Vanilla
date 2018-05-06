#include <atomic>
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "Utils.h"
#include "DesktopEntries.h"

DesktopEntries::DesktopEntries() : Thread("DesktopEntries") { }

DesktopEntries::~DesktopEntries()
{
    if (isThreadRunning())
    {
        stopThread(1000);
    }
}

/**
 * return the number of stored DesktopEntry objects 
 */
int DesktopEntries::size()
{

    const ScopedLock readLock(lock);
    return entries.size();
}

/**
 * Get a list of all DesktopEntry objects within several categories
 */
std::set<DesktopEntry> DesktopEntries::getCategoryListEntries
(StringArray categoryList)
{

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

/**
 * Get all DesktopEntries with a given category name
 */
std::set<DesktopEntry> DesktopEntries::getCategoryEntries(String category)
{

    const ScopedLock readLock(lock);
    return categories[category];
}

/**
 * Get the list of all categories found in all desktop entries. 
 */
std::set<String> DesktopEntries::getCategories()
{
    const ScopedLock readLock(lock);
    std::set<String> categoryNames;
    for (std::map<String, std::set < DesktopEntry>>::iterator it = categories.begin();
         it != categories.end(); it++)
    {
        categoryNames.insert(it->first);
    }
    return categoryNames;
}

/**
 * Discards any existing entry data and reloads all desktop entries
 * from the file system.
 */
void DesktopEntries::loadEntries
(std::function<void(String) > notifyCallback, std::function<void() > onFinish)
{
    if (!isThreadRunning())
    {
        {
            const ScopedTryLock readLock(lock);
            if (!readLock.isLocked())
            {
                DBG("DesktopEntries::" << __func__
                        << ": Can't load desktop entries, thread is already locked");
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

/**
 * If entries are currently loading asynchronously, this will signal for
 * them to stop. Unless loading finishes on its own before this has a chance to
 * stop it, the onFinish callback to loadEntries will not be called.
 */
void DesktopEntries::clearCallbacks()
{
    const ScopedLock loadingLock(lock);
    notifyCallback = [](String s)
    {
    };
    onFinish = []()
    {
    };
}

void DesktopEntries::run()
{
    ScopedExecTimer threadTimer("Desktop Entry Loading");
    const ScopedLock loadingLock(lock);
    std::atomic<bool> uiCallPending;
    uiCallPending = false;
    //read the contents of all desktop application directories
    DBG("DesktopEntries::" << __func__ << ": finding desktop entries...");
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
            wait(1);
        }
        DesktopEntry entry(file);
        StringArray onlyShowIn = entry.getValue(DesktopEntry::onlyShowIn);
        if (entry.getValue(DesktopEntry::hidden)
            || entry.getValue(DesktopEntry::noDisplay)
            || entry.getValue(DesktopEntry::notShowIn).contains("pocket-home")
            || (!onlyShowIn.isEmpty() && !onlyShowIn.contains("pocket-home")))
        {
            continue;
        }
        
        StringArray deCategories = entry.getValue(DesktopEntry::categories);
        if (deCategories.isEmpty())
        {
            deCategories.add("Other");
        }
        deCategories.add("All");
        for (const String& category : deCategories)
        {
            categories[category].insert(entry);
        }
        entries.insert(entry);

    }
    MessageManager::callAsync([&uiCallPending, this]
    {
    	ScopedExecTimer endTimer("Async OnFinish Callback");
        const ScopedLock loadingLock(lock);
        notifyCallback("Finished loading applications.");
        DBG("DesktopEntries::" << __func__
                << ": All desktop entries loaded.");
        onFinish();
    });
}
