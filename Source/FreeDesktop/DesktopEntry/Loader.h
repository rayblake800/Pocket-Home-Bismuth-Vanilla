#pragma once
#include <map>
#include <set>
#include <functional>
#include "ResourceHandler.h"
#include "DesktopEntry/Data.h"
#include "DesktopEntry/DesktopEntry.h"

/** 
 * @file  DesktopEntry/Loader.h
 *
 * @brief  Finds and caches all desktop entry file data.
 */
class DesktopEntry::Loader : public ResourceHandler<DesktopEntry::Thread>
{
public:
    Loader();

    virtual ~Loader() { }

    /**
     * @brief  Finds a single DesktopEntry::Data object using its desktop file 
     *         ID.
     *
     * @param entryFileID         The desktop file ID of a desktop entry file.
     *
     * @throws std::out_of_range  When the ID parameter does not match a loaded
     *                            desktop entry file.
     *
     * @return                    The desktop entry with the given desktop file 
     *                            ID.
     */
    Data getDesktopEntry(const juce::String& entryFileID) const;

    /**
     * @brief  Gets all DesktopEntry::Data objects in a single category.
     *
     * @param category  A desktop application category string.
     *
     * @return          All DesktopEntry objects that have the given category.
     */
    juce::Array<DesktopEntry::Data> getCategoryEntries
        (const juce::String& category) const;

    /**
     * @brief  Finds all DesktopEntry objects within several categories.
     * 
     * @param categoryList  One or more application category names.
     *
     * @return              All DesktopEntry objects with at least one of the 
     *                      category values in the category list.
     */
    juce::Array<DesktopEntry::Data> getCategoryEntries
        (const juce::StringArray& categoryList) const;
    
    /**
     * @brief  Reloads a single desktop entry from the file system.
     *
     * @param entryFileID  The desktop file ID of a desktop entry file.
     */
    void reloadEntry(const juce::String& entryFileID);
 


    /**
     * @brief  Lists the main types of changes to desktop entry files that the
     *         DesktopEntryLoader could find.
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

    /**
     * @brief  Scans all desktop entry files for any changes made since the 
     *         last time the DesktopEntryLoader read the entry files.
     *
     * @param handleChanges  A callback to run on the juce message thread when 
     *                       all changes have been read.  The function will
     *                       be passed the list of updated entry IDs mapped to 
     *                       EntryChange descriptions of the changes.
     *
     * @return               A callback ID that may later be used to cancel the
     *                       callback function, if it is still pending.
     */
    CallbackID scanForChanges
    (const std::function<void(std::map<juce::String, EntryChange>)> 
            handleChanges);

    /**
     * @brief  Schedules an action to run once all entries have been loaded.
     *
     * @param onFinish  A callback function to run when the thread is done 
     *                  loading or reloading entries.  If loading finished
     *                  before this function was called, the callback will run
     *                  immediately.  Otherwise, it will run on the Juce message
     *                  thread as soon as the desktop entry loader is finished.
     *
     * @return          A callback ID that may later be used to cancel the
     *                  callback function, if it is still pending.
     */
    CallbackID waitUntilLoaded(std::function<void()> onFinish);

    /**
     * @brief  Removes an onFinish callback functions that is set to run when 
     *         the thread finishes loading.
     *
     * This should be called whenever it's necessary to delete an object
     * referenced in the callbacks while desktop entries are still loading.
     *
     * @param callbackID  The ID of the callback function that should be
     *                    cancelled.  If this is invalid, no action will be
     *                    taken, besides briefly locking the entry loader
     *                    thread if the callbackID is non-zero.
     */
    void clearCallback(CallbackID callbackID);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DesktopEntryLoader)
};
