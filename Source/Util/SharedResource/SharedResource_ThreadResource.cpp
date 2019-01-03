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
    stopThreadResource();
}

/*
 * Starts the ThreadResource's thread if it isn't already running.
 */
void SharedResource::ThreadResource::startThreadResource()
{
    startThread();
}

/*
 * Performs all necessary steps to stop the thread, and waits for the thread to 
 * exit.
 */
void SharedResource::ThreadResource::stopThreadResource()
{
    jassert(Thread::getCurrentThreadId() != getThreadId());
    if(isThreadRunning())
    {
        signalThreadShouldExit();
        notify();
        DBG("SharedResource::ThreadResource::" << __func__ << ": Thread \""
                << getResourceKey().toString() 
                << "\" is stopping.");
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
    while(!threadShouldExit())
    {
        ThreadLock* threadLock = new ThreadLock(getResourceKey());
        threadLock->enterWrite();
        init();
        threadLock->exitWrite();

        while(!threadShouldExit() && !threadShouldWait())
        {
            runLoop(*threadLock);
        }
        
        threadLock->enterWrite();
        cleanup();
        threadLock->exitWrite();

        // Delete the lock outside of the thread, just in case deleting the lock
        // also deletes the thread.
        std::function<void()> deleteLock = buildAsyncFunction(LockType::write,
            [this, threadLock]()
            {
                if(threadShouldExit())
                {
                    notify();
                    waitForThreadToExit(-1);
                }
                delete threadLock;
            });
        
        // Use the message thread if possible, create a new thread instead if the
        // message thread is stopping.
        juce::MessageManager* messageManager 
            = juce::MessageManager::getInstanceWithoutCreating();
        if(messageManager == nullptr || messageManager->hasStopMessageBeenSent())
        {
            juce::Thread::launch(deleteLock);
        }
        else
        {
            juce::MessageManager::callAsync(deleteLock);
        }

        if(threadShouldWait() && !threadShouldExit())
        {
            wait(-1);
        }
    }
}
