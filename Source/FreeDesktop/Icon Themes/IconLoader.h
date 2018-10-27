#pragma once
#include "IconThread.h"
#include "ResourceHandler.h"
/**
 * @file IconLoader.h
 * 
 * @brief  Sends icon search requests to the shared IconThread.
 */
class IconLoader : public ResourceHandler<IconThread>
{
public:
    IconLoader();

    virtual ~IconLoader() { }

    /**
     * @brief  Adds a request to the list of queued tasks.
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
    void loadIcon(
            const juce::String icon, 
            const int size, 
            const std::function<void(juce::Image)> assignImage,
            const IconThemeIndex::Context context = IconThemeIndex::unknownCtx,
            const int scale = 1);
};
