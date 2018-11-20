#pragma once
#include "SharedResource/Handler.h"
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
     * @brief  Runs a function on this GLib event loop, waiting until the 
     *         function has finished.
     * 
     * @param toRun  A function that needs to run on the GLib event loop.
     */
    void run(const std::function<void()> toRun) const;

    /**
     * @brief  Asynchronously runs a function once on this GLib event loop.
     * 
     * @param toRun   A function that needs to run on the GLib event loop.
     */
    void runAsync(const std::function<void()> toRun) const;

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
    GMainContext* getContext();
    
    /**
     * @brief  Starts the thread resource if it is currently not running.
     */
    void startThread();

    /**
     * @brief  Stops the thread resource without waiting for the thread to exit.
     */
    void stopThread();
};
