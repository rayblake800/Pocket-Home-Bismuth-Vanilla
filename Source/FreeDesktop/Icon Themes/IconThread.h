#pragma once
#include <map>
#include <functional>
#include "ResourceManager.h"
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


class IconThread : private ResourceManager
{
public:
    IconThread();

    virtual ~IconThread() { }

    /**
     * Queues up an icon request.  
     * 
     * @param icon           This should be either a full icon file path, or the 
     *                       filename(without extension) of an icon in one of 
     *                       the system icon directories.  If no direct match
     *                       is found, the icon thread will attempt to find and
     *                       use an icon with a name partially matching this
     *                       string.
     * 
     * @param size           The ideal width and height, in pixels, of the
     *                       returned Image object.  IconThread will attempt
     *                       to find an image file as close to this size as
     *                       possible.
     * 
     * @param assignImage    This function will be called on the message thread.
     *                       The image parameter passed to the callback will
     *                       either be the requested image, or a default icon
     *                       image if no image files match the request.
     * 
     * @param context        An icon context to use to limit which icon theme
     *                       sub-directories are searched, or 
     *                       IconThemeIndex::unknownCtx to search all
     *                       sub-directories within icon themes. 
     * 
     * @param scale          The scale factor that the caller expects to apply
     *                       to the image file before displaying the Image
     *                       object.  This value is typically only relevant on
     *                       extra high resolution displays.
     */
    void loadIcon(juce::String icon, int size, 
            std::function<void(juce::Image) > assignImage,
            IconThemeIndex::Context context = IconThemeIndex::unknownCtx,
            int scale = 1);


private:

    /**
     * Shares access to the icon map and the job queue.
     */
    class IconResource : public juce::Thread,
    public ResourceManager::SharedResource
    {
    public:
        IconResource();

        /**
         * Make sure the thread exits before destroying this object.
         */
        virtual ~IconResource();

        struct QueuedJob
        {
            juce::String icon;
            int size;
            int scale;
            IconThemeIndex::Context context;
            std::function<void(juce::Image) > callback;
        };

        /**
         * Returns the number of pending icon requests. 
         */
        int numJobsQueued();

        /**
         * Adds another job request to the queue.
         * 
         * @param newJob
         */
        void addQueuedJob(QueuedJob newJob);

        /**
         * Removes and returns the last job from the list.
         * 
         * @return the last job, or a QueuedJob with an empty icon string and
         *          callback function if the queue is empty.
         */
        QueuedJob getQueuedJob();

    private:
        /**
         * Asynchronously handles queued icon requests
         */
        void run() override;
        
        /**
         * Search icon theme directories for an icon matching a given request.
         * 
         * @param request  Defines the name and size of the requested icon
         * 
         * @return  The full path of the best matching icon file, or the empty
         *          string if no match is found.
         */
        juce::String getIconPath(const QueuedJob& request);

        //Queued icon requests waiting for the icon thread.
        juce::Array<QueuedJob, juce::CriticalSection> queuedJobs;
        //Icon theme indexes used to load icons, in order of priority
        juce::OwnedArray<IconThemeIndex> iconThemes;
        //Directories to search, in order, for icon themes and un-themed icons.
        juce::StringArray iconDirectories;
    };

    //Default image icons to copy into AppMenuButtons
    juce::Image defaultIcon;

    //default icon path definitions
    static const juce::String defaultIconPath;

    //ResourceManager shared object and lock;
    static juce::ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static juce::ReadWriteLock iconLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconThread)
};