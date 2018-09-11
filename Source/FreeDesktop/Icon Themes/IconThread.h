#pragma once
#include <map>
#include "SharedResource.h"
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

class IconThread : public juce::Thread, public SharedResource
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
        /* Function used to asynchronously return the loaded image */
        std::function<void(juce::Image) > callback;
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
    const QueuedJob getQueuedJob();

private:
    /**
     * @brief  Asynchronously handles queued icon requests.
     */
    void run() override;
    
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

    /* Icon theme indexes used to load icons, in order of priority */
    juce::OwnedArray<IconThemeIndex> iconThemes;

    /* Directories to search, in order, for icon themes and un-themed icons. */
    juce::StringArray iconDirectories;

    /* Default icon */
    juce::Image defaultIcon;
};
