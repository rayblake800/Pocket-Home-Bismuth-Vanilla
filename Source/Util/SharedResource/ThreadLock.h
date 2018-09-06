#pragma once
#include "JuceHeader.h"

/**
 * @file ThreadLock.h
 *
 * @brief A variant on the juce::ReadWriteLock that can be locked for writing 
 *        any number of times, as long as the same thread creates the write
 *        lock.
 *
 * Threads can request to claim the lock for reading or writing. Threads can 
 * hold multiple claims to the lock, and multiple threads may hold reading
 * claims on the lock simultaneously.  Threads must release the lock 
 * is made to take the lock for reading, the thread will yield until no other 
 * thread has the lock held for writing, and no callers are waiting to hold the
 * thread for wri.  When a request is made to take a thread-lock, the thread
 * will yield until no read-locks are held, no thread-locks are held by other
 * threads, and no other thread has been waiting longer to take a thread-lock.
 *
 * This allows the lock to be safely acquired multiple times by the same thread
 * for writing, so that any function that requires thread safety does not also
 * need to ensure that no function calling it has claimed its lock, and no
 * function it calls will need to claim its lock.
 *
 */
class ThreadLock
{
public:
    ThreadLock() { }

    virtual ~ThreadLock() { }

    /**
     * Requests to take a read-lock, blocking the calling thread until it 
     * is safe for it to read whatever resource is protected by the lock.  This 
     * will block if any other thread holds a thread-lock, or if this thread 
     * does not hold the thread-lock and another thread is waiting to hold the 
     * thread-lock.
     */
    void takeReadLock();

    /**
     * Requests to take a thread-lock, blocking the calling thread until it is
     * safe for it to write to whatever resource is protected by this lock. This
     * will block if any thread holds a read-lock, or if another thread holds
     * the thread-lock.
     */
    void takeThreadLock();

    /**
     * Releases a lock held by this thread.  This must be called exactly once
     * for each call to takeReadLock or takeThreadLock, when the caller is ready
     * to stop accessing the locked resource.
     */
    void releaseLock();
private:
    // Handles lock/unlock mechanics
    juce::ReadWriteLock lock;
    
    // Prevents concurrent access to thread-lock data members below
    juce::CriticalSection metaLock;
    // Thread id of the last thread to acquire a thread-lock
    juce::Thread::ThreadID lockingThread;
    // The number of thread locks currently held
    int threadLockCount = 0;
    //map of all threads holding read locks
    std::map<juce::Thread::ThreadID, int> readLockMap;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThreadLock)
};
