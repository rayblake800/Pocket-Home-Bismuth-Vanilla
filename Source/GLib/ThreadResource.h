#pragma once
#include "gio/gio.h"
#include "WindowFocus.h"
#include "JuceHeader.h"
#include "GLib/EventLoop.h"
#include "SharedResource/ThreadResource.h"

/**
 * @file GLib/ThreadResource.h
 * 
 * @brief  Runs a GLib event loop on a shared thread resource. 
 * 
 * On creation, this starts up a GLib event thread to handle events associated
 * with a GMainContext provided on creation. This GMainContext will be set as
 * the thread default context. This context will be available through the 
 * getContext method, so that it can be used to add signal sources and signal
 * handlers to this thread.
 * 
 * GLib::ThreadResource also provides methods for synchronously or 
 * asynchronously executing code within the thread. These should be used to 
 * handle all interactions with GLib object tied to the thread context, unless 
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
    ThreadResource(const juce::Identifier& resourceKey, GMainContext* context);
    
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
     * @param fn  A function that needs to run on the GLib event loop.
     */
    void call(std::function<void()> fn);
    
    /**
     * @brief  Asynchronously runs a function once on this GLib event loop.
     * 
     * @param fn   A function that needs to run on the GLib event loop.
     */
    void callAsync(std::function<void()> fn);
    
    /**
     * @brief  Gets the event loop's context.
     *
     * @return  The GMainContext* set when the EventLoop was constructed.
     */
    GMainContext* getContext();

    /**
     * @brief  Exits the GLib event loop before stopping the thread normally.
     */
    virtual void stopThreadResource() override;

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
    
    /*
     * @brief  Resumes the event loop whenever window focus is regained.
     */
    virtual void windowFocusGained() override;
    
    /**
     * @brief  Lets the thread wait while the EventLoop isn't running, rather
     *         than stopping it completely.
     *
     * @return  True, so the ThreadResource sleeps when the EventLoop finishes. 
     */
    virtual bool threadShouldWait() override;

    /**
     * Starts the GLib thread, then waits until the thread is running and
     * the thread context and main loop are initialized. This function locks the
     * threadStateLock for writing.
     * 
     * @return  True if the thread started successfully, false if the
     *          GLibThread is being destroyed.
     */
    bool startGLibThread();
    
    /* Runs the GLib event loop. */
    EventLoop eventLoop;

    /* Stores the ThreadLock so it can be accessed within the event loop. */
    SharedResource::ThreadResource::ThreadLock* threadLock = nullptr;
};
