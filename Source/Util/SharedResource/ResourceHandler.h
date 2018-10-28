#pragma once
#include "JuceHeader.h"
#include "SharedResource.h"

/**
 * @file ResourceHandler.h
 *
 * @brief  Provides a more convenient interface for SharedResource::Handler 
 *         objects, interacting with their specific SharedResource type.
 *
 * @tparam ResourceType  The type of SharedResource class managed by the
 *                       ResourceHandler.
 */

template <class ResourceType>
class ResourceHandler : public SharedResource::Handler
{
protected:
    /**
     * @brief  Creates a new ResourceHandler, initializing its SharedResource
     *         if necessary.
     *
     * @param resourceKey     The unique key string identifying the ResourceType
     *                        SharedResource.
     *
     * @param createResource  A function that will create the resource if it
     *                        has not yet been initialized.
     */
    ResourceHandler(
            const juce::Identifier& resourceKey,
            const std::function<SharedResource*()> createResource) :
    SharedResource::Handler(resourceKey, createResource) { }

public:
    virtual ~ResourceHandler() { }

protected:
    /**
     * @brief  A custom pointer object holding the class SharedResource.  As 
     *         long as this object exists, the SharedResource will be locked.
     */
    class LockedResourcePtr
    {
    private:
        /* Disallow direct creation of LockedResourcePtr objects. */
        friend class ResourceHandler;

        /**
         * Create the pointer, locking the SharedResource for reading.
         *
         * @param resourceManager  The ResourceHandler creating this pointer.
         *
         * @param lockType         Sets if the resource will be locked for
         *                         writing, blocking all other access, or locked
         *                         for reading, only blocking write attempts.
         */
        LockedResourcePtr(const ResourceHandler& resourceManager, 
                SharedResource::LockType lockType) : 
        resourceManager(resourceManager), lockType(lockType)
        {
            if(lockType == SharedResource::LockType::read)
            {
                resourceManager.getResourceLock().enterRead();
            }
            else
            {
                resourceManager.getResourceLock().enterWrite();
            }
        }

    public:
        /**
         * Destroy the pointer, unlocking the SharedResource.
         */
        virtual ~LockedResourcePtr()
        {
            
            if(lockType == SharedResource::LockType::read)
            {
                resourceManager.getResourceLock().exitRead();
            }
            else
            {
                resourceManager.getResourceLock().exitWrite();
            }
        }

        /**
         * Gets the SharedResource held by this pointer.
         *
         * @return  The single SharedResource instance.
         */
        ResourceType& operator*() const
        {
            return *resourceManager.getClassResource();
        }

        /**
         * Accesses the held SharedResource instance's methods or data.
         *
         * @return  The address of the SharedResource instance.
         */
        ResourceType* operator->() const
        {
            return static_cast<ResourceType*>
                    (resourceManager.getClassResource());
        }

        /**
         * Checks if another SharedResource pointer points to this object's
         * SharedResource.
         *
         * @param rhs  A pointer to a SharedResource.
         *
         * @return     Whether rhs has the same address as this object's
         *             SharedResource.
         */
        bool operator==(const SharedResource* rhs) const
        {
            return resourceManager.classResource.get() == rhs;
        }

    private:
        /* Used to access the SharedResource and lock. */
        const ResourceHandler& resourceManager;

        /* Indicates the lock type. */
        const SharedResource::LockType lockType;
    };
    
    /**
     * @brief  Gets a pointer to the class resource, locking it for reading.
     *
     * ResourceHandlers should use this to access their SharedResource whenever
     * they need to read data from it without changing it.
     *
     * @return  A read-locked pointer to the class SharedResource.
     */
    LockedResourcePtr getReadLockedResource() const
    {
        return LockedResourcePtr(*this, SharedResource::LockType::read);
    }

    /**
     * @brief  Gets a pointer to the class resource, locking it for writing.
     *
     * ResourceHandlers should use this to access their SharedResource whenever
     * they need to write to (or otherwise change) the resource.
     *
     * @return  A write-locked pointer to the class SharedResource.
     */
    LockedResourcePtr getWriteLockedResource() const
    {
        return LockedResourcePtr(*this, SharedResource::LockType::write);
    }
};
