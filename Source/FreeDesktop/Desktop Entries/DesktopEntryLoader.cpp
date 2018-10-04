#include <atomic>
#include "Utils.h"
#include "Localized.h"
#include "ThreadResource.h"
#include "XDGDirectories.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryLoader.h"

/* SharedResource object class key. */ 
static const juce::Identifier desktopEntryThreadKey("DesktopEntryThread");

/* The desktop entry subdirectory within the data directory. */
static const constexpr char* entryDirectory = "/applications/";

/* Localized text keys: */
static const constexpr char* reading_entry = "reading_entry";

/**
 * @brief  Asynchronously loads and caches desktop entry data.
 */
class DesktopEntryThread : public ThreadResource, private Localized
{
    /* Only DesktopEntryLoader has direct access. */
    friend class DesktopEntryLoader;
    
    /* The list of all entries. */
    std::set<DesktopEntry> entries;
    
    /* Tracks unloaded desktop entry files, mapping entry desktop IDs to their
       juce file objects. */
    std::map<juce::String, juce::File> pendingEntryFiles;

    /* Map of category names to lists of entries. */
    std::map<juce::String, std::set<DesktopEntry>> categories;
    
    /* Callback function to send loading progress update strings. */
    std::function<void(juce::String) > notifyCallback;
    
    /* Callback to run when desktop entries finish loading. */
    std::function<void() > onFinish;

    DesktopEntryThread() : ThreadResource(desktopEntryThreadKey),
    Localized("DesktopEntryThread") { }

public:
    virtual ~DesktopEntryThread() { }

private:
    void loadEntries(
            std::function<void(juce::String) > notifyCallback,
            std::function<void() > onFinish)
    {
        using namespace juce;
        if (!isThreadRunning())
        {
            entries.clear();
            categories.clear();
            this->notifyCallback = notifyCallback;
            this->onFinish = onFinish;
            startThread();
        }
    }

    virtual void init() override
    {
        using namespace juce;
        StringArray dirs = XDGDirectories::getDataSearchPaths();
        for(const String& dir : dirs)
        {
            File directory(dir + entryDirectory);
            if(directory.isDirectory())
            {
                Array<File> entries = directory.findChildFiles(File::findFiles,
                        true, "*.directory;*.desktop");
                for (File& file : entries)
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
                << pendingEntryFiles.size() << " unique desktop entry files in "
                << dirs.size() << " data directories.");
    }

    virtual void runLoop(ThreadResource::ThreadLock& threadLock) override
    {
        using namespace juce;
        threadLock.takeWriteLock();
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
        int entriesRead = entries.size();
        int entriesPending = pendingEntryFiles.size();
        String notifyText = localeText(reading_entry) + String(entriesRead)
            + "/" + String(entriesRead + entriesPending);
        MessageManager::callAsync(buildAsyncFunction(LockType::write,
            [this, notifyText]
            {
                notifyCallback(notifyText);
            }));   
    }

    void cleanup() override
    {
        using namespace juce;
        DBG("DesktopEntryThread::" << __func__ << ": Loaded"
                << entries.size() << " desktop entries.");
        MessageManager::callAsync(buildAsyncFunction(LockType::read,
                    [this] { onFinish(); }));
    }
};

DesktopEntryLoader::DesktopEntryLoader() : 
ResourceHandler<DesktopEntryThread>(desktopEntryThreadKey,
        []()->SharedResource* { return new DesktopEntryThread(); }) { }

DesktopEntryLoader::~DesktopEntryLoader() { }


/*
 * return the number of stored DesktopEntry objects 
 */
int DesktopEntryLoader::size()
{
    auto deThread = getReadLockedResource();
    if(deThread->isThreadRunning())
    {
        return 0;
    }
    return deThread->entries.size();
}

/*
 * Get all DesktopEntryLoader with a given category name
 */
std::set<DesktopEntry> DesktopEntryLoader::getCategoryEntries
(const juce::String& category)
{
    auto deThread = getReadLockedResource();
    if(deThread->isThreadRunning())
    {
        return { };
    }
    return deThread->categories.at(category);
}

/*
 * Get a list of all DesktopEntry objects within several categories
 */
std::set<DesktopEntry> DesktopEntryLoader::getCategoryListEntries
(const juce::StringArray& categoryList)
{
    using namespace juce;
    auto deThread = getReadLockedResource();
    std::set<DesktopEntry> categoryEntries;
    for (const String& category : categoryList)
    {
        std::set<DesktopEntry> catEntries = deThread->categories.at(category);
        for (DesktopEntry entry : catEntries)
        {
            categoryEntries.insert(entry);
        }
    }
    return categoryEntries;
}

/*
 * Get the list of all categories found in all desktop entries. 
 */
std::set<juce::String> DesktopEntryLoader::getCategories()
{
    using namespace juce;
    auto deThread = getReadLockedResource();    
    std::set<String> categoryNames;
    for (auto iter = deThread->categories.begin(); 
              iter != deThread->categories.end(); iter++)
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
 * If entries are currently loading asynchronously, this will signal for
 * them to stop. Unless loading finishes on its own before this has a chance to
 * stop it, the onFinish callback to loadEntries will not be called.
 */
void DesktopEntryLoader::clearCallbacks()
{
    using namespace juce;
    auto deThread = getWriteLockedResource();
    deThread->notifyCallback = [](String s){};
    deThread->onFinish = [](){};
}


