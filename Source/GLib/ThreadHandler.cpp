#include "GLib/ThreadHandler.h"
#include "GLib/ThreadResource.h"
#include "GLib/SmartPointers/SharedContextPtr.h"
/*
 * Permanently connects the ThreadHandler to a single thread resource on
 * construction.
 */
GLib::ThreadHandler::ThreadHandler(const juce::Identifier& resourceKey,
        const std::function<ThreadResource*()> createResource) :
SharedResource::Handler<ThreadResource> (resourceKey, createResource) { }

/*
 * Calls a function on this GLib event loop, waiting until the function has
 * finished.
 */
void GLib::ThreadHandler::call(const std::function<void()> toCall,
        const std::function<void()> onFailure) const
{
    SharedResource::LockedPtr<ThreadResource> glibThread
        = getWriteLockedResource();
    glibThread->call(toCall, onFailure);
}

/*
 * Asynchronously calls a function once on this GLib event loop.
 */
void GLib::ThreadHandler::callAsync(const std::function<void()> toCall,
        const std::function<void()> onFailure) const
{
    SharedResource::LockedPtr<ThreadResource> glibThread
        = getWriteLockedResource();
    glibThread->callAsync(toCall, onFailure);
}

/*
 * Checks if the thread resource is currently running.
 */
bool GLib::ThreadHandler::isThreadRunning() const
{
    SharedResource::LockedPtr<ThreadResource> glibThread
        = getReadLockedResource();
    return glibThread->isThreadResourceRunning();
}

/*
 * Gets the GLib event thread's context.
 */
GLib::SharedContextPtr GLib::ThreadHandler::getContext()
{
    SharedResource::LockedPtr<ThreadResource> glibThread
        = getReadLockedResource();
    return glibThread->getContext();
}

/*
 * Starts the thread resource if it is currently not running.
 */
void GLib::ThreadHandler::startThread()
{
    SharedResource::LockedPtr<ThreadResource> glibThread
        = getWriteLockedResource();
    glibThread->startThreadResource();
}

/*
 * Stops the thread resource, waiting for the thread to exit.
 */
void GLib::ThreadHandler::stopThread()
{
    SharedResource::LockedPtr<ThreadResource> glibThread
        = getWriteLockedResource();
    glibThread->stopThreadResource();
}
