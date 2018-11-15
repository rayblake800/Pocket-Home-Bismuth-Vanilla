#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource/Holder.h"

/* Holds the unique SharedResource::Holder instance */
static std::unique_ptr<SharedResource::Holder> holderInstance;

/* Prevents concurrent access to the containers holding resource data.*/
juce::CriticalSection SharedResource::Holder::holderLock;

/*
 * Clears all resource locks.
 */
SharedResource::Holder::~Holder()
{
    const juce::ScopedLock cleanupLock(holderLock);
#ifdef JUCE_DEBUG
    // Test that all resources were properly destroyed.
    for(int i = 0; i < resourceList.size(); i++)
    {
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
}

/*
 * Finds and gets an InstanceData pointer using its resource key.
 */
SharedResource::InstanceData* SharedResource::Holder::getResource
(const juce::Identifier& resourceKey)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    return resourceList.getUnchecked(resourceIndex);
}

/*
 * Saves the address of a new resource instance. This should only be called when
 * the existing resource instance at the given ID is null, or to set the pointer
 * to null while destroying the resource.
 */
void SharedResource::Holder::setResource(const juce::Identifier& resourceKey,
        InstanceData* resource)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    jassert((resourceList.getUnchecked(resourceIndex) == nullptr)
            != (resource == nullptr));
    resourceList.set(resourceIndex, resource);
}

/*
 * Gets the lock used to control access to a resource instance.
 */
const juce::ReadWriteLock& 
SharedResource::Holder::getResourceLock(const juce::Identifier& resourceKey)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    return *resourceLocks[resourceIndex];
}

/*
 * Gets the index where a resource and its lock are stored in the resourceList 
 * and resourceLocks. If necessary, this will initialize the resource container,
 * creating a new lock and assigning an index.
 */
int SharedResource::Holder::getResourceIndex
(const juce::Identifier& resourceKey)
{
    if(resourceIndices.count(resourceKey) == 0)
    {
        initResource(resourceKey);
    }
    return resourceIndices.at(resourceKey);
}

/*
 * Initializes a resource container, assigning it an index, creating its lock, 
 * and setting the initial resource instance address as nullptr.
 */
void SharedResource::Holder::initResource(const juce::Identifier& resourceKey)
{
    jassert(resourceList.size() == resourceLocks.size());
    resourceIndices[resourceKey] = resourceList.size();
    resourceList.add(nullptr);
    resourceLocks.add(new juce::ReadWriteLock());
}

