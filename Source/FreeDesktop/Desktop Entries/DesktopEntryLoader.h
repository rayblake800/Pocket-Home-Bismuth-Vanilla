#pragma once
#include <map>
#include <set>
#include <functional>
#include "Utils.h"
#include "ResourceHandler.h"
#include "DesktopEntry.h"

/** 
 * @file  DesktopEntryLoader.h
 *
 * @brief  Finds and caches all desktop entry file data.
 */

/* Private SharedResource object class. */
class DesktopEntryThread;

class DesktopEntryLoader : public ResourceHandler<DesktopEntryThread>
{
public:
    DesktopEntryLoader();

    virtual ~DesktopEntryLoader() { }

    /**
     * @brief  Gets the number of cached desktop entries.
     *
     * @return  The number of stored DesktopEntry objects.
     */
    int size();

    /**
     * @brief  Gets the desktop file IDs of all entries within a category.
     *
     * @param category  A desktop application category name.
     *
     * @return          ID strings for each desktop entry within that category.
     */
    juce::StringArray getCategoryEntryIDs(const juce::String& category);

    /**
     * @brief  Gets the desktop file IDs of all entries within a list of several
     *         categories.
     *
     * @param categoryList  A list of desktop application category names.
     *
     * @return              ID strings for all desktop entries that are in at 
     *                      least one of the given categories.
     */
    juce::StringArray getCategoryListEntryIDs
        (const juce::StringArray& categoryList);

    /**
     * @brief  Finds a single DesktopEntry object using its desktop file ID.
     *
     * @param entryFileID         The desktop file ID of a desktop entry file.
     *
     * @throws std::out_of_range  When the ID parameter does not match a loaded
     *                            desktop entry file.
     *
     * @return                    The desktop entry with the given desktop file 
     *                            ID.
     */
    DesktopEntry getDesktopEntry(const juce::String& entryFileID);

    /**
     * @brief  Gets all DesktopEntry objects in a single category.
     *
     * @param category  A desktop application category string.
     *
     * @return          All DesktopEntry objects that have the given category.
     */
    juce::Array<DesktopEntry> getCategoryEntries(const juce::String& category);

    /**
     * @brief  Finds all DesktopEntry objects within several categories.
     * 
     * @param categoryList  One or more application category names.
     *
     * @return              All DesktopEntry objects with at least one of the 
     *                      category values in the category list.
     */
    juce::Array<DesktopEntry> getCategoryListEntries
        (const juce::StringArray& categoryList);
    
    /**
     * @brief  Reloads a single desktop entry from the file system.
     *
     * @param entryFileID  The desktop file ID of a desktop entry file.
     */
    void reloadEntry(const juce::String& entryFileID);

    /**
     * @brief  Scans all desktop entry files for any changes made since the 
     *         last time the DesktopEntryLoader read the entry files.
     */
    void scanForChanges();
    
    /**
     * @brief  Identifies a callback function that will run after entries are 
     *         loaded.
     *
     * A zero value represents an unset or invalid ID.
     */
       
    typedef juce::uint32 CallbackID;

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
