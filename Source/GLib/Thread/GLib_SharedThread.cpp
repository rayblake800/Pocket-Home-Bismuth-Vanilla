#include "GLib_SharedThread.h"

/*
 * Creates the thread with an initial GMainContext.
 */
GLib::SharedThread::SharedThread
(const juce::String threadName, const SharedContextPtr context) :
    Thread(threadName), eventLoop(context), contextCaller(context) { }
    
/*
 * Checks if this is being executed within the event loop.
 */
bool GLib::SharedThread::runningOnLoop()
{
    return eventLoop.runningOnLoop();
}
    
/*
 * Runs a function on this GLib event loop, waiting until the function has 
 * finished.
 */
void GLib::SharedThread::call(
        std::function<void()> toCall, 
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
                startResourceThread();
            }
        }
        contextCaller.call(toCall, onFailure, afterAdding);
    }
}

/*
 * Asynchronously runs a function once on this thread's GLib event loop.
 */
void GLib::SharedThread::callAsync(std::function<void()> toCall,
        std::function<void()> onFailure)
{
    if(isThreadRunning())
    {
        notify();
    }
    contextCaller.callAsync(toCall, onFailure);
}

/*
 * Runs an asynchronous callback function on the thread after safely acquiring 
 * the thread's resource lock.
 */
void GLib::SharedThread::lockForAsyncCallback(
        const SharedResource::LockType lockType,
        const std::function<void()> lockedAction)
{

    SharedResource::Thread::Lock* threadLock = getThreadLock();
    /* If this assertion fails, the method is being called outside the event
     * loop. */
    jassert(threadLock != nullptr);
    const bool acquiredLock = (lockType == SharedResource::LockType::read) ?
            threadLock->tryEnterRead() : threadLock->tryEnterWrite();
    if(acquiredLock)
    {
        lockedAction();
        if(lockType == SharedResource::LockType::read)
        {
            threadLock->exitRead();
        }
        else
        {
            threadLock->exitWrite();
        }
    }
    else
    {
        callAsync([this, lockType, lockedAction]()
        {
            lockForAsyncCallback(lockType, lockedAction);
        });
    }
}

/*
 * Gets the thread's context.
 */
GLib::SharedContextPtr GLib::SharedThread::getContext() const
{
    return eventLoop.getContext();
}

/*
 * Exits the GLib event loop before stopping the thread normally.
 */
void GLib::SharedThread::stopResourceThread()
{
    if(isThreadRunning())
    {
        signalThreadShouldExit();
        contextCaller.call([this]()
        {
            eventLoop.stopLoop();
        });
        Thread::stopResourceThread();
    }
}
    
/*
 * Wakes the thread if it is currently waiting, and prevents it from waiting 
 * again until after the thread loop restarts.
 */
void GLib::SharedThread::notifyThread()
{
    waitingAllowed = false;
    notify();
}

/*
 * Grants the SharedThread access to its ThreadLock within the GLib event loop.
 */
SharedResource::Thread::Lock* GLib::SharedThread::getThreadLock()
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
void GLib::SharedThread::runLoop(SharedResource::Thread::Lock& lock)
{
    threadLock = &lock;
    eventLoop.runLoop();
    threadLock = nullptr;
    waitingAllowed = !threadShouldExit();
}

/*
 * Pauses the event loop whenever window focus is lost.
 */
void GLib::SharedThread::windowFocusLost()
{
    eventLoop.stopLoop();
}

/*
 * Resumes the event loop whenever window focus is regained.
 */
void GLib::SharedThread::windowFocusGained()
{
    notify();
}

/*
 * Lets the thread wait while the EventLoop isn't running, rather than stopping 
 * it completely.
 */
bool GLib::SharedThread::threadShouldWait()
{
    return waitingAllowed;
}
