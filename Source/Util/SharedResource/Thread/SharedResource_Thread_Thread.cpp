#include "SharedResource_Thread_Thread.h"

/* Number of milliseconds to wait before forcibly terminating the thread: */
static const constexpr int timeoutMilliseconds = 5000;

namespace ThreadResource = SharedResource::Thread;

/*
 * Creates the Thread object without starting the thread.
 */
ThreadResource::Thread::Thread(const juce::String name) :
        juce::Thread(name) { }

/*
 * Ensures the thread stops running before it is destroyed.
 */
ThreadResource::Thread::~Thread()
{
    stopResourceThread();
    // Force the thread to stop if it refuses on its own for some reason.
    stopThread(timeoutMilliseconds);
}

/*
 * Starts the Thread's thread if it isn't already running.
 */
void ThreadResource::Thread::startResourceThread()
{
    std::unique_lock<std::mutex> startLock(startMutex);
    if(!isThreadRunning())
    {
        // Make sure a previous Lock instance doesn't still exist:
        {
            std::unique_lock<std::mutex> stopLock(stopMutex);
            if(threadLock != nullptr)
            {
                DBG("SharedResource::Thread::Thread::" << __func__ 
                        << ": Waiting for last thread instance to finish "
                        << "cleanup.");
                stopCondition.wait(stopLock);
            }
        }
        // Create the thread lock before starting the thread:
        DBG(getThreadName() << ": Creating thread lock:");
        threadLock.reset(new Lock(getThreadResourceKey()));
        startThread();
        startCondition.wait(startLock);
    }
}

/*
 * Performs all necessary steps to stop the thread, and waits for the thread to 
 * exit.
 */
void ThreadResource::Thread::stopResourceThread()
{
    // Don't try to stop the thread while running on the thread:
    jassert(Thread::getCurrentThreadId() != getThreadId());
    if(isThreadRunning())
    {
        signalThreadShouldExit();
        notify();
        DBG("SharedResource::Thread::Thread::" << __func__ << ": Thread \""
                << getThreadName() << "\" is stopping.");
    }
}

/*
 * Stops the thread resource, waiting until the thread completely stops running 
 * and deletes its ThreadLock.
 */
void ThreadResource::Thread::stopThreadAndWait()
{
    std::unique_lock<std::mutex> stopLock(stopMutex);
    if(isThreadRunning() || threadLock != nullptr)
    {
        stopResourceThread();
        stopCondition.wait(stopLock);
    }
}

/*
 * Initializes the thread, runs the action loop, then runs cleanup routines 
 * before the thread exits.
 */
void ThreadResource::Thread::run()
{
    jassert(threadLock != nullptr);
    // Notify any callers waiting for the thread to start:
    {
        std::unique_lock<std::mutex> startLock(startMutex);
        startCondition.notify_all();
    }

    DBG("SharedResource::Thread::Thread::" << __func__ 
            << ": Initializing thread \"" << getThreadName()
            << "\".");
    init(*threadLock);

    DBG("SharedResource::Thread::Thread::" << __func__ 
            << ": Thread \"" << getThreadName()
            << "\" running main action loop.");
    while(!threadShouldExit())
    {
        runLoop(*threadLock);

        // If the threadLock is the thread's only reference, start shutting it
        // down:
        if(getThreadReferenceCount() == 1)
        {
            signalThreadShouldExit();
        }

        if(threadShouldWait() && !threadShouldExit())
        {
            DBG("SharedResource::Thread::Thread::" << __func__ << ": Thread \""
                    << getThreadName() << "\" is waiting.");
            wait(-1);
            DBG("SharedResource::Thread::Thread::" << __func__ << ": Thread \""
                    << getThreadName() << "\" is resuming.");
        }
    }
        
    DBG("SharedResource::Thread::Thread::" << __func__ 
            << ": Running cleanup for thread \"" << getThreadName() << "\".");
    cleanup(*threadLock);
        
    DBG("SharedResource::Thread::Thread::" << __func__ << ": Thread \""
                << getThreadName() << "\" finished running.");

    // Delete the lock outside of the thread, just in case deleting the lock
    // also deletes the thread.
    std::function<void()> lockDelete = [this]()
    {
        std::unique_lock<std::mutex> stopLock(stopMutex);
        DBG("SharedResource::Thread::Thread: Asynchronously deleting "
                << "ThreadLock after thread exits.");
        if(threadShouldExit())
        {
            notify();
            waitForThreadToExit(-1);
        }
        threadLock.reset(nullptr);
        stopCondition.notify_all();
    };

    // The MessageManager thread shouldn't be used, as it might be stopping or 
    // waiting for the lock to be deleted.
    juce::Thread::launch(lockDelete);
}

/*
 * Ensures the thread is stopped when the application starts to shut down.
 */
void ThreadResource::Thread::onShutdown()
{
    stopThreadAndWait();
}
