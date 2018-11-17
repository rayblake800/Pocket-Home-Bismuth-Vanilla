#include "IconLoader.h"

/*
 * Adds a request to the list of queued tasks.
 */
IconLoader::RequestID IconLoader::loadIcon(
        const juce::String icon, 
        const int size, 
        const std::function<void(juce::Image)> assignImage,
        const IconThemeIndex::Context context,
        const int scale)
{
    auto iconThread = getWriteLockedResource();
    IconThread::IconRequest newJob = 
    {
        icon,
        size,
        scale,
        context,
        assignImage
    };
    return iconThread->addRequest(newJob);
}

/*
 * Cancels a pending image assignment.
 */
void IconLoader::cancelImageRequest(const RequestID toCancel)
{
    auto iconThread = getWriteLockedResource();
    iconThread->cancelRequest(toCancel);
}


