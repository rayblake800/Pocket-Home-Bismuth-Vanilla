#pragma once
#include <map>
#include "SharedResource/ThreadResource.h"
#include "IconThemeIndex.h"
#include "JuceHeader.h"

/**
 * @File IconThread.h
 * 
 * @brief Finds icon files in icon theme directories, and loads them as Image
 *        objects.
 * 
 * The IconThread provides an interface for other objects to request icon Image
 * objects with a specific name, and a preferred size and scale.  The IconThread
 * handles these requests asynchronously, searching the user's selected icon
 * theme directories for the closest icon matching the request.  
 * 
 * This process uses the XDG Base Directory Specification, the user's .gtkrc
 * config file, and the icon themes' index.theme files to determine which
 * directories are prioritized.  GTK's icon-theme.cache files are used to
 * quickly locate image files within icon theme directories.
 * 
 * @see IconThemeIndex.h, IconCache.h
 */

class IconThread : public SharedResource::ThreadResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    IconThread();

    /**
     * @brief Ensures the thread exits before destruction.
     */
    virtual ~IconThread();


    /**
     * @brief  Identifies pending icon callback functions so that they can be
     *         cancelled.
     */
    typedef unsigned int CallbackID;

    /**
     * @brief  Saves an icon loading callback function to the callback map so
     *         that it can be retrieved later.
     *
     * @param loadingCallback  A function used to handle an icon's Image object
     *                         once it has been created.
     *
     * @return                 A unique callback ID that can be used to retrieve
     *                         or remove the callback function.
     */
    CallbackID saveIconCallback
    (const std::function<void(juce::Image)> loadingCallback);

    /**
     * @brief  Removes and returns an icon loading callback function from the
     *         callbackMap.
     *
     * @param callbackID  The ID of the function to take.
     *
     * @return            The callback function with that ID, or an invalid
     *                    function if the ID was invalid.
     */
    std::function<void(juce::Image)> takeIconCallback
    (const CallbackID callbackID);

    /**
     * Holds a queued icon request
     */
    struct QueuedJob
    {
        /* Name or path of an icon */
        juce::String icon;
        /* Ideal width/height, in pixels */
        int size;
        /* Expected scale factor of the icon */
        int scale;
        /* Category of icon requested */
        IconThemeIndex::Context context;
        
        /* IDs the function used to asynchronously return the loaded image */
        CallbackID iconCallbackID;
    };

    /**
     * @brief  Gets the number of pending icon requests. 
     *
     * @return  The number of queued requests.
     */
    int numJobsQueued();

    /**
     * @brief  Adds another icon request to the queue.
     * 
     * @param newJob  A new icon request.
     */
    void addQueuedJob(const QueuedJob newJob);

    /**
     * @brief  Removes and returns the last job from the list.
     * 
     * @return  The last job, or a QueuedJob with an empty icon string and
     *          callback function if the queue is empty.
     */
    const QueuedJob takeQueuedJob();
private:
    /**
     * @brief  Asynchronously handles queued icon requests.
     */
    virtual void runLoop(ThreadLock& lock) override;
    
    /**
     * @brief  Searches icon theme directories for an icon matching a given
     *         request.
     * 
     * @param request  Defines the name and size of the requested icon/
     * 
     * @return         The full path of the best matching icon file, or the
     *                 empty string if no match is found.
     */
    juce::String getIconPath(const QueuedJob& request);

    /* Queued icon requests waiting for the icon thread. */
    juce::Array<QueuedJob, juce::CriticalSection> queuedJobs;

    /* Maps callback functions so they can be cancelled if necessary. */ 
    std::map<CallbackID, std::function<void(juce::Image)>> callbackMap;

    /* Icon theme indexes used to load icons, in order of priority */
    juce::OwnedArray<IconThemeIndex> iconThemes;

    /* Directories to search, in order, for icon themes and un-themed icons. */
    juce::StringArray iconDirectories;

    /* Stores loaded icon names mapped to image objects to avoid having to
       repeatedly load icons: */
    juce::HashMap<juce::String, juce::Image> imageCache;

    /* Default icon */
    juce::Image defaultIcon;
};
