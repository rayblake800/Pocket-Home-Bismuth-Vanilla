#pragma once
/**
 * @file  GLib_ContextCaller.h
 *
 * @brief  Runs functions synchronously and asynchronously within a GLib event
 *         loop attached to a specific GLib context.
 */

#include "GLib_SharedContextPtr.h"
/* Since the main UI/message thread isn't a Juce thread, standard C++ thread
   libraries need to be used to wait/notify. */
#include <mutex>
#include <condition_variable>
#include "JuceHeader.h"

namespace GLib { class ContextCaller; }

/**
 * @brief  Holds a GMainContext data pointer, and uses it to call functions
 *         within an associated GLib event loop.
 *
 *  The ContextCaller will not run the event loop itself, or ensure that the 
 * GLib context even has an associated EventLoop. That should be handled 
 * elsewhere, probably by a GLib::ThreadResource object.
 */
class GLib::ContextCaller
{
public:
    /**
     * @brief  Initializes the ContextCaller, setting its GLib main context.
     *
     * @param context  A reference to the context where the ContextCaller
     *                 will schedule function calls.
     */
    ContextCaller(const SharedContextPtr& context);

    /**
     * @brief  Cancels all pending calls when the ContextCaller is destroyed.
     */
    virtual ~ContextCaller();
    
    /**
     * @brief  Schedules a function call to run asynchronously within the
     *         context's event loop.
     *
     * @param toCall     The function that needs to run within the event loop.
     *
     * @param onFailure  An optional function to run if the ContextCaller is
     *                   destroyed before the toCall function could run.
     */
    void callAsync(std::function<void()> toCall,
            std::function<void()> onFailure = std::function<void()>());

    /**
     * @brief  Schedules a function call to run within the context's event loop,
     *         then waits for the function to execute.
     *
     * @param toCall        The function that needs to run within the context's
     *                      event loop.
     *
     * @param onFailure     An optional function to run if the ContextCaller is
     *                      destroyed before the toCall function could run.
     *
     * @param afterAdding   An optional function to run after the pending call
     *                      is scheduled, just before the calling thread is
     *                      forced to wait for the function call.
     */
    void call(std::function<void()> toCall,
            std::function<void()> onFailure = std::function<void()>(),
            std::function<void()> afterAdding = std::function<void()>());

private:

    /**
     * @brief  All data needed to call a function within a GLib context's main
     *         event loop.
     */
    struct CallData
    {
        /* The ContextCaller that scheduled the call, needed to delete this
           CallData after running the function. */
        ContextCaller* caller;

        /* The function that needs to be called on the event loop */
        std::function<void()> toCall;

        /* A function to run if toCall could not be run.  This value can be an
           invalid function if no onFailure call is needed. */
        std::function<void()> onFailure;

        /* The GSource used to schedule the call on the GMainLoop, and to remove
           completed calls. */
        GSource* callSource;

        /* Prevents synchronous calls from running until the calling thread 
           starts waiting. */
        std::mutex* callerMutex = nullptr;
        /* Used to notify a waiting thread that its function call is complete.*/
        std::condition_variable* callPending = nullptr;
    };
    
    /**
     * @brief  Adds a function to the GMainContext so it will execute on the 
     *         event loop.
     * 
     * @param call         The function to run on the event loop..
     * 
     * @param onFailure    The function to run if the call() function couldn't
     *                     run.
     * 
     * @param callerMutex  If this value is non-null, the EventLoop will
     *                     lock it while running this call.
     * 
     * @param callPending  If this value is non-null, the EventLoop will
     *                     use it and callerMutex to wake up the calling
     *                     thread after running the call.
     */
    void addAndInitCall(std::function<void()> call,
            std::function<void()> onFailure,
            std::mutex* callerMutex = nullptr,
            std::condition_variable* callPending = nullptr);
    /**
     * @brief  A callback function used to execute arbitrary functions within
     *         the context's GMainLoop.
     *
     * @param runData  Holds the data needed to call the function and possibly
     *                 notify the function's source.
     *
     * @return         False.
     *                 TODO: Find and document why this always should return
     *                       false. I know I had a good reason for this, and it
     *                       works correctly, but I forgot to document why, and
     *                       GLib callback function documentation is poorly
     *                       documented and hard to find.
     */
    static gboolean runAsync(CallData* runData);

    /* Holds pending calls, ensuring CallData is not leaked if the EventLoop
       is destroyed before a call can run. */
    juce::OwnedArray<CallData, juce::CriticalSection> pendingCalls;

    /* The shared context pointer used by the ContextCaller to schedule calls.*/
    SharedContextPtr contextPtr;
};
