#pragma once
/**
 * @file  Glib_ThreadHandler.h
 *
 * @brief  Provides access to a GLib thread resource.
 */

#include "SharedResource_Handler.h"
#include "GLib_SharedContextPtr.h"

namespace GLib { template <class ResourceType> class ThreadHandler; }
namespace GLib { class SharedContextPtr; }

template <class HandlerType>
class GLib::ThreadHandler : public HandlerType
{
public:
    /**
     * @brief  Permanently connects the ThreadHandler to a single thread
     *         resource on construction.
     *
     * @param resourceKey     The ThreadResource's unique SharedResource key.
     *
     * @param 
     */
    ThreadHandler(const juce::Identifier& resourceKey) :
        HandlerType(resourceKey) { }

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
        auto glibThread = HandlerType::getWriteLockedResource();
        glibThread->call(toCall, onFailure, [&glibThread]()
        {
            glibThread->notifyThread();
            glibThread.unlock();
        });
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
        auto glibThread = HandlerType::getWriteLockedResource();
        glibThread->callAsync(toCall, onFailure);
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
        auto glibThread = HandlerType::getReadLockedResource();
        return glibThread->isThreadRunning();
    }
    
    /**
     * @brief  Gets the GLib event thread's context.
     *
     * @return  The GMainContext* set when the thread resource was constructed.
     */
    SharedContextPtr getContext()
    {
        auto glibThread = HandlerType::getReadLockedResource();
        return glibThread->getContext();
    }
    
    /**
     * @brief  Starts the thread resource if it is currently not running.
     */
    void startThread()
    {
        auto glibThread = HandlerType::getWriteLockedResource();
        glibThread->startThreadResource();
    }

    /**
     * @brief  Stops the thread resource, waiting for the thread to exit.
     */
    void stopThread()
    {
        auto glibThread = HandlerType::getWriteLockedResource();
        glibThread->stopThreadAndWait();
    }
};
