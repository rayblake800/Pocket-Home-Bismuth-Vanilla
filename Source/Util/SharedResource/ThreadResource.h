#pragma once
#include "SharedResource.h"
#include "JuceHeader.h"

/**
 * @file  ThreadResource.h
 * 
 * @brief  An abstract basis for SharedResource Thread objects. 
 */
class ThreadResource : public SharedResource, public juce::Thread
{
public:
    /**
     * @brief  Creates a new ThreadResource.
     *
     * @param resourceKey  The thread's unique SharedResource object key.
     *                     This will also be used to set the thread name.
     */
    ThreadResource(const juce::Identifier& resourceKey);

    virtual ~ThreadResource() { }

protected:
    
    /**
     * @brief  Prevents the thread object from being destroyed while the
     *         thread is running, and allows the thread to lock itself. 
     */
    class ThreadLock : public SharedResource::Handler
    {
    private:
        /* The ThreadLock may only be created by ThreadResource. */
        friend class ThreadResource;
        ThreadLock();

    public:
        virtual ~ThreadLock() { }
            
        /**
         * @brief  Blocks the thread until it can be locked for reading.
         *
         * Once locked for reading, the thread will block any attempts to
         * acquire a write lock until all readers have released their locks.
         * No changes may be made to the ThreadResource's data members while
         * this is locked for reading.
         */
        void takeReadLock();

        /**
         * @brief  Blocks the thread until it can be locked for writing.
         *
         * Once locked for writing, no other thread will be able to access the
         * ThreadResource's data members until the write lock has been released.
         */
        void takeWriteLock();

        /**
         * @brief  Releases a lock held on this thread. This must be called once
         *         for each call to takeReadLock or takeWriteLock.
         */
        void releaseLock();
    };

private:
    /**
     * @brief  Runs once each time the thread starts running. 
     *
     * The thread will be locked for writing while this method runs.  Override
     * this to provide custom initialization for a ThreadResource subclass.
     */
    virtual void init() { }

    virtual void runLoop(ThreadLock* lock) = 0;

    virtual void cleanup() { }

    virtual void run() final override;
    

    juce::ScopedPointer<ThreadLock> threadLock;
};
