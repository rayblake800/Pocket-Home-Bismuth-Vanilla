#include "IconLoader.h"

IconLoader::IconLoader() :
ResourceHandler<IconThread>(IconThread::resourceKey,
        []()->SharedResource* { return new IconThread(); }) { }

/*
 * Adds a request to the list of queued tasks.
 */
IconLoader::CallbackID IconLoader::loadIcon(
        const juce::String icon, 
        const int size, 
        const std::function<void(juce::Image)> assignImage,
        const IconThemeIndex::Context context,
        const int scale)
{
    auto iconThread = getWriteLockedResource();
    CallbackID callbackID = iconThread->saveIconCallback(assignImage);
    IconThread::QueuedJob newJob = 
    {
        icon,
        size,
        scale,
        context,
        callbackID
    };
    iconThread->addQueuedJob(newJob);
    return callbackID;
}

/*
 * Cancels a pending image assignment.
 */
void IconLoader::cancelImageAssignment(const CallbackID toCancel)
{
    auto iconThread = getWriteLockedResource();
    iconThread->takeIconCallback(toCancel);
}


