/* 
 * @file   DesktopEntries.h
 *
 * Finds all .Desktop and .Directory files in the system,
 * and stores and sorts them as DesktopEntry objects.
 */

#pragma once
#include <map>
#include <set>
#include <functional>
#include "../../Utils.h"
#include "DesktopEntry.h"

class DesktopEntries {
public:
    DesktopEntries();
    DesktopEntries(const DesktopEntries& orig);
    virtual ~DesktopEntries();

    /**
     * @return the number of stored DesktopEntry objects
     */
    int size();

    /**
     * Get all DesktopEntry objects with a given category name
     * @param category the category name
     * @return a set of all matching DesktopEntries
     */
    std::set<DesktopEntry> getCategoryEntries(String category);

    /**
     * Get a list of all DesktopEntry objects within several categories
     * @param categoryList one or more category values
     * @return all DesktopEntry objects with at least one of the category values
     * in categories
     */
    std::set<DesktopEntry> getCategoryListEntries(Array<String> categoryList);

    /**
     * Get the list of all categories found in all desktop entries.
     * @return a set of all category entries
     */
    std::set<String> getCategories();

    /**
     * Discards any existing entry data and reloads all desktop entries
     * from the file system.
     * @param notifyCallback optional callback function to send loading
     * progress update strings.  This will be called asynchronously on the
     * message thread.
     * @param onFinish optional callback function to run on completion.  This
     * will be called asynchronously on the message thread.
     */
    void loadEntries
    (std::function<void(String) > notifyCallback = std::function<void(String)>(),
            std::function<void() > onFinish = std::function<void()>());

private:
    //list of all entries
    std::set<DesktopEntry> entries;
    //maps category names to lists of entries
    std::map<String, std::set<DesktopEntry>> categories;

    CriticalSection lock;

    /**
     *Asynchronously loads DesktopEntry objects from files
     */
    class LoadingThread : public Thread{
    public:
        LoadingThread();
        virtual ~LoadingThread();
        
        /**
         * Reload all desktop entries in a seperate thread.
         * @param threadOwner
         * @param notifyCallback this callback runs periodically to pass back
         * loading progress strings, so progress can be shown in the UI.
         * @param onFinish This callback runs when all data finishes
         *  loading.
         */
        void asyncLoadEntries(DesktopEntries * threadOwner,
                std::function<void(String) > notifyCallback,
                std::function<void() > onFinish);
    private:
        void run() override;
        DesktopEntries * threadOwner;
        std::function<void(String)> notifyCallback;
        std::function<void() > onFinish;
    };
    LoadingThread loadingThread;
};


