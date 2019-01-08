#include "GLib_ThreadResource.h"
#include "GLib_SharedContextPtr.h"

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
    if(eventLoop.runningOnLoop())
    {
        toCall();
    }
    else
    {
        if(juce::Thread::getCurrentThreadId() != getThreadId())
        {
            if(isThreadRunning())
            {
                notify();
            }
            else
            {
                startThread();
            }
        }
        contextCaller.call(toCall, onFailure, afterAdding);
    }
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
 * Wakes the thread if it is currently waiting, and prevents it from waiting 
 * again until after the thread loop restarts.
 */
void GLib::ThreadResource::notifyThread()
{
    waitingAllowed = false;
    notify();
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
    waitingAllowed = true;
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
    notifyThread();
}
    
/*
 * Lets the thread wait while the EventLoop isn't running, rather than stopping 
 * it completely.
 */
bool GLib::ThreadResource::threadShouldWait()
{
    return waitingAllowed;
}
