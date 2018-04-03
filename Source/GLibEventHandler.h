/**
 * @file GLibEventHandler.h
 * 
 * @brief GLibEventHandler runs an event loop used to send and receive GLib 
 *         signals.
 * 
 * This manages the thread used to run all interactions between pocket-home and
 * the wifi and bluetooth devices over DBus. It provides a context for adding 
 * GLib signal sources and signal handlers, and methods for synchronously and
 * asynchronously running functions on the GLib thread.  
 * 
 * This should only be used when interacting with GLib signal sources and
 * handlers.  For general purpose asynchronous execution, use the Juce
 * MessageManager.
 */
#pragma once
#include "gio/gio.h"
#include "ResourceManager.h"
#include "JuceHeader.h"

class GLibEventHandler : private ResourceManager
{
public:
    /**
     * Initializes and starts the main GLib event loop on its own thread.
     */
    GLibEventHandler();

    /**
     * Stops the event loop thread and cleans up all GLib resources.
     */
    virtual ~GLibEventHandler();

    /**
     * Asynchronously run a function once on the GLib event loop.
     * 
     * @param fn   A function that needs to run on the GLib event loop.
     */
    void callAsyncGLib(std::function<void() > fn);


    /**
     * Run a function on the GLib event loop, blocking until the function
     * finishes.
     * 
     * @param fn  A function that needs to run on the GLib event loop.
     */
    void callGLib(std::function<void() > fn);

private:

    class GLibThread : public Thread, public ResourceManager::SharedResource
    {
    public:
        GLibThread();

        virtual ~GLibThread();

        GSource* addAndInitCall(std::function<void() > call);

        /**
         * Check if a function call scheduled to run on the thread is still
         * waiting to run.
         * 
         * @param callSource
         * @return 
         */
        bool callIsPending(GSource* callSource);
    private:
        /**
         * Runs the GLib main loop.
         */
        void run() override;

        struct CallbackData
        {
            std::function<void() > call;
            GSource* callSource;
            Array<GSource*, CriticalSection>* sourceTracker;
        };

        /**
         * Callback function used to execute arbitrary functions on the 
         * GMainLoop.
         */
        static gboolean runAsync(CallbackData* runData);



        GMainLoop* mainLoop = nullptr;
        GMainContext* context = nullptr;
        Array<GSource*, CriticalSection> gSourceTracker;
    };

    //Shared resource instance and lock
    static ScopedPointer<SharedResource> threadResource;
    static CriticalSection threadLock;
};