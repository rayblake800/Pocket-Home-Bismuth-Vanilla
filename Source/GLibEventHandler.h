/**
 * @file GLibEventHandler.h
 * 
 * @brief GLibEventHandler runs an event loop used to send and receive GLib 
 *         signals.
 * 
 * On creation, this starts up a GLib event thread with its own context.  It
 * provides methods for synchronously and asynchronously running functions on 
 * the GLib thread. Within these methods, the thread context can be acquired
 * with g_main_context_get_thread_default, and new signal sources and signal
 * handlers may be added to the context. 
 * 
 * This should only be used when interacting with GLib signal sources and
 * handlers.  For general purpose asynchronous execution, use the Juce
 * MessageManager.
 */
#pragma once
#include "gio/gio.h"
#include "JuceHeader.h"

class GLibEventHandler : private Thread
{

protected:
    /**
     * Initializes and starts the main GLib event loop on its own thread.
     */
    GLibEventHandler();

public:
    /**
     * Stops the event loop thread and cleans up all GLib resources.
     */
    virtual ~GLibEventHandler();


    /**
     * Asynchronously run a function once on the GLib event loop.
     * 
     * @param fn   A function that needs to run on the GLib event loop.
     */
    void gLibCallAsync(std::function<void() > fn);


    /**
     * Run a function on the GLib event loop, yielding until the function
     * has finished.
     * 
     * @param fn  A function that needs to run on the GLib event loop.
     */
    void gLibCall(std::function<void() > fn);

private:
    /**
     * Adds a function to the GMainContext so it will execute on the event
     * thread.
     * 
     * @param call  The function to run.
     * 
     * @return      the GSource pointer identifying the function.  This can
     *               be used to check 
     */
    GSource* addAndInitCall(std::function<void() > call);


    /**
     * Runs the GLib main loop.
     */
    void run() override;

    struct CallData
    {
        std::function<void() > call;
        GSource* callSource;
        Array<GSource*, CriticalSection>* sourceTracker;
    };

    /**
     * Callback function used to execute arbitrary functions on the 
     * GMainLoop.
     */
    static gboolean runAsync(CallData* runData);

    GMainLoop* mainLoop = nullptr;
    GMainContext* context = nullptr;

    Array<GSource*, CriticalSection> gSourceTracker;
};