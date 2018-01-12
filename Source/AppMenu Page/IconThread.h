/**
 * @File IconThread.h
 * @Author Anthony Brown
 * 
 * Loads application and directory icons asynchronously for AppMenuButtons.
 */

#pragma once
#include <map>
#include "AppMenuButton/AppMenuButton.h"

class IconThread : public Thread {
public:
    IconThread();
    virtual ~IconThread();

    /**
     * Queues up an icon request.  
     * @param button a button that needs an icon.  It will be assigned
     * a default icon immediately, while the correct icon is loaded
     * asynchronously.
     * @param icon either a full icon file path, or the filename(without
     * extension) of an icon in one of the system icon directories.
     */
    void loadIcon(AppMenuButton::Ptr button, String icon);
    
    /**
     * While AppMenuButtons still need icons, find them in a
     * separate thread.
     */
    void run() override;
    

private:
    struct QueuedJob{
        AppMenuButton::Ptr button;
        String icon;
    };
    Array<QueuedJob> queuedJobs;
    
    
    //Contains <filename(no extention),fullpath/filename.extension>
    //mappings for all icons found on the system.
    std::map<String, String> iconPaths;
    //True iff icon paths have already been mapped
    bool iconPathsMapped;

    /**
     * Creates the map of all icon file paths.
     */
    void mapIcons();
    
    //Prevent concurrent modification of icon maps/queuedJobs
    CriticalSection lock;
    
    //Default image icons to copy into AppMenuButtons
    Image defaultAppIcon;
    Image defaultDirectoryIcon;

    //default icon path definitions
    static const String defaultAppIconPath;
    static const String defaultDirectoryIconPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconThread)
};