#include <atomic>
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "DesktopEntries.h"

DesktopEntries::DesktopEntries() { }

DesktopEntries::DesktopEntries(const DesktopEntries& orig)
{
    const ScopedLock readLock(orig.lock);
    entries = orig.entries;
}

DesktopEntries::~DesktopEntries() { }

//return the number of stored DesktopEntry objects

int DesktopEntries::size()
{

    const ScopedLock readLock(lock);
    return entries.size();
}


//Get a list of all DesktopEntry objects within several categories

std::set<DesktopEntry> DesktopEntries::getCategoryListEntries
(Array<String> categoryList)
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

//Get all DesktopEntries with a given category name

std::set<DesktopEntry> DesktopEntries::getCategoryEntries(String category)
{

    const ScopedLock readLock(lock);
    return categories[category];
}

//Get the list of all categories found in all desktop entries.

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
    if (!loadingThread.isThreadRunning())
    {
        {
            const ScopedTryLock readLock(lock);
            if(!readLock.isLocked()){
                DBG("Can't load desktop entries, thread is already locked");
                return;
            }
            entries.clear();
            categories.clear();
        }
        loadingThread.asyncLoadEntries(this, notifyCallback, onFinish);
    }
}

DesktopEntries::LoadingThread::LoadingThread() : Thread("DesktopEntryLoader") { }

void DesktopEntries::LoadingThread::asyncLoadEntries
(DesktopEntries * threadOwner,
        std::function<void(String) > notifyCallback,
        std::function<void() > onFinish)
{
    this->threadOwner = threadOwner;
    this->notifyCallback = notifyCallback;
    this->onFinish = onFinish;
    startThread();
}

DesktopEntries::LoadingThread::~LoadingThread()
{
    if (isThreadRunning())
    {
        stopThread(1000);
    }
}

void DesktopEntries::LoadingThread::run()
{
    const ScopedLock loadingLock(threadOwner->lock);
    std::atomic<bool> uiCallPending;
    uiCallPending = false;
    //read the contents of all desktop application directories
    DBG("finding desktop entries...");
    std::vector<String> dirs = {
                                getHomePath() + "/.local/share/applications",
                                "/usr/share/applications",
                                "/usr/local/share/applications"
    };
    //track entry names and ignore duplicates
    std::set<String> files;
    std::set<String> paths;
    for (int i = 0; i < dirs.size(); i++)
    {

        while (uiCallPending)
        {
            if (threadShouldExit())
            {
                return;
            }
            wait(-1);
        }
        uiCallPending = true;
        MessageManager::callAsync([&i, &dirs, &uiCallPending, this]
        {
            notifyCallback(String("Scanning application directory ") +
                    String(i + 1) + String("/")
                    + String(dirs.size()));
            uiCallPending = false;
            this->notify();
        });
        std::vector<String> dfiles = listFiles(dirs[i]);
        for (int i2 = 0; i2 < dfiles.size(); i2++)
        {
            if (files.insert(dfiles[i2]).second)
            {
                paths.insert(dirs[i] + "/" + dfiles[i2]);
            }
        }
    }
    int fileIndex = 0;
    //read in files as DesktopEntry objects
    for (std::set<String>::iterator it = paths.begin();
         it != paths.end(); it++)
    {

        fileIndex++;
        while (uiCallPending)
        {
            if (threadShouldExit())
            {
                return;
            }
            wait(-1);
        }
        uiCallPending = true;
        MessageManager::callAsync([&fileIndex, &files, &uiCallPending, this]
        {
            notifyCallback(String("Reading file ") + String(fileIndex) +
                    String("/") + String(files.size()));
            uiCallPending = false;
            this->notify();
        });
        String path = *it;
        String extension = path.fromLastOccurrenceOf(".", false, true);
        if (extension == "desktop" || extension == "directory")
        {
            DesktopEntry entry(path);
            if (entry.getValue(DesktopEntry::hidden) ||
                entry.getValue(DesktopEntry::noDisplay) ||
                entry.getValue(DesktopEntry::notShowIn)
                .contains("pocket-home"))
            {
                continue;
            }
            Array<String> deCategories = entry.getValue(DesktopEntry::categories);
            if (deCategories.isEmpty())
            {
                deCategories.add("Other");
            }
            deCategories.add("All");
            for (String category : deCategories)
            {
                threadOwner->categories[category].insert(entry);
            }
            threadOwner->entries.insert(entry);
        }
    }
    while (uiCallPending)
    {
        if (threadShouldExit())
        {
            return;
        }
        wait(-1);
    }
    uiCallPending = true;
    MessageManager::callAsync([&uiCallPending, this]
    {
        DBG("All desktop entries loaded.");
        onFinish();
        uiCallPending = false;
        this->notify();
    });
}