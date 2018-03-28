#include "RAIISingleton.h"

/**
 * Throws an error if the resource is destroyed while the reference
 * list is not empty.
 */
RAIISingleton::SharedResource::~SharedResource()
{
    if (!referenceList.isEmpty())
    {
        DBG("~SharedResource Error: shared resource is being destroyed while "
                << referenceList.size() << " references still exist!");
    }
}

RAIISingleton::RAIISingleton(
        ScopedPointer<SharedResource>& classResource,
        CriticalSection& resourceLock,
        std::function<SharedResource*() > createResource) :
classResource(classResource),
resourceLock(resourceLock)
{
    const ScopedLock initLock(resourceLock);
    if (classResource == nullptr)
    {
        classResource = createResource();
    }
    classResource->referenceList.add(this);
}

/**
 * This will remove itself from the sharedResource's reference list, and
 * destroy the resource if this was the last reference in the list.
 */
RAIISingleton::~RAIISingleton()
{
    const ScopedLock initLock(resourceLock);
    if (classResource == nullptr)
    {
        DBG("RAIISingleton Error: shared resource is missing!");
    }
    classResource->referenceList.removeAllInstancesOf(this);
    if (classResource->referenceList.isEmpty())
    {
        classResource = nullptr;
    }
}