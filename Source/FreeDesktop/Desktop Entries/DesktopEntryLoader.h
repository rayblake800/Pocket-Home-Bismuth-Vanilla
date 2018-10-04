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

    virtual ~DesktopEntryLoader();

    /**
     * @return the number of stored DesktopEntry objects
     */
    int size();

    /**
     * Get all DesktopEntry objects with a given category name.
     *
     * @param category  The category name to search for.
     *
     * @return a set of all matching DesktopEntries
     */
    std::set<DesktopEntry> getCategoryEntries(const juce::String& category);

    /**
     * Finds all DesktopEntry objects within several categories.
     * 
     * @param categoryList  One or more application category names.
     *
     * @return the set of all DesktopEntry objects with at least one of the 
     *         category values in the category list.
     */
    std::set<DesktopEntry> getCategoryListEntries
        (const juce::StringArray& categoryList);

    /**
     * Finds the list of all categories found in all desktop entries.
     * 
     * @return a set of all category entries.
     */
    std::set<juce::String> getCategories();

    /**
     * Discards any existing entry data and asynchronously reload all desktop 
     * entries from the file system.
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
     * This removes the notifyCallback and onFinish callback functions that
     * are currently set to run when the thread finishes loading.  This should
     * be called whenever it's necessary to delete an object referenced in the
     * callbacks while desktop entries are still loading.
     */
    void clearCallbacks();

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DesktopEntryLoader)

};


