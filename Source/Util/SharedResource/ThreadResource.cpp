#include "ThreadResource.h"

/* Number of milliseconds to wait before forcibly terminating the thread: */
static const constexpr int timeoutMilliseconds = 1000;

/*
 * Creates a new ThreadResource.
 */
ThreadResource::ThreadResource(const juce::Identifier& resourceKey) :
    SharedResource(resourceKey), Thread(resourceKey.toString()) { }

/*
 * Ensures the thread stops running before it is destroyed.
 */
ThreadResource::~ThreadResource()
{
    if(isThreadRunning())
    {
        stopThread(timeoutMilliseconds);
    }
}

/*
 * Creates a ThreadLock tied to a single thread resource.
 */
ThreadResource::ThreadLock::ThreadLock(const juce::Identifier& resourceKey) : 
    Handler(resourceKey, []()->SharedResource* { return nullptr; }) { }

/*
 * Blocks the thread until it can be locked for reading.
 */
void ThreadResource::ThreadLock::takeReadLock()
{
    getResourceLock().takeReadLock();
}

/*
 * Blocks the thread until it can be locked for writing.
 */
void ThreadResource::ThreadLock::takeWriteLock()
{
    getResourceLock().takeWriteLock();
}

/*
 * Releases a lock held on this thread. This must be called once for each call 
 * to takeReadLock or takeWriteLock.
 */
void ThreadResource::ThreadLock::releaseLock()
{
    getResourceLock().releaseLock();
}

/*
 * Initializes the thread, runs the action loop, then runs cleanup routines 
 * before the thread exits.
 */
void ThreadResource::run()
{
    ThreadLock threadLock(getResourceKey());
    threadLock.takeWriteLock();
    init();
    threadLock.releaseLock();

    while(!threadShouldExit())
    {
        runLoop(threadLock);
    }
    
    threadLock.takeWriteLock();
    cleanup();
    threadLock.releaseLock();
}
