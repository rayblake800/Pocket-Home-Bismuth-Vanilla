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
        const ScopedUnlock lockUnlock(metaLock);
        lock.enterRead();
    }
}

void ThreadLock::takeThreadLock()
{
    using namespace juce;
    const ScopedLock lockLock(metaLock);
    Thread::ThreadID threadId = Thread::getCurrentThreadId();
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
    if(threadLockCount > 0)
    {
        if(lockingThread != Thread::getCurrentThreadId())
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
        lock.exitRead();
    }
}




