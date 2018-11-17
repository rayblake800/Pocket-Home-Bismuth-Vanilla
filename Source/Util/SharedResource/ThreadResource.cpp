#include "ThreadResource.h"

/* Number of milliseconds to wait before forcibly terminating the thread: */
static const constexpr int timeoutMilliseconds = 1000;

/*
 * Creates a new ThreadResource.
 */
SharedResource::ThreadResource::ThreadResource
(const juce::Identifier& resourceKey) :
    Resource(resourceKey), Thread(resourceKey.toString()) { }

/*
 * Ensures the thread stops running before it is destroyed.
 */
SharedResource::ThreadResource::~ThreadResource()
{
    if(isThreadRunning())
    {
        stopThread(timeoutMilliseconds);
    }
}

/*
 * Creates a ThreadLock tied to a single thread resource.
 */
SharedResource::ThreadResource::ThreadLock::ThreadLock
(const juce::Identifier& resourceKey) : 
    Handler(resourceKey, []()->ThreadResource* { return nullptr; }) { }

/*
 * Blocks the thread until it can be locked for reading.
 */
void SharedResource::ThreadResource::ThreadLock::enterRead() const
{
    getResourceLock().enterRead();
}

/*
 * Blocks the thread until it can be locked for writing.
 */
void SharedResource::ThreadResource::ThreadLock::enterWrite() const
{
    getResourceLock().enterWrite();
}

/*
 * Releases a read lock held by this thread. This must be called once for each 
 * call to takeReadLock.
 */
void SharedResource::ThreadResource::ThreadLock::exitRead() const
{
    getResourceLock().exitRead();
}

/*
 * Releases a write lock held by this thread. This must be called once for each 
 * call to takeWriteLock.
 */
void SharedResource::ThreadResource::ThreadLock::exitWrite() const
{
    getResourceLock().exitWrite();
}

/*
 * Initializes the thread, runs the action loop, then runs cleanup routines 
 * before the thread exits.
 */
void SharedResource::ThreadResource::run()
{

    ThreadLock* newLock = new ThreadLock(getResourceKey());
    // Lock the message manager thread before setting the new lock to avoid 
    // assigning it while the message manager is clearing the old lock.
    {
        const juce::MessageManagerLock mmLock;
        threadLock.reset(newLock);
    }
    threadLock->enterWrite();
    init();
    threadLock->exitWrite();

    while(!threadShouldExit())
    {
        runLoop(*threadLock);
    }
    
    threadLock->enterWrite();
    cleanup();
    threadLock->exitWrite();

    std::function<void()> deleteLock = buildAsyncFunction(LockType::write,
        [this, newLock]()
        {
            if(newLock == threadLock.get())
            {
                threadLock.reset(nullptr);
            }
        });
}
