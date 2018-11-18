#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource/Implementation/Holder.h"

/* Holds the unique SharedResource::Holder instance */
static std::unique_ptr<SharedResource::Holder> holderInstance;

/* Prevents concurrent access to the containers holding resource data.*/
juce::CriticalSection SharedResource::Holder::holderLock;

SharedResource::Holder::Holder()
{
    DBG("SharedResource::Holder::" << __func__ 
            << ": Creating SharedResource instance Holder ");
}

/*
 * Clears all resource locks.
 */
SharedResource::Holder::~Holder()
{
    const juce::ScopedLock cleanupLock(holderLock);
    DBG("SharedResource::Holder::" << __func__ 
            << ": Destroying SharedResource instance Holder ");
#ifdef JUCE_DEBUG
    // Test that all resources were properly destroyed.  Claim all resource
    // locks for writing in case a resource is still being deleted.
    for(int i = 0; i < resourceList.size(); i++)
    {
        const juce::ScopedWriteLock resourceLock(*resourceLocks[i]);
        if(resourceList[i] != nullptr)
        {
            juce::String resourceName = "[Missing ID!]";
            for(auto iter : resourceIndices)
            {
                if(iter.second == i)
                {
                    resourceName = iter.first.toString();
                    break;
                }
            }
            DBG("SharedResource::Holder::" << __func__ << ": Resource "
                    << resourceName << " was not destroyed!");
            jassertfalse;
        }
    }
    jassert(resourceIndices.size() == resourceList.size()
            && resourceList.size() == resourceLocks.size());
#endif
    resourceIndices.clear();
    resourceList.clear();
    resourceLocks.clear();
}

/*
 * Gets the program's sole SharedResource::Holder instance.
 */
SharedResource::Holder* SharedResource::Holder::getHolderInstance()
{
    const juce::ScopedLock initLock(holderLock);
    if(holderInstance == nullptr)
    {
        holderInstance.reset(new Holder());
    }
    return holderInstance.get();
}

/*
 * Finds and gets an Instance pointer using its resource key.
 */
SharedResource::Instance* SharedResource::Holder::getResource
(const juce::Identifier& resourceKey)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    return holderInstance->resourceList.getUnchecked(resourceIndex);
}

/*
 * Saves the address of a new resource instance. This should only be called when
 * the existing resource instance at the given ID is null, or to set the pointer
 * to null while destroying the resource.
 */
void SharedResource::Holder::setResource(const juce::Identifier& resourceKey,
        Instance* resource)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    Holder* const holder = getHolderInstance();

    // Make sure either a null resource is becoming non-null, or a
    // non-null resource is becoming null.
    jassert((holder->resourceList.getUnchecked(resourceIndex) == nullptr)
            != (resource == nullptr));

    holder->resourceList.set(resourceIndex, resource);

}

/*
 * Deletes the holder instance if it is holding no valid Instance pointers.  
 * This should be called after deleting a resource Instance.
 */
void SharedResource::Holder::clearIfEmpty()
{
    if(holderInstance != nullptr)
    {
        for(Instance* resourceInstance : holderInstance->resourceList)
        {
            if(resourceInstance != nullptr)
            {
                return;
            }
        }
        holderInstance.reset(nullptr);
    }
}

/*
 * Gets the lock used to control access to a resource instance.
 */
const juce::ReadWriteLock& 
SharedResource::Holder::getResourceLock(const juce::Identifier& resourceKey)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    return *getHolderInstance()->resourceLocks[resourceIndex];
}

/*
 * Gets the index where a resource and its lock are stored in the resourceList 
 * and resourceLocks. If necessary, this will initialize the resource container,
 * creating a new lock and assigning an index.
 */
int SharedResource::Holder::getResourceIndex
(const juce::Identifier& resourceKey)
{
    if(getHolderInstance()->resourceIndices.count(resourceKey) == 0)
    {
        initResource(resourceKey);
    }
    return getHolderInstance()->resourceIndices.at(resourceKey);
}

/*
 * Initializes a resource container, assigning it an index, creating its lock, 
 * and setting the initial resource instance address as nullptr.
 */
void SharedResource::Holder::initResource(const juce::Identifier& resourceKey)
{
    Holder* const holder = getHolderInstance();
    jassert(holder->resourceList.size() == holder->resourceLocks.size());
    holder->resourceIndices[resourceKey] = holder->resourceList.size();
    holder->resourceList.add(nullptr);
    holder->resourceLocks.add(new juce::ReadWriteLock());
}

