#include "ResourceManager.h"

/**
 * While debugging, prints an error if the resource is destroyed while the 
 * reference list is not empty.
 */
ResourceManager::SharedResource::~SharedResource()
{
    if (!referenceList.isEmpty())
    {
        DBG("~SharedResource Error: shared resource is being destroyed while "
                << referenceList.size() << " references still exist!");
    }
}

ResourceManager::ResourceManager(
        juce::ScopedPointer<SharedResource>& classResource,
        juce::ReadWriteLock& resourceLock,
        std::function<SharedResource*() > createResource) :
classResource(classResource),
resourceLock(resourceLock)
{
    using namespace juce;
    const ScopedWriteLock initLock(resourceLock);
    if (classResource == nullptr)
    {
        classResource = createResource();
        jassert(classResource != nullptr);
    }
    classResource->referenceList.add(this);
}

/**
 * This will remove itself from the sharedResource's reference list, and
 * destroy the resource if this was the last reference in the list.
 */
ResourceManager::~ResourceManager()
{
    using namespace juce;
    const ScopedWriteLock initLock(resourceLock);
    if (classResource == nullptr)
    {
        DBG("ResourceManager Error: shared resource is missing!");
    }
    classResource->referenceList.removeAllInstancesOf(this);
    if (classResource->referenceList.isEmpty())
    {
        classResource = nullptr;
    }
}
