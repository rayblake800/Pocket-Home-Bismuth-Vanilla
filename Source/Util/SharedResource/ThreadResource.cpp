#include "ThreadResource.h"

ThreadResource::ThreadResource(const juce::Identifier& resourceKey) :
    SharedResource(resourceKey), Thread(resourceKey.toString()) { }

void ThreadResource::takeReadLock()
{
    if(isThreadRunning() && threadLock != nullptr)
    {
        threadLock->takeReadLock();
    }
}

void ThreadResource::takeWriteLock()
{
    if(isThreadRunning() && threadLock != nullptr)
    {
        threadLock->takeWriteLock();
    }
}

void ThreadResource::releaseLock()
{
    if(isThreadRunning() && threadLock != nullptr)
    {
        threadLock->releaseLock();
    }
}

void ThreadResource::run()
{
}
 
ThreadResource::ThreadLock::ThreadLock()
{
}
void ThreadResource::ThreadLock::takeReadLock()
{
}
void ThreadResource::ThreadLock::takeWriteLock()
{
}
void ThreadResource::ThreadLock::releaseLock()
{
}
