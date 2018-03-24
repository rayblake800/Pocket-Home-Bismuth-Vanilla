/**
 * @File IconThread.h
 * 
 * Loads application and directory icons asynchronously. All icons in typical
 * icon directories are mapped, attempting to avoid duplicates.  This map is
 * then used to quickly locate icon files by name as they are needed.
 */

#pragma once
#include <map>
#include <functional>
#include "JuceHeader.h"

class IconThread : public Thread
{
public:
    IconThread();

    virtual ~IconThread() { }

    /**
     * Queues up an icon request.  
     * 
     * @param icon          This should be either a full icon file path, or the 
     *                       filename(without extension) of an icon in one of 
     *                       the system icon directories.  If no direct match
     *                       is found, the icon thread will attempt to find and
     *                       use an icon with a name partially matching this
     *                       string.
     * 
     * @param assignImage   A callback function to asynchronously use the
     *                       image found by the thread. Unless the icon can be 
     *                       immediately located without searching, It will be 
     *                       called twice, once to immediately apply a default 
     *                       image, and again when the requested image has been
     *                       found.
     */
    void loadIcon(String icon, std::function<void(Image) > assignImage);

    /**
     * While AppMenuButtons still need icons, this finds them in a separate 
     * thread.
     */
    void run() override;

private:
    //Queued icon requests waiting for the icon thread.

    struct QueuedJob
    {
        String icon;
        std::function<void(Image) > callback;
    };
    Array<QueuedJob> queuedJobs;

    /**
     * Compares icon directories for IconThread::mapIcons, prioritizing ones 
     * with resolution numbers closer to 128
     */
    class IconFileComparator
    {
    public:
        static int compareElements(File first, File second);
    };

    /**
     * Searches for and returns an icon's full path. 
     * 
     * @param icon  Either a full icon file path, or the filename(without
     *               extension) of an icon in one of the system icon 
     *               directories.
     * 
     * @return     The closest matching icon image available 
     */
    String getIconPath(String icon);

    //Contains <filename(no extention),fullpath/filename.extension>
    //mappings for all icons found on the system.
    std::map<String, String> iconPaths;
    //True iff icon paths have already been mapped
    bool iconPathsMapped = false;

    /**
     * Creates the map of all icon file paths.
     */
    void mapIcons();

    //Prioritize icon directories with listed resolutions closest to this value.
    const int idealIconResolution = 128;

    //Prevent concurrent modification of icon maps/queuedJobs
    CriticalSection lock;

    //Default image icons to copy into AppMenuButtons
    Image defaultIcon;

    //default icon path definitions
    static const String defaultIconPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconThread)
};