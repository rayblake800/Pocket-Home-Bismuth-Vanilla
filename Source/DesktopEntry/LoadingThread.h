// Only the DesktopEntryLoader may access the DesktopEntryThread
#ifdef DESKTOP_ENTRY_LOADER_IMPLEMENTATION
#pragma once
#include <map>
#include "ThreadResource.h"
#include "DesktopEntry/DesktopEntry.h"
#include "DesktopEntry/EntryFile.h"

/**
 * @file  DesktopEntry/LoadingThread.h
 *
 * @brief  Loads, caches, and updates desktop entry data.
 */
class DesktopEntry::LoadingThread : public ThreadResource
{
public:
    /* SharedResource object class key */ 
    static const juce::Identifier resourceKey;

    /**
     * @brief  Creates and starts the thread resource.
     */
    LoadingThread();

    virtual ~LoadingThread() { }

    /**
     * @brief  Finds a desktop entry from its desktop file ID. 
     *
     * @param entryFileID  A valid desktop file ID.
     *
     * @return             The matching desktop entry, or an empty DesktopEntry
     *                     if the entryFileID does not match a stored entry
     *                     object.
     */
    EntryFile getDesktopEntry (const juce::String entryFileID) const;

    /**
     * @brief  Gets all desktop entries with a specific application category.
     *
     * @param category  The category to search for in the DesktopEntry list.
     *
     * @return          All loaded entries that have that category.
     */
    juce::Array<EntryFile> getCategoryEntries(const juce::String category) 
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
    juce::Array<EntryFile> getCategoryEntries
        (const juce::StringArray categoryList) const;

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

    /* A shorter name to use for objects mapping desktop file IDs to 
       desktop entry update types. */
    using ChangeMap = std::map<juce::String, UpdateType>;

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
    CallbackID addUpdateCallback
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
     * @brief  Finds all unloaded or updated desktop entry files within the 
     *         application data directories, ignoring files with duplicate
     *         desktop file IDs.
     *
     * This function runs once whenever the desktop entry thread starts running.
     */
    virtual void init() override;

    /**
     * @brief  Loads or updates a single desktop entry file in the list of 
     *         pending files.
     *
     * This function will be called repeatedly on the desktop entry thread until
     * all entry files have been loaded or updated.
     *
     * @param threadLock  An object used to access the desktop entry thread's
     *                    SharedResource lock.
     */
    virtual void runLoop(ThreadResource::ThreadLock& threadLock) override;
    
    /**
     * @brief  Runs all registered callback functions once all desktop entry 
     *         files have been loaded or updated.
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
    std::map<juce::String, EntryFile> entries;

    /* Maps desktop ID strings to .desktop files. */
    std::map<juce::String, juce::File> entryFiles;

    /* All <Desktop file ID, .desktop file> pairs waiting to be loaded. */
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
    std::map<CallbackID, std::function<void(ChangeMap)>> onUpdate;
};
#endif
