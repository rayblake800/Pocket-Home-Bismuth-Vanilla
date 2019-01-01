#pragma once
/**
 * @file  GLib/EventLoop.h
 * 
 * @brief  Manages a GMainLoop* GLib event loop.
 */

#include "GLib/SmartPointers/SharedContextPtr.h"
#include "GLib/SmartPointers/LoopPtr.h"
#include "JuceHeader.h"
#include <gio/gio.h>

namespace GLib { class EventLoop; }

class GLib::EventLoop 
{
public:
    /**
     * @brief  Creates the event loop with an initial GMainContext.
     *
     * @param context  A pointer holding the context that will be claimed by 
     *                 this thread. 
     */
    EventLoop(const SharedContextPtr& context);
    
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
     * @return  The GLib context set when the EventLoop was constructed.
     */
    SharedContextPtr getContext();
    
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
    SharedContextPtr context;

    /* The GLib event loop managed by this object. */
    const LoopPtr eventLoop;
};
