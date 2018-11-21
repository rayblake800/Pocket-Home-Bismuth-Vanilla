#pragma once
/* Since the main UI/message thread isn't a Juce thread, standard C++ thread
   libraries need to be used to wait/notify. */
#include <mutex>
#include <condition_variable>
#include "gio/gio.h"
#include "WindowFocus.h"
#include "JuceHeader.h"
#include "GLib/Object.h"

/**
 * @file  GLib/EventLoop.h
 * 
 * @brief  Manages a GMainLoop* GLib event loop.
 */
class GLib::EventLoop 
{
public:
    /**
     * @brief  Creates the event loop with an initial GMainContext.
     *
     * @param context  The context that will be claimed by this thread. This 
     *                 will be unreferenced when the thread is destroyed.
     */
    EventLoop(GMainContext* context);
    
    /**
     * @brief  Unreferences the loop's GMainLoop and GMainContext, and unlocks
     *         any unhandled pending event loop calls.
     */
    virtual ~EventLoop();    

    /**
     * @brief  Checks if this is being executed within the event loop.
     *
     * @return  True if and only if this function is running within the event 
     *          loop.
     */
    bool runningOnLoop();
    
    /**
     * @brief  Gets the event loop's context.
     *
     * @return  The GMainContext* set when the EventLoop was constructed.
     */
    GMainContext* getContext();
    
    /*
     * @brief  Runs the GLib main loop. 
     */
    void runLoop();
    
    /**
     * @brief  Terminates the GLib main loop.
     */
    void stopLoop();

private:
    /*
     * @brief  Holds all data needed to handle a function call passed in by the 
     *         call() or callAsync() methods.
     */
    struct CallData
    {
        EventLoop* eventLoop;
        std::function<void() > call;
        GSource* callSource;
        std::mutex* callerMutex;
        std::condition_variable* callPending;
    };
    
    /**
     * @brief  Adds a function to the GMainContext so it will execute on the 
     *         event loop.
     * 
     * @param call         The function to run.
     * 
     * @param callerMutex  If this value is non-null, the EventLoop will
     *                     lock it while running this call.
     * 
     * @param callPending  If this value is non-null, the EventLoop will
     *                     use it and callerMutex to wake up the calling
     *                     thread after running the call.
     */
    void addAndInitCall(std::function<void() > call,
            std::mutex* callerMutex = nullptr,
            std::condition_variable* callPending = nullptr);

    /**
     * @brief  A callback function used to execute arbitrary functions on the 
     *         GMainLoop.
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

    /* GLib thread/event loop context object. */
    GMainContext* context = nullptr;

    /* The GLib event loop managed by this object. */
    GMainLoop* mainLoop = nullptr;
};
