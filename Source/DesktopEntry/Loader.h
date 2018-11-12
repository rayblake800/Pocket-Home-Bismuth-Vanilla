#pragma once
#include <map>
#include <set>
#include <functional>
#include "ResourceHandler.h"
#include "DesktopEntry/EntryFile.h"
#include "DesktopEntry/DesktopEntry.h"
#include "DesktopEntry/Implementation.h"

/** 
 * @file  DesktopEntry/Loader.h
 *
 * @brief  Finds and caches all desktop entry file data.
 */
class DesktopEntry::Loader : public ResourceHandler<DesktopEntry::LoadingThread>
{
public:
    Loader();

    virtual ~Loader() { }

    /**
     * @brief  Finds a single EntryFile object using its desktop file ID.
     *
     * @param entryFileID         The desktop file ID of a desktop entry file.
     *
     * @throws std::out_of_range  When the ID parameter does not match a loaded
     *                            desktop entry file.
     *
     * @return                    The desktop entry with the given desktop file 
     *                            ID.
     */
    EntryFile getDesktopEntry(const juce::String& entryFileID) const;

    /**
     * @brief  Gets all loaded desktop entry file objects.
     *
     * @return  The complete list of loaded EntryFile objects.
     */
    juce::Array<EntryFile> getAllEntries() const;

    /**
     * @brief  Gets all EntryFile objects in a single category.
     *
     * @param category  A desktop application category string.
     *
     * @return          All EntryFile objects that have the given category.
     */
    juce::Array<EntryFile> getCategoryEntries(const juce::String& category) 
        const;

    /**
     * @brief  Finds all EntryFile objects within several categories.
     * 
     * @param categoryList  One or more application category names.
     *
     * @return              All EntryFile objects with at least one of the 
     *                      category values in the category list.
     */
    juce::Array<EntryFile> getCategoryEntries
    (const juce::StringArray& categoryList) const;

    /**
     * @brief  Scans all desktop entry files for any changes made since the 
     *         last time the LoadingThread read the entry files.
     */
    void scanForChanges();

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Loader)
};
