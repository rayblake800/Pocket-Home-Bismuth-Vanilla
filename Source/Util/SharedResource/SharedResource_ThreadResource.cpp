#include "SharedResource_ThreadResource.h"

/* Number of milliseconds to wait before forcibly terminating the thread: */
static const constexpr int timeoutMilliseconds = 5000;

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
    stopThreadResource();
    // Force the thread to stop if it refuses on its own for some reason.
    stopThread(timeoutMilliseconds);
}

/*
 * Starts the ThreadResource's thread if it isn't already running.
 */
void SharedResource::ThreadResource::startThreadResource()
{
    std::unique_lock<std::mutex> startLock(startMutex);
    if(!isThreadRunning())
    {
        startThread();
        ThreadLock tempUnlocker(getResourceKey());
        tempUnlocker.exitWrite();
        startCondition.wait(startLock);
        tempUnlocker.enterWrite();
    }
}

/*
 * Performs all necessary steps to stop the thread, and waits for the thread to 
 * exit.
 */
void SharedResource::ThreadResource::stopThreadResource()
{
    // Don't try to stop the thread while running on the thread:
    jassert(Thread::getCurrentThreadId() != getThreadId());
    if(isThreadRunning())
    {
        signalThreadShouldExit();
        notify();
        DBG("SharedResource::ThreadResource::" << __func__ << ": Thread \""
                << getResourceKey().toString() 
                << "\" is stopping.");
    }
}

/*
 * Stops the thread resource, waiting until the thread completely stops running 
 * and deletes its ThreadLock.
 */
void SharedResource::ThreadResource::stopThreadAndWait()
{
    std::unique_lock<std::mutex> stopLock(stopMutex);
    if(isThreadRunning() || threadLock != nullptr)
    {
        ThreadLock tempUnlocker(getResourceKey());
        tempUnlocker.exitWrite();
        stopThreadResource();
        stopCondition.wait(stopLock);
        tempUnlocker.enterWrite();
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
    // Make sure a previous ThreadLock instance doesn't still exist:
    {
        std::unique_lock<std::mutex> stopLock(stopMutex);
        if(threadLock != nullptr)
        {
            stopCondition.wait(stopLock);
        }
    }
    // Notify any callers waiting for the thread to start:
    {
        std::unique_lock<std::mutex> startLock(startMutex);
        startCondition.notify_all();
    }
    threadLock.reset(new ThreadLock(getResourceKey()));

    while(!threadShouldExit())
    {
        threadLock->enterWrite();
        DBG("SharedResource::ThreadResource::" << __func__ 
                << ": Initializing thread \"" << getResourceKey().toString() 
                << "\".");
        init();
        threadLock->exitWrite();

        DBG("SharedResource::ThreadResource::" << __func__ 
                << ": Thread \"" << getResourceKey().toString() 
                << "\" entering main action loop.");
        while(!threadShouldExit() && !threadShouldWait())
        {
            runLoop(*threadLock);
        }
        
        threadLock->enterWrite();
        DBG("SharedResource::ThreadResource::" << __func__ 
                << ": Running cleanup for thread \"" 
                << getResourceKey().toString() << "\".");
        cleanup();
        threadLock->exitWrite();

        if(threadShouldWait() && !threadShouldExit())
        {
            DBG("SharedResource::ThreadResource::" << __func__ << ": Thread \""
                    << getResourceKey().toString() 
                    << "\" is waiting.");
            wait(-1);
        }

        DBG("SharedResource::ThreadResource::" << __func__ << ": Thread \""
                << getResourceKey().toString() 
                << "\" finished running.");
    }

    // Delete the lock outside of the thread, just in case deleting the lock
    // also deletes the thread.
    std::function<void()> lockDelete = buildAsyncFunction(LockType::write,
    [this]()
    {
        std::unique_lock<std::mutex> stopLock(stopMutex);
        DBG("SharedResource::ThreadResource: Asynchronously deleting "
                << "ThreadLock after thread exits.");
        if(threadShouldExit())
        {
            notify();
            waitForThreadToExit(-1);
        }
        threadLock.reset(nullptr);
        stopCondition.notify_all();
    });

    // The MessageManager thread shouldn't be used, as it might be stopping or 
    // waiting for the lock to be deleted.
    juce::Thread::launch(lockDelete);
}
