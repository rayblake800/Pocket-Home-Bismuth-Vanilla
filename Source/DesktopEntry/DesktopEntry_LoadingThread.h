#ifndef DESKTOP_ENTRY_IMPLEMENTATION
  #error File included outside of DesktopEntry implementation.
#endif
#pragma once
/**
 * @file  DesktopEntry/LoadingThread.h
 *
 * @brief  Loads, caches, and updates desktop entry data.
 */

#include "SharedResource_ThreadResource.h"
#include "DesktopEntry_EntryFile.h"
#include "DesktopEntry_CallbackID.h"
#include <map>

/**
 *  The desktop entry standard defines a set of directories that may contain 
 * desktop files, along with the order that they should be searched.  Each
 * desktop entry has a desktop file ID based on its filename and path within
 * its desktop entry directory.  When multiple entry files exist with the same 
 * desktop file ID, the first one found using the standard search order will be
 * used, and all others are ignored.
 *
 * The LoadingThread searches these directories for desktop entries, and caches
 * all visible entries without duplicate IDs as EntryFile objects.  Once data is
 * first loaded, the LoadingThread may be used to update the data, scanning 
 * all desktop entry file directories for changes, and sharing these changes 
 * with all DesktopEntry::UpdateListener objects.
 */
namespace DesktopEntry { class LoadingThread; }

class DesktopEntry::LoadingThread : public SharedResource::ThreadResource
{
public:
    /* SharedResource object class key */ 
    static const juce::Identifier resourceKey;

    /**
     * @brief  Creates and starts the thread resource.
     */
    LoadingThread();

    virtual ~LoadingThread() { }

    /* Allow handlers to notify the thread normally to wake it when waiting. */
    using juce::Thread::notify;

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
     * @brief  Gets all loaded desktop entry file objects.
     *
     * @return  The complete list of loaded EntryFile objects.
     */
    juce::Array<EntryFile> getAllEntries() const;

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

    /**
     * @brief  Cancels a pending callback ID.
     *
     * @param toCancel  The ID of a callback to find and remove from the loading
     *                  or update callback list.
     */
    void cancelCallback(const CallbackID toCancel);

    /**
     * @brief  Finds all relevant changes to the set of loaded desktop entry 
     *         files, and prepares to fully update desktop entry data.
     */
    void findUpdatedFiles();

    /**
     * @brief  Checks if the thread has finished loading desktop entry files,
     *         and is either running cleanup or waiting for another request.
     *
     * @return  True if entries have been loaded, and desktop entry data may be
     *          accessed.
     */
    bool isFinishedLoading();

private:
    /**
     * @brief  Scans for new and updated desktop entry files for the thread to
     *         process.
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
     *         files have been loaded or updated, and notifies UpdateListeners
     *         of all new changes.
     *
     * This function will be called on the desktop entry thread just before the
     * thread stops running.
     */
    void cleanup() override;

    /**
     * @brief   Makes the thread sleep after loading or updating all desktop
     *          files.
     *
     * @return  True if there are still files to (re)load, false if all desktop
     *          files were loaded or updated.
     */
    virtual bool threadShouldWait() override;
    
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
    std::map<juce::String, juce::File> pendingFiles;

    /* Maps category names to lists of desktop file IDs. */
    std::map<juce::String, juce::StringArray> categories;

    /* Tracks if desktop entries were completely loaded, and aren't currently
       being updated. */
    bool finishedLoading = false;

    /* Callbacks to run when desktop entries finish loading. */
    std::map<CallbackID, std::function<void()>> onFinish;

    /* Stored data from the last desktop entry update: */

    /* The last time desktop entry files were scanned. */
    juce::Time lastScanTime;

    /* Lists the IDs of all new desktop entry files discovered in the last
       update scan. */
    juce::StringArray lastAddedIDs;

    /* Lists the IDs of all changed desktop entry files found during the last
       update scan. */
    juce::StringArray lastChangedIDs;

    /* Lists the IDs of all hidden or removed desktop entry files discovered
       during the last update scan. */
    juce::StringArray lastRemovedIDs;
};
