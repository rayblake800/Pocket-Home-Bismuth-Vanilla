#include "ScopedThreadReadLock.h"

ScopedThreadReadLock::ScopedThreadReadLock(ThreadLock& threadLock) :
threadLock(threadLock)
{
    threadLock.takeReadLock();
}

ScopedThreadReadLock::~ScopedThreadReadLock()
{
    threadLock.releaseLock();
}
