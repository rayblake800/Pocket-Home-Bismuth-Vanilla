#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource/Implementation/Holder.h"
#include "SharedResource/Implementation/LockedInstancePtr.h"

/*
 * Initializes the resource pointer, locking the resource.
 */
SharedResource::LockedInstancePtr::LockedInstancePtr
(const juce::Identifier& resourceKey, const LockType lockType) :
resourceKey(resourceKey),
lockType(lockType)
{
    const juce::ReadWriteLock& resourceLock 
        = Holder::getHolderInstance()->getResourceLock(resourceKey);
    if(lockType == LockType::read)
    {
        resourceLock.enterRead();
    }
    else
    {
        resourceLock.enterWrite();
    }
}

/*
 * Unlocks the resource when the LockedInstancePtr is destroyed.
 */
SharedResource::LockedInstancePtr::~LockedInstancePtr()
{
    const juce::ReadWriteLock& resourceLock 
        = Holder::getHolderInstance()->getResourceLock(resourceKey);
    if(lockType == LockType::read)
    {
        resourceLock.exitRead();
    }
    else
    {
        resourceLock.exitWrite();
    }
}

/*
 * Accesses the resource Instance's methods or data.
 */
SharedResource::Instance* SharedResource::LockedInstancePtr::operator->() const
{
    return Holder::getHolderInstance()->getResource(resourceKey);
}
