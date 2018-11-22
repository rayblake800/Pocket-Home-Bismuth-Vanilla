#pragma once
#include <gio/gio.h>
#include "JuceHeader.h"
#include "GLib/SmartPointers/MainContextPtr.h"
#include "GLib/SmartPointers/MainLoopPtr.h"

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
     * @brief  Ensures the loop has stopped before it is destroyed.

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
    /* GLib thread/event loop context object. */
    const MainContextPtr context;

    /* The GLib event loop managed by this object. */
    const MainLoopPtr eventLoop;
};
