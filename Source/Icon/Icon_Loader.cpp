#include "Icon_Loader.h"
#include "Icon_ThreadResource.h"

Icon::Loader::Loader() { }

/*
 * Adds a request to the list of queued tasks.
 */
Icon::RequestID Icon::Loader::loadIcon(
        const juce::String icon, 
        const int size, 
        const std::function<void(juce::Image)> assignImage,
        const Context context,
        const int scale)
{
    SharedResource::LockedPtr<ThreadResource> iconThread 
            = getWriteLockedResource();
    ThreadResource::IconRequest newJob = 
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
void Icon::Loader::cancelImageRequest(const RequestID toCancel)
{
    SharedResource::LockedPtr<ThreadResource> iconThread 
            = getWriteLockedResource();
    iconThread->cancelRequest(toCancel);
}


