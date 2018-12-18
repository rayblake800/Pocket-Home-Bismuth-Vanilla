#include "GLib/ThreadResource.h"
#include "GLib/SmartPointers/SharedContextPtr.h"

/*
 * Creates the thread with an initial GMainContext.
 */
GLib::ThreadResource::ThreadResource
(const juce::Identifier& resourceKey, const SharedContextPtr& context) : 
SharedResource::ThreadResource(resourceKey), 
eventLoop(context), contextCaller(context) { }

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
void GLib::ThreadResource::call(std::function<void()> toCall,
            std::function<void()> onFailure,
            std::function<void()> afterAdding)
{
    if(isThreadRunning())
    {
        notify();
    }
    else
    {
        startThread();
    }
    contextCaller.call(toCall, onFailure, afterAdding);
}

/*
 * Asynchronously run a function once on this GLib event loop.
 */
void GLib::ThreadResource::callAsync(std::function<void()> toCall,
            std::function<void()> onFailure)
{
    if(isThreadRunning())
    {
        notify();
    }
    contextCaller.callAsync(toCall, onFailure);
}

/*
 * Gets the thread's GLib context.
 */
GLib::SharedContextPtr GLib::ThreadResource::getContext()
{
    return eventLoop.getContext();
}

/*
 * Exits the GLib event loop before stopping the thread normally.
 */
void GLib::ThreadResource::stopThreadResource()
{
    contextCaller.callAsync([this]()
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

/*
 * Runs the GLib main loop.
 */
void GLib::ThreadResource::runLoop
(SharedResource::ThreadResource::ThreadLock& lock)
{
    threadLock = &lock;
    eventLoop.runLoop();
    threadLock = nullptr;
}

/*
 * Pauses the event loop whenever window focus is lost.
 */
void GLib::ThreadResource::windowFocusLost()
{
    eventLoop.stopLoop();
}

/*
 * Resumes the event loop whenever window focus is regained.
 */
void GLib::ThreadResource::windowFocusGained()
{
    notify();
}
