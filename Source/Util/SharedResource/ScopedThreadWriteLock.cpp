#include "ScopedThreadWriteLock.h"

ScopedThreadWriteLock::ScopedThreadWriteLock(ThreadLock& threadLock) :
threadLock(threadLock)
{
    threadLock.takeThreadLock();
}

ScopedThreadWriteLock::~ScopedThreadWriteLock()
{
    threadLock.releaseLock();
}
