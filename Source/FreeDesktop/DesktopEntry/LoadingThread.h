// Only the DesktopEntryLoader may access the DesktopEntryThread
#ifdef DESKTOP_ENTRY_LOADER_IMPLEMENTATION
#pragma once
#include <map>
#include "DesktopEntry.h"
#include "ThreadResource.h"

/**
 * @file  DesktopEntryThread.h
 *
 * @brief  Loads, caches, and updates desktop entry data.
 */
class DesktopEntryThread : public ThreadResource
{
public:
    /* SharedResource object class key */ 
    static const juce::Identifier resourceKey;

    /**
     * @brief  Creates and starts the thread resource.
     */
    DesktopEntryThread();

    virtual ~DesktopEntryThread() { }

    /**
     * @brief  Finds a desktop entry from its desktop file ID. 
     *
     * @param entryFileID  A valid desktop file ID.
     *
     * @return             The matching desktop entry, or an empty DesktopEntry
     *                     if the entryFileID does not match a stored entry
     *                     object.
     */
    DesktopEntry getDesktopEntry (const juce::String entryFileID) const;

    /**
     * @brief  Gets all desktop entries with a specific application category.
     *
     * @param category  The category to search for in the DesktopEntry list.
     *
     * @return          All loaded entries that have that category.
     */
    juce::Array<DesktopEntry> getCategoryEntries(const juce::String category) 
        const;

    /**
     * @brief  Gets all desktop entries with one or more specific application
     *         categories.
     *
     * @param categoryList  The list of categories to search for in the
     *                      DesktopEntry list.
     *
     * @return              All loaded entries that have at least one of the
     *                      categories in the category list.
     */
    juce::Array<DesktopEntry> getCategoryEntries
        (const juce::StringArray categoryList) const;

    /**
     * @brief  Identifies a callback function that will run after entries are 
     *         loaded or updated.
     *
     * A zero value represents an unset or invalid ID.
     */
    typedef juce::uint32 CallbackID;

    /**
     * @brief  Adds a callback function to run once when the thread finishes 
     *         loading or updating desktop entries.  This will only be added if
     *         the thread is currently running.
     *
     * @param loadingCallback  The callback function to run when the thread
     *                         finishes running.
     *
     * @return                 An ID that can be used to cancel the callback
     *                         function, or zero if the thread is not running
     *                         and the callback wasn't saved.
     */
    CallbackID addLoadingCallback(const std::function<void()> loadingCallback);

    /**
     * @brief  Lists the main types of changes to desktop entry files that the
     *         DesktopEntryThread could find.
     */
    enum class EntryChange
    {
        /* A new .desktop file was discovered: */
        entryCreated,
        /* An existing .desktop file was updated: */
        entryChanged,
        /* An existing .desktop file was deleted or hidden: */
        entryRemoved
    };

    /* A shorter name to use for objects mapping desktop file IDs to 
       desktop entry change types. */
    using ChangeMap = std::map<juce::String, EntryChange>;

    /**
     * @brief  Adds a callback function to run once when the thread finishes
     *         updating desktop entries.
     *
     * @param updateCallback  A callback function to run when updates are
     *                        finished.  The ChangeMap listing all updates will
     *                        be passed in as a parameter.
     *
     * @return                An ID that can be used to cancel the callback
     *                        function.
     */
    CallbackID requestUpdate
        (const std::function<void(const ChangeMap)> updateCallback);

    /**
     * @brief  Cancels a pending callback ID.
     *
     * @param toCancel  The ID of a callback to find and remove from the loading
     *                  or update callback list.
     */
    void cancelCallback(const CallbackID toCancel);

private:
    /**
     * @brief  Finds all desktop entry files within the application data
     *         directories, ignoring all files with duplicate desktop file IDs.
     *
     * This function runs once whenever the desktop entry thread starts running.
     */
    virtual void init() override;

    /**
     * @brief  Loads a DesktopEntry object from file data.
     *
     * This function will be called repeatedly on the desktop entry thread until
     * all entries are loaded.
     *
     * @param threadLock  An object used to access the desktop entry thread's
     *                    SharedResource lock.
     */
    virtual void runLoop(ThreadResource::ThreadLock& threadLock) override;
    
    /**
     * @brief  Runs the loading finished callback function once all desktop
     *         entries are loaded.
     *
     * This function will be called on the desktop entry thread just before the
     * thread stops running.
     */
    void cleanup() override;

    /**
     * @brief  Generates a unique callback ID the thread can assign to a 
     *         callback function.
     *
     * @return  An unused, non-zero callback ID value. 
     */
    CallbackID generateCallbackID() const;

    /**
     * @brief 
     *
     * @param idList
     *
     * @return 
     */
    juce::Array<DesktopEntry> getEntriesByID(const juce::StringArray idList) 
        const;

    /**
     * @brief  Find a desktop entry file using its desktop file ID.
     *
     * @param entryFileID  The desktop file ID of a desktop entry.
     *
     * @return             The first matching file found within application data
     *                     directories, or an empty file object if no matching
     *                     file is found.
     */
    juce::File findEntryFile(const juce::String entryFileID);

    /* Maps desktop ID strings to desktop entry objects. */
    std::map<juce::String, DesktopEntry> entries;

    /* Maps desktop ID strings to desktop entry files. */
    std::map<juce::String, juce::File> entryFiles;

    /* All <Desktop File ID, Desktop File> pairs waiting to be loaded. */
    juce::Array<std::pair<juce::String, juce::File>> pendingFiles;

    /* Maps category names to lists of desktop file IDs. */
    std::map<juce::String, juce::StringArray> categories;

    /* The last time desktop entry files were scanned. */
    juce::Time lastScanTime;

    /* Stores all changes to desktop entries discovered since the last entry
       update began: */
    ChangeMap latestChanges;

    /* Callbacks to run when desktop entries finish loading. */
    std::map<CallbackID, std::function<void()>> onFinish;

    /* Callbacks to run when desktop entries finish updating. */
    std::map<CallbackID,
        std::function<void(ChangeMap)>> onUpdate;
};
#endif
