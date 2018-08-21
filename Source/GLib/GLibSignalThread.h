/**
 * @file GLibSignalThread.h
 * 
 * @brief GLibSignalThread runs the main event loop used to send and receive 
 *        GLib signals.
 * 
 * On creation, this starts up a GLib event thread to handle events on the
 * global-default GMainContext, where libNM and other GLib libraries send
 * signals.  When GLib signal sources and signal handlers are added to the 
 * default context, they will use this thread to send signals, and run callback
 * functions in response to those signals.
 * 
 * All GLibSignalThreads share the global event thread, and provide methods for 
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
#include "GLibThread.h"
#include "ResourceManager.h"
#include "JuceHeader.h"



class GLibSignalThread : private ResourceManager
{
public:
    GLibSignalThread();

    virtual ~GLibSignalThread() { }
    
    /**
     * Returns true if it's being executed on the GLib default event thread.
     */
    bool runningOnGLibThread();

    /**
     * Asynchronously run a function once on the default GLib event loop.
     * 
     * @param fn   A function that needs to run on the GLib event loop.
     */
    void gLibCallAsync(std::function<void() > fn);


    /**
     * Run a function on the GLib event loop, waiting until the function
     * has finished.
     * 
     * @param fn  A function that needs to run on the GLib event loop.
     */
    void gLibCall(std::function<void() > fn);


private:

    class GLibDefaultThread : public SharedResource, public GLibThread
    {
    public:

        /**
         * Initializes and starts the main GLib event loop on its own thread.
         */
        GLibDefaultThread();
        
        virtual ~GLibDefaultThread() { }
    };

    static juce::ScopedPointer<SharedResource> globalThread;
    static juce::ReadWriteLock threadLock;
};