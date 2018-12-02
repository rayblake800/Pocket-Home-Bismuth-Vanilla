#pragma once
#include "GLib/ThreadResource.h"
#include "SharedResource/Handler.h"

/**
 * @file  Glib/ThreadHandler.h
 *
 * @brief  Provides access to a GLib thread resource.
 */
template<class GLibThreadType>
class GLib::ThreadHandler : public SharedResource::Handler<GLibThreadType>
{
public:
    /**
     * @brief  Permanently connects the ThreadHandler to a single thread
     *         resource on construction.
     */
    ThreadHandler() :
    SharedResource::Handler<GLibThreadType>(GLibThreadType::resourceKey,
        []()
        {
            return new GLibThreadType();
        }) { }

    virtual ~ThreadHandler() { }

    /**
     * @brief  Calls a function on this GLib event loop, waiting until the 
     *         function has finished.
     * 
     * @param toCall     A function that needs to be called on the GLib event 
     *                   loop.
     *
     * @param onFailure  An optional function to call if accessing the event
     *                   loop failed.
     */
    void call(const std::function<void()> toCall, 
            const std::function<void()> onFailure 
                = std::function<void()>()) const
    {
        SharedResource::LockedPtr<GLibThreadType> threadResource
            = SharedResource::Handler<GLibThreadType>::getWriteLockedResource();
        threadResource->call(toCall, onFailure, [&threadResource]() 
                { threadResource.unlock(); });
    }

    /**
     * @brief  Asynchronously calls a function once on this GLib event loop.
     * 
     * @param toCall   A function that needs to be called on the GLib event loop.
     *
     * @param onFailure  An optional function to call if accessing the event
     *                   loop failed.
     */
    void callAsync(const std::function<void()> toCall,
            const std::function<void()> onFailure 
                = std::function<void()>()) const
    {
        SharedResource::LockedPtr<GLibThreadType> threadResource
            = SharedResource::Handler<GLibThreadType>::getWriteLockedResource();
        threadResource->callAsync(toCall, onFailure);
    }

    /**
     * @brief  Checks if the thread resource is currently running.
     *
     * @return  Whether the thread is currently running. This will return true
     *          even if the thread is waiting and not running the GLib event 
     *          loop.
     */
    bool isThreadRunning() const
    {
        SharedResource::LockedPtr<ThreadResource> threadResource
            = SharedResource::Handler<GLibThreadType>::getReadLockedResource();
        return threadResource->isThreadRunning();
    }
    
    /**
     * @brief  Gets the GLib event thread's context.
     *
     * @return  The GMainContext* set when the thread resource was constructed.
     */
    SharedContextPtr getContext()
    {
        SharedResource::LockedPtr<ThreadResource> threadResource
            = SharedResource::Handler<GLibThreadType>::getReadLockedResource();
        return threadResource->getContext();
    }
    
    /**
     * @brief  Starts the thread resource if it is currently not running.
     */
    void startThread()
    {
        SharedResource::LockedPtr<ThreadResource> threadResource
            = SharedResource::Handler<GLibThreadType>::getWriteLockedResource();
        threadResource->startThread();
    }

    /**
     * @brief  Stops the thread resource, waiting for the thread to exit.
     */
    void stopThread()
    {
        SharedResource::LockedPtr<ThreadResource> threadResource
            = SharedResource::Handler<GLibThreadType>::getWriteLockedResource();
        threadResource->stopThreadResource();
    }
};
