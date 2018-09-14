#include "ScopedThreadWriteLock.h"

ScopedThreadWriteLock::ScopedThreadWriteLock(ThreadLock& threadLock) :
threadLock(threadLock)
{
    threadLock.takeWriteLock();
}

ScopedThreadWriteLock::~ScopedThreadWriteLock()
{
    threadLock.releaseLock();
}
