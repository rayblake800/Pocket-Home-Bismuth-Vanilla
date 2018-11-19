#include "GLib/ThreadResource.h"

/*
 * Creates the thread with an initial GMainContext.
 */
GLib::ThreadResource::ThreadResource
(const juce::Identifier& resourceKey, GMainContext* context) : 
SharedResource::ThreadResource(resourceKey), 
eventLoop(context)
{  
    jassert(context != nullptr);
}

/*
 * Checks if this is being executed within the event loop.
 */
bool GLib::ThreadResource::runningOnLoop()
{
    return eventLoop.runningOnLoop(); 
}

/*
 * Run a function on this GLib event loop, waiting until the function has 
 * finished.
 */
void GLib::ThreadResource::call(std::function<void()> fn)
{
    if(isThreadRunning())
    {
        notify();
    }
    else
    {
        startGLibThread();
    }
    eventLoop.call(fn);
}

/*
 * Asynchronously run a function once on this GLib event loop.
 */
void GLib::ThreadResource::callAsync(std::function<void()> fn)
{
    if(isThreadRunning())
    {
        notify();
    }
    eventLoop.callAsync(fn);
}

/*
 * Returns the thread default context that was set when this thread was
 * created.
 */
GMainContext* GLib::ThreadResource::getContext()
{
    return eventLoop.getContext();
}

/*
 * Exits the GLib event loop before stopping the thread normally.
 */
void GLib::ThreadResource::stopThreadResource()
{
    eventLoop.callAsync([this]()
        {
            eventLoop.stopLoop();
        });
    SharedResource::ThreadResource::signalThreadShouldExit();
}

/*
 * Grants the ThreadResource access to its ThreadLock within the GLib event 
 * loop.
 */
SharedResource::ThreadResource::ThreadLock* 
GLib::ThreadResource::getThreadLock()
{
    if(eventLoop.runningOnLoop())
    {
        return threadLock;
    }
    return nullptr;
}

/**
 * Runs the GLib main loop.
 */
void GLib::ThreadResource::runLoop
(SharedResource::ThreadResource::ThreadLock& lock)
{
    threadLock = &lock;
    eventLoop.runLoop();
    threadLock = nullptr;
}

/**
 * Pause the event loop whenever window focus is lost.
 */
void GLib::ThreadResource::windowFocusLost()
{
    eventLoop.stopLoop();
}

/*
 * Resume the event loop whenever window focus is regained.
 */
void GLib::ThreadResource::windowFocusGained()
{
    notify();
}
