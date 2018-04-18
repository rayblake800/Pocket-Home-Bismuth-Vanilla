/**
 * @file GLibSignalHandler.h
 * 
 * @brief GLibSignalHandler runs the event loop used to send and receive GLib 
 *         signals.
 * 
 * On creation, this starts up a GLib event thread to handle events on the
 * global-default GMainContext, where libNM and other GLib libraries send
 * signals.  When GLib signal sources and signal handlers are added to the 
 * default context, they will use this thread to send signals, and run callback
 * functions in response to those signals.
 * 
 * All GLibSignalHandler share the global event thread, and provide methods for 
 * synchronously and asynchronously running functions on the thread. Signal 
 * sources and signal handlers can safely be added and removed from any thread,
 * but any other interaction with GLib objects should be handled on this thread.
 * 
 * This should only be used when interacting with GLib signal sources and
 * handlers.  For general purpose asynchronous execution, use the Juce
 * MessageManager.
 */
#pragma once
//Since the main UI/message thread isn't a Juce thread, standard C++ thread
// libraries need to be used to wait/notify
#include <mutex>
#include <condition_variable>

#include "gio/gio.h"
#include "ResourceManager.h"
#include "JuceHeader.h"



class GLibSignalHandler : private ResourceManager
{
public:
    GLibSignalHandler();

    virtual ~GLibSignalHandler() { }
    
    /**
     * Returns true if it's being executed on the GLib event thread.
     */
    bool runningOnGLibThread();

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

    class GLibThread : public SharedResource, public Thread
    {
    public:

        /**
         * Initializes and starts the main GLib event loop on its own thread.
         */
        GLibThread();

        /**
         * Stops the event loop thread and cleans up all GLib resources.
         */
        virtual ~GLibThread();

        /**
         * Adds a function to the GMainContext so it will execute on the event
         * thread.
         * 
         * @param call         The function to run.
         * 
         * @param callerMutex  If this value is non-null, the event thread will
         *                     lock it while running this call.
         * 
         * @param callPending  If this value is non-null, the event thread will
         *                     use it and callerMutex to wake up the calling
         *                     thread after running the call.
         */
        void addAndInitCall(std::function<void() > call,
                std::mutex* callerMutex = nullptr,
                std::condition_variable* callPending = nullptr);

        /**
         * Runs the GLib main loop.
         */
        void run() override;
        
        struct CallData
        {
            std::function<void() > call;
            GSource* callSource;
            std::mutex* callerMutex;
            std::condition_variable* callPending;
        };
        
    private:

        /**
         * Callback function used to execute arbitrary functions on the 
         * GMainLoop.
         */
        static gboolean runAsync(CallData* runData);

        GMainLoop* mainLoop = nullptr;
    };

    static ScopedPointer<SharedResource> globalThread;
    static CriticalSection threadLock;
};