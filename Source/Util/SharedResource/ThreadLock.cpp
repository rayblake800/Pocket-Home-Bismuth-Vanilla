#include "ThreadLock.h"

void ThreadLock::takeReadLock()
{
    using namespace juce;
    const ScopedLock lockLock(metaLock);
    Thread::ThreadID threadId = Thread::getCurrentThreadId();
    if(threadLockCount > 0 && lockingThread == threadId)
    {
        threadLockCount++;
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

void ThreadLock::takeThreadLock()
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

    if(threadLockCount == 0 || lockingThread != threadId)
    {
        const ScopedUnlock lockUnlock(metaLock);
        lock.enterWrite();
    }
    lockingThread = threadId;
    threadLockCount++;
}

void ThreadLock::releaseLock()
{
    using namespace juce;
    const ScopedLock lockLock(metaLock);
    Thread::ThreadID threadId = Thread::getCurrentThreadId();
    if(threadLockCount > 0)
    {
        if(lockingThread != threadId)
        {
            DBG("ThreadLock::" << __func__ << ": Thread "
                    << String((unsigned long) Thread::getCurrentThreadId())
                    << " tried to release a thread lock held by thread "
                    << String((unsigned long) lockingThread));
            jassertfalse;
            return;
        }
        threadLockCount--;
        if(threadLockCount == 0)
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




