#pragma once
/**
 * @file  GLib_SharedThread.h
 * 
 * @brief  Runs a GLib event loop on a shared thread resource. 
 */

#include "WindowFocus.h"
#include "GLib_EventLoop.h"
#include "GLib_ContextCaller.h"
#include "GLib_SharedContextPtr.h"
#include "SharedResource_Thread_Thread.h" 
#include "JuceHeader.h" 
#include <gio/gio.h>

namespace GLib { class SharedThread; }

/**
 *  On creation, this starts up a GLib event thread to handle events associated
 * with a GMainContext provided on creation. This GMainContext will be set as
 * the thread default context. This context will be available through the 
 * getContext method, so that it can be used to add signal sources and signal
 * handlers to this thread.
 * 
 *  GLib::SharedThread also provides methods for synchronously or 
 * asynchronously executing code within the thread. These should be used to 
 * handle all interactions with GLib objects tied to the thread context, unless 
 * those objects are explicitly guaranteed to be thread-safe.
 *
 *  SharedThread is an abstract interface, meant to be implemented as a 
 * SharedResource Resource or Module.
 */
class GLib::SharedThread : public SharedResource::Thread::Thread,
    private WindowFocus::Listener
{
public:
    /**
     * @brief  Creates the thread with an initial GMainContext.
     *
     * @param threadName  The name to give to the object's thread.
     *
     * @param context     The GLib context the thread should use.
     */
    SharedThread(const juce::String threadName, const SharedContextPtr context);
    
    virtual ~SharedThread() { }
    
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
     * @brief  Runs an asynchronous callback function on the thread after safely 
     *         acquiring the thread's resource lock.
     *
     *  Only call this method from within an asynchronous callback function that
     * is already running within the thread's event loop.
     *
     *  If an asynchronous callback function attempts to acquire the thread's
     * resource lock normally, the thread will remain inactive until the lock is
     * acquired. This will cause a deadlock will occur if another thread is 
     * trying to use the SharedThread::call method while it holds the resource 
     * lock.
     * 
     *  If lockForAsyncCallback fails to immediately acquire the lock, instead
     * of waiting for the lock to be released, it postpones the asynchronous
     * action so the thread that already holds the lock can run code on the 
     * SharedThread if necessary. The lockedAction will be rescheduled as many
     * times as is necessary until it can finally acquire the lock.
     *
     * @param lockType       Selects whether the thread's Resource should be
     *                       locked for reading or for writing.
     *
     * @param lockedAction   An asynchronous action that should run once the
     *                       lock is acquired.
     */
    void lockForAsyncCallback(const SharedResource::LockType lockType,
            const std::function<void()> lockedAction);
    
    /**
     * @brief  Gets the thread's context.
     *
     * @return  The GLib context set when the EventLoop was constructed.
     */
    SharedContextPtr getContext() const;

    /**
     * @brief  Exits the GLib event loop before stopping the thread normally.
     *
     *  The caller must aquire the thread resource's lock for writing before
     * calling this method. The lock will be unlocked and then relocked
     * as the thread exits.
     */
    virtual void stopResourceThread() override;

    /**
     * @brief  Wakes the thread if it is currently waiting, and prevents it from
     *         waiting again until after the thread loop restarts.
     */
    void notifyThread();

    /**
     * @brief  Grants the SharedThread access to its ThreadLock within the
     *         GLib event loop.
     *
     * @return  The ThreadLock if called within the event loop, nullptr
     *          otherwise.
     */
    SharedResource::Thread::Lock* getThreadLock();
    
private:
    /**
     * @brief  Runs the GLib main loop. 
     */
    virtual void runLoop(SharedResource::Thread::Lock& lock) override;

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
    SharedResource::Thread::Lock* threadLock = nullptr;
    
    /* Runs the GLib event loop. */
    EventLoop eventLoop;

    /* Executes functions in the event loop. */
    ContextCaller contextCaller;
};
