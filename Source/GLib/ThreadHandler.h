#pragma once
#include "SharedResource/Handler.h"
#include "GLib/SmartPointers/SharedContextPtr.h"
#include "GLib/GLib.h"

class GLib::ThreadHandler : public SharedResource::Handler<ThreadResource>
{
public:
    /**
     * @brief  Permanently connects the ThreadHandler to a single thread
     *         resource on construction.
     *
     * @param resourceKey  The unique key identifying a GLib::ThreadResource.
     */
    ThreadHandler(const juce::Identifier& resourceKey);

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
                = std::function<void()>()) const;

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
                = std::function<void()>()) const;

    /**
     * @brief  Checks if the thread resource is currently running.
     *
     * @return  Whether the thread is currently running. This will return true
     *          even if the thread is waiting and not running the GLib event 
     *          loop.
     */
    bool isThreadRunning() const;
    
    /**
     * @brief  Gets the GLib event thread's context.
     *
     * @return  The GMainContext* set when the thread resource was constructed.
     */
    SharedContextPtr getContext();
    
    /**
     * @brief  Starts the thread resource if it is currently not running.
     */
    void startThread();

    /**
     * @brief  Stops the thread resource, waiting for the thread to exit.
     */
    void stopThread();
};
