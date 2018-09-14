#include "IconLoader.h"

IconLoader::IconLoader() :
ResourceHandler<IconThread>(IconThread::resourceKey,
        []()->SharedResource* { return new IconThread(); }) { }

/*
 * Adds a request to the list of queued tasks.
 */
void IconLoader::loadIcon(
        const juce::String icon, 
        const int size, 
        const std::function<void(juce::Image)> assignImage,
        const IconThemeIndex::Context context,
        const int scale)
{
    IconThread::QueuedJob newJob = 
    {
        icon,
        size,
        scale,
        context,
        assignImage
    };
    
    auto iconThread = getWriteLockedResource();
    iconThread->addQueuedJob(newJob);
}
