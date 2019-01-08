#pragma once
/**
 * @file GLib_ThreadResource.h
 * 
 * @brief  Runs a GLib event loop on a shared thread resource. 
 */

#include "SharedResource_ThreadResource.h"
#include "WindowFocus.h"
#include "JuceHeader.h"
#include "GLib_EventLoop.h"
#include "GLib_ContextCaller.h"
#include <gio/gio.h>

namespace GLib { class ThreadResource; }
namespace GLib { class SharedContextPtr; }

/**
 *  On creation, this starts up a GLib event thread to handle events associated
 * with a GMainContext provided on creation. This GMainContext will be set as
 * the thread default context. This context will be available through the 
 * getContext method, so that it can be used to add signal sources and signal
 * handlers to this thread.
 * 
 *  GLib::ThreadResource also provides methods for synchronously or 
 * asynchronously executing code within the thread. These should be used to 
 * handle all interactions with GLib objects tied to the thread context, unless 
 * those objects are explicitly guaranteed to be thread-safe.
 */
class GLib::ThreadResource : public SharedResource::ThreadResource,
    private WindowFocus::Listener
{
public:
    /**
     * @brief  Creates the thread with an initial GMainContext.
     *
     * @param resourceKey  The unique key used to identify the single object
     *                     instance of a ThreadResource subclass.
     *
     * @param context      The event context that will be claimed by this 
     *                     thread. This will be unreferenced when the thread is 
     *                     destroyed.
     */
    ThreadResource(const juce::Identifier& resourceKey, 
            const SharedContextPtr& context);
    
    virtual ~ThreadResource() { }
    
    /**
     * @brief  Checks if this is being executed within the event loop.
     *
     * @return  True if and only if this function is running within the event 
     *          loop.
     */
    bool runningOnLoop();
    
    /**
     * @brief  Runs a function on this GLib event loop, waiting until the 
     *         function has finished.
     *
     * @param toCall        The function that needs to run within the thread's 
     *                      GLib event loop.
     *
     * @param onFailure     An optional function to run if the thread resource 
     *                      is destroyed before the toCall function could run.
     *
     * @param afterAdding   An optional function to run after the pending call
     *                      is scheduled, just before the calling thread is
     *                      forced to wait for the function call.
     */
    void call(std::function<void()> toCall,
            std::function<void()> onFailure = std::function<void()>(),
            std::function<void()> afterAdding = std::function<void()>());
    
    /**
     * @brief  Asynchronously runs a function once on this thread's GLib event 
     *         loop.
     *
     * @param toCall     The function that needs to run within the event loop.
     *
     * @param onFailure  An optional function to run if the thread resource is
     *                   destroyed before the toCall function could run.
     */
    void callAsync(std::function<void()> toCall,
            std::function<void()> onFailure = std::function<void()>());
    
    /**
     * @brief  Gets the thread's context.
     *
     * @return  The GLib context set when the EventLoop was constructed.
     */
    SharedContextPtr getContext();

    /**
     * @brief  Exits the GLib event loop before stopping the thread normally.
     */
    virtual void stopThreadResource() override;

    /**
     * @brief  Wakes the thread if it is currently waiting, and prevents it from
     *         waiting again until after the thread loop restarts.
     */
    void notifyThread();

protected:
    /**
     * @brief  Grants the ThreadResource access to its ThreadLock within the
     *         GLib event loop.
     *
     * @return  The ThreadLock if called within the event loop, nullptr
     *          otherwise.
     */
    SharedResource::ThreadResource::ThreadLock* getThreadLock();
    
private:
    /**
     * @brief  Runs the GLib main loop. 
     */
    virtual void runLoop(SharedResource::ThreadResource::ThreadLock& lock) 
        override;

    /**
     * @brief  Pauses the event loop whenever window focus is lost.
     */
    virtual void windowFocusLost() override;
    
    /**
     * @brief  Resumes the event loop whenever window focus is regained.
     */
    virtual void windowFocusGained() override;
    
    /**
     * @brief  Lets the thread wait while the EventLoop isn't running, rather
     *         than stopping it completely.
     *
     * @return  True if the thread hasn't waited already since the EventLoop
     *          stopped running.
     */
    virtual bool threadShouldWait() override;

    /* Tracks if the thread is allowed to wait. */
    bool waitingAllowed = true;

    /* Used to allow any function running on the thread's EventLoop to access
       its threadLock. */
    ThreadResource::ThreadLock* threadLock = nullptr;
    
    /* Runs the GLib event loop. */
    EventLoop eventLoop;

    /* Executes functions in the event loop. */
    ContextCaller contextCaller;
};
