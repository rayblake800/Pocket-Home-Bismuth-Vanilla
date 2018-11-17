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
     * @brief  Identifies pending icon requests so that they can be cancelled.
     */
    typedef unsigned int RequestID;

    /**
     * @brief  Cancels a pending icon request.
     *
     * @param requestID  The ID of the icon loading request to remove.
     */
    void cancelRequest(const RequestID requestID);

    /**
     * Holds a pending icon request.
     */
    struct IconRequest
    {
        /* Name or path of an icon */
        juce::String icon;
        /* Ideal width/height, in pixels */
        int size;
        /* Expected scale factor of the icon */
        int scale;
        /* Category of icon requested */
        IconThemeIndex::Context context;
        /* Function used to apply the requested icon */
        std::function<void(juce::Image)> loadingCallback;
    };

    /**
     * @brief  Adds an icon loading request to the queue.
     * 
     * @param request  A new icon request.
     *
     * @return         An ID that may be used to cancel the pending request.
     */
    RequestID addRequest(IconRequest request);

private:
    /**
     * @brief  Asynchronously handles queued icon requests.
     */
    virtual void runLoop(ThreadLock& lock) override;
    
    /**
     * @brief  Searches icon theme directories for an icon matching a given
     *         request.
     * 
     * @param request  Defines the name and size of the requested icon.
     * 
     * @return         The full path of the best matching icon file, or the
     *                 empty string if no match is found.
     */
    juce::String getIconPath(const IconRequest& request);

    /* All pending icon requests, mapped by ID so they can be cancelled. */
    std::map<RequestID, IconRequest> requestMap;

    /* Icon theme indexes used to load icons, in order of priority */
    juce::OwnedArray<IconThemeIndex> iconThemes;

    /* Directories to search, in order, for icon themes and unthemed icons. */
    juce::StringArray iconDirectories;

    /* Stores loaded icon names mapped to image objects to avoid having to
       repeatedly load icons: */
    juce::HashMap<juce::String, juce::Image> imageCache;

    /* Default icon */
    juce::Image defaultIcon;
};
