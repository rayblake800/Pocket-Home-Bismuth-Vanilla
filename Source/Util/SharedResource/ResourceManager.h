#pragma once
#include "JuceHeader.h"
#include "SharedResource.h"

/**
 * @file ResourceManager.h
 *
 * @brief Provides a more convenient interface for 
 *        SharedResource::ResourceManager objects, interacting with their
 *        specific SharedResource type.
 *
 * @tparam ResourceType  The type of SharedResource class managed by the
 *                       ResourceManager.
 */

template <class ResourceType>
class ResourceManager : private SharedResource::ResourceManager
{
protected:
    /**
     * @param classResource    This must be a reference to a static 
     *                         ScopedPointer that will hold the class 
     *                         SharedResource. This will initialize the 
     *                         resource if necessary, and add the 
     *                         ResourceManager to the SharedResource's 
     *                         reference. 
     *                        
     *                         ResourceManager instances should never directly
     *                         access the classResource ScopedPointer.
     * 
     * @param resourceLock     A reference to a static ReadWriteLock used to 
     *                         control access to the SharedResource.  
     *                         
     *                         ResourceManager instances should never directly
     *                         access the resourceLock.
     * 
     * @param createResource   This function will be used to create the 
     *                         SharedResource if necessary.  This should not
     *                         acquire the resource lock.
     */
    ResourceManager(
            juce::ScopedPointer<SharedResource>& classResource,
            juce::ReadWriteLock& resourceLock,
            std::function<ResourceType*()> createResource) :
    SharedResource::ResourceManager(classResource, resourceLock, createResource)
    { }

public:
    virtual ~ResourceManager() { }

protected:
    /**
     * A custom pointer object holding the class SharedResource.  As long as
     * this object exists, the SharedResource will be locked.
     */
    class LockedResourcePtr
    {
    private:
        //Disallow direct creation of LockedResourcePtr objects.
        friend class ResourceManager;

        /**
         * Create the pointer, locking the SharedResource for reading.
         *
         * @param resourceManager  The ResourceManager creating this pointer.
         *
         * @param lockType         Sets if the resource will be locked for
         *                         writing, blocking all other access, or locked
         *                         for reading, only blocking write attempts.
         */
        LockedResourcePtr
        (ResourceManager& resourceManager, SharedResource::LockType lockType) : 
        resourceManager(resourceManager), lockType(lockType)
        {
            switch(lockType)
            {
                case SharedResource::read:
                    resourceManager.resourceLock.enterRead();
                    break;
                case SharedResource::write:
                    resourceManager.resourceLock.enterWrite();
                    break;
            }
        }

    public:
        /**
         * Destroy the pointer, unlocking the SharedResource.
         */
        virtual ~LockedResourcePtr()
        {
            switch(lockType)
            {
                case SharedResource::read:
                    resourceManager.resourceLock.exitRead();
                    break;
                case SharedResource::write:
                    resourceManager.resourceLock.exitWrite();
                    break;
            }
        }

        /**
         * Gets the SharedResource held by this pointer.
         *
         * @return  The single SharedResource instance.
         */
        ResourceType& operator*()
        {
            return *resourceManager.classResource.get();
        }

        /**
         * Accesses the held SharedResource instance's methods or data.
         *
         * @return  The address of the SharedResource instance.
         */
        ResourceType* operator->()
        {
            return resourceManager.classResource.get();
        }

        /**
         * Checks if another SharedResource pointer points to this object's
         * SharedResource.
         *
         * @rhs  A pointer to a SharedResource.
         *
         * @return  True iff rhs has the same address as this object's
         *          SharedResource.
         */
        bool operator==(const SharedResource* rhs)
        {
            return resourceManager.classResource.get() == rhs;
        }

    private:
        // Used to access the SharedResource and lock.
        ResourceManager& resourceManager;

        // Indicates the lock type.
        const SharedResource::LockType lockType;
    };
    

    /**
     * Gets a pointer to the class resource, locking it for reading.
     * ResourceManagers should use this to access their SharedResource whenever
     * they need to read data from it without changing it.
     *
     * @return  A read-locked pointer to the class SharedResource.
     */
    LockedResourcePtr getReadLockedResource()
    {
        return LockedResourcePtr(*this, false);
    }

    /**
     * Gets a pointer to the class resource, locking it for writing.
     * ResourceManagers should use this to access their SharedResource whenever
     * they need to write to (or otherwise change) the resource.
     *
     * @return  A write-locked pointer to the class SharedResource.
     */
    LockedResourcePtr getWriteLockedResource()
    {
        return LockedResourcePtr(*this, true);
    }
};
