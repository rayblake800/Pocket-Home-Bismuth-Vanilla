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
void ThreadResource::ThreadLock::enterRead() const
{
    getResourceLock().enterRead();
}

/*
 * Blocks the thread until it can be locked for writing.
 */
void ThreadResource::ThreadLock::enterWrite() const
{
    getResourceLock().enterWrite();
}

/*
 * Releases a read lock held by this thread. This must be called once for each 
 * call to takeReadLock.
 */
void ThreadResource::ThreadLock::exitRead() const
{
    getResourceLock().exitRead();
}

/*
 * Releases a write lock held by this thread. This must be called once for each 
 * call to takeWriteLock.
 */
void ThreadResource::ThreadLock::exitWrite() const
{
    getResourceLock().exitWrite();
}

/*
 * Initializes the thread, runs the action loop, then runs cleanup routines 
 * before the thread exits.
 */
void ThreadResource::run()
{
    ThreadLock threadLock(getResourceKey());
    threadLock.enterWrite();
    init();
    threadLock.exitWrite();

    while(!threadShouldExit())
    {
        runLoop(threadLock);
    }
    
    threadLock.enterWrite();
    cleanup();
    threadLock.exitWrite();
}
