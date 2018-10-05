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
     * @brief  Discards any existing entry data and asynchronously reloads all
     *         desktop entries from the file system.
     * 
     * @param notifyCallback  An optional callback function to send loading
     *                        progress update strings.  This will be called 
     *                        asynchronously on the message thread.
     *
     * @param onFinish        An optional callback function to run on 
     *                        completion.  This will be called asynchronously on
     *                        the message thread.
     */
    void loadEntries
    (std::function<void(juce::String) > notifyCallback 
            = std::function<void(juce::String)>(),
            std::function<void() > onFinish = std::function<void()>());

    /**
     * @brief  Removes the notifyCallback and onFinish callback functions that
     *         are currently set to run when the thread finishes loading. 
     *
     * This should be called whenever it's necessary to delete an object
     * referenced in the callbacks while desktop entries are still loading.
     */
    void clearCallbacks();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DesktopEntryLoader)
};
