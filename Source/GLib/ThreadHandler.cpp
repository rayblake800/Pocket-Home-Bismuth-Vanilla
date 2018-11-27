#include "GLib/ThreadResource.h"
#include "GLib/ThreadHandler.h"

/*
 * Permanently connects the ThreadHandler to a single thread resource on 
 * construction.
 */
GLib::ThreadHandler::ThreadHandler(const juce::Identifier& resourceKey) :
SharedResource::Handler<ThreadResource>(resourceKey,
        [resourceKey]()->ThreadResource*
        { 
            return new ThreadResource(resourceKey, 
                    SharedContextPtr(g_main_context_new()));
        }) { } 

/*
 * Runs a function on this GLib event loop, waiting until the function has 
 * finished.
 */
void GLib::ThreadHandler::call(const std::function<void()> toCall,
        const std::function<void()> onFailure) const
{
    SharedResource::LockedPtr<ThreadResource> threadResource
        = getWriteLockedResource();
    threadResource->call(toCall, onFailure, [&threadResource]() 
            { threadResource.unlock(); });
    
}

/*
 * Asynchronously runs a function once on this GLib event loop.
 */
void GLib::ThreadHandler::callAsync(const std::function<void()> toCall,
        const std::function<void()> onFailure) const
{
    SharedResource::LockedPtr<ThreadResource> threadResource
        = getWriteLockedResource();
    threadResource->callAsync(toCall, onFailure);
}

/*
 * Checks if the thread resource is currently running.
 */
bool GLib::ThreadHandler::isThreadRunning() const
{
    SharedResource::LockedPtr<ThreadResource> threadResource
        = getReadLockedResource();
    return threadResource->isThreadRunning();
}

/*
 * Gets the GLib event thread's context.
 */
GLib::SharedContextPtr GLib::ThreadHandler::getContext()
{
    SharedResource::LockedPtr<ThreadResource> threadResource
        = getReadLockedResource();
    return threadResource->getContext();
}

/*
 * Starts the thread resource if it is currently not running.
 */
void GLib::ThreadHandler::startThread()
{
    SharedResource::LockedPtr<ThreadResource> threadResource
        = getWriteLockedResource();
    threadResource->startThread();
}

/*
 * Stops the thread resource, waiting for the thread to exit.
 */
void GLib::ThreadHandler::stopThread()
{
    SharedResource::LockedPtr<ThreadResource> threadResource
        = getWriteLockedResource();
    threadResource->stopThreadResource();
}
