#pragma once
#include "SharedResource.h"
#include "JuceHeader.h"

/**
 * @file  ThreadResource.h
 * 
 * @brief  An abstract basis for SharedResource Thread objects. 
 *
 * Each ThreadResource subclass manages a single child thread using the
 * SharedResource control pattern. ThreadResource threads may be accessed
 * through any of their Handler objects, and will only be destroyed when all of
 * their handlers are destroyed. 
 *
 * ThreadResource objects guarantee that they will not be deleted while their
 * thread still runs.  While running within their own thread, ThreadResource
 * objects are able to access their own resource locks to prevent handlers 
 * from modifying their data.
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

    /**
     * @brief  Ensures the thread stops running before it is destroyed.
     */
    virtual ~ThreadResource();

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

        /**
         * @brief  Creates a ThreadLock tied to a single thread resource.
         *
         * @param resourceKey  The thread's unique SharedResource object key.
         */
        ThreadLock(const juce::Identifier& resourceKey);

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
     * this to define custom initialization routines for a ThreadResource 
     * subclass.
     */
    virtual void init() { }

    /**
     * @brief  The main thread action loop. When the thread is running, this
     *         will continually run until something stops the thread.
     *
     * @param lock  Grants access to the thread's SharedResource lock within the
     *              loop.  This should not be saved or shared outside the
     *              thread.
     */
    virtual void runLoop(ThreadLock& lock) = 0;

    /**
     * @brief  Runs once each time the thread stops running.
     *
     * The thread will be locked for writing while this method runs.  Override
     * this to define custom cleanup routines for a ThreadResource subclass.
     */
    virtual void cleanup() { }

    /**
     * @brief  Initializes the thread, runs the action loop, then runs cleanup
     *         routines before the thread exits.
     */
    virtual void run() final override;
};
