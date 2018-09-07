#include "ThreadLock.h"

/*
 * Requests to take a read-lock, blocking the calling thread until it 
 * is safe for it to read whatever resource is protected by the lock.  This 
 * will block if any other thread holds a write-lock, or if this thread 
 * does not hold the write-lock and another thread is waiting to hold the 
 * write-lock.
 */
void ThreadLock::takeReadLock()
{
    using namespace juce;
    const ScopedLock lockLock(metaLock);
    Thread::ThreadID threadId = Thread::getCurrentThreadId();
    if(writeLockCount > 0 && writeLockThread == threadId)
    {
        writeLockCount++;
    }
    else
    {
        if(readLockMap.count(threadId) == 0)
        {
            readLockMap[threadId] = 1;
        }
        else
        {
            readLockMap[threadId]++;
        }
        const ScopedUnlock lockUnlock(metaLock);
        lock.enterRead();
    }
}

/*
 * Requests to take a write-lock, blocking the calling thread until it is
 * safe for it to write to whatever resource is protected by this lock. This
 * will block if any thread holds a read-lock, or if another thread holds
 * the write-lock.
 */
void ThreadLock::takeWriteLock()
{
    using namespace juce;
    const ScopedLock lockLock(metaLock);
    Thread::ThreadID threadId = Thread::getCurrentThreadId();
    while(readLockMap.count(threadId) > 0 && readLockMap[threadId] > 0)
    {
        DBG("ThreadLock::" << __func__ << ": Thread "
                << String((unsigned long) threadId) << " trying to take "
                << "a write lock while it is already read-locked "
                << readLockMap[threadId] << " times.");
        jassertfalse;
        const ScopedUnlock lockUnlock(metaLock);
        releaseLock();
    }

    if(writeLockCount == 0 || writeLockThread != threadId)
    {
        const ScopedUnlock lockUnlock(metaLock);
        lock.enterWrite();
    }
    writeLockThread = threadId;
    writeLockCount++;
}

/*
 * Releases a lock held by this thread.  This must be called exactly once
 * for each call to takeReadLock or takeWriteLock, when the caller is ready
 * to stop accessing the locked resource.
 */
void ThreadLock::releaseLock()
{
    using namespace juce;
    const ScopedLock lockLock(metaLock);
    Thread::ThreadID threadId = Thread::getCurrentThreadId();
    if(writeLockCount > 0)
    {
        if(writeLockThread != threadId)
        {
            DBG("ThreadLock::" << __func__ << ": Thread "
                    << String((unsigned long) Thread::getCurrentThreadId())
                    << " tried to release a thread lock held by thread "
                    << String((unsigned long) writeLockThread));
            jassertfalse;
            return;
        }
        writeLockCount--;
        if(writeLockCount == 0)
        {
            lock.exitWrite();
        }
    }
    else
    {
        if(readLockMap.count(threadId) == 0)
        {
            readLockMap[threadId] = 0;
        }
        if(readLockMap[threadId] < 1)
        {
            DBG("ThreadLock::" << __func__ << ": Thread "
                    << String((unsigned long) Thread::getCurrentThreadId())
                    << " tried to unlock, but has no lock.");
            //Print thread lock state:
            jassertfalse;
        }
        else
        {
            readLockMap[threadId]--;
            lock.exitRead();
        }
    }
}




