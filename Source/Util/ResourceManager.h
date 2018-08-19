#pragma once
#include "JuceHeader.h"

/**
 * @file ResourceManager.h
 *
 * @brief An abstract basis for classes that control access to SharedResource
 *        objects.
 *
 * Each class inheriting ResourceManager shares access to a single
 * SharedResource object.  When a ResourceManager object is instantiated, 
 * a new SharedResource instance will only be created if no other instance 
 * of its ResourceManager class exists.  That SharedResource will only be 
 * destroyed when the last instance of its ResourceManager is destroyed.
 *
 * In order for each ResourceManager subclass to control exactly one object,
 * each must maintain a static ScopedPointer to hold the SharedResource, and a
 * static juce::ReadWriteLock to control access to that resource.  After
 * construction, the ResourceManager should not access these static variables
 * directly. Instead, it should use GetResourceForRead() or getResourceForWrite()
 * to access 
 *
 * As this application is multi-threaded, all SharedResource implementations
 * should disallow concurrent modification.  Each ResourceManager subclass
 * will have a shared Juce::ReadWriteLock that should be used whenever the
 * shared resource is accessed.  
 */

template <class ResourceType>
class ResourceManager
{
public:
    /**
     * @param classResource    This must be a reference to a static 
     *                         ScopedPointer that will hold the class 
     *                         SharedResource. This will initialize the 
     *                         resource if necessary, and add the 
     *                         ResourceManager to the SharedResource's 
     *                         reference. 
     *                        
     *                         ResourceManager instances should never assign the
     *                         classResource ScopedPointer themselves.
     * 
     * @param resourceLock     A reference to a static ReadWriteLock to be
     *                         shared by the class.  This must be locked any
     *                         time the sharedResource is accessed. 
     * 
     * @param resourceCreator  This function will be used to create the 
     *                         SharedResource if necessary.  This should not
     *                         acquire the resource lock.
     */
    template <class ResourceType>
    ResourceManager(
            juce::ScopedPointer<ResourceType>& classResource,
            juce::ReadWriteLock& resourceLock,
            std::function<ResourceType*()> resourceCreator)
    {
        using namespace juce;
        const ScopedWriteLock initLock(resourceLock);
        if (classResource == nullptr)
        {
            classResource = createResource();
        }
        classResource->referenceList.add(this);
    }
            
    /**
     * This will remove itself from the sharedResource's reference list, and
     * destroy the resource if this was the last reference in the list.
     * 
     * @tparam ResourceType    The SharedResource subclass type.
     *
     */
    template <class ResourceType>
    virtual ~ResourceManager()
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

protected:
    /**
     * A constant pointer to the SharedResource held by this ResourceManager.
     * This will lock the resource when created, and unlock the resource
     * when destroyed.
     */
    template <class ResourceType>
    class LockedResourcePtr
    {
    public:
        friend class ResourceManager;
    
        /**
         * Used to select how the resource will be locked.
         */
        enum LockType
        {
            readLock,
            writeLock
        };
                
        /**
         * Unlocks the resource when this pointer is destroyed.
         *
         * @tparam ResourceType  The SharedResource subclass type.
         */
        template <class ResourceType>
        ~LockedResourcePtr()
        {
            if(lockType == readLock)
            {
                resourceLock.exitRead();
            }
            else if(lockType == writeLock)
            {
                resourceLock.exitWrite();
            }
        }
        
        /**
         * Accesses the stored resource.
         *
         * @tparam ResourceType  The SharedResource subclass type.
         *
         * @return  A pointer to the shared resource.
         */
        template <class ResourceType>
        ResourceType* operator*()
        {
            return *resource;
        }
        
        /**
         * Accesses the stored resource.
         *
         * @tparam ResourceType  The SharedResource subclass type.
         *
         * @return  A pointer to the shared resource.
         */
        template <class ResourceType>
        ResourceType* operator->()
        {
            return *resource;
        }
    
    private:
        /**
         * Creates a new locked resource pointer, acquiring the
         * resource lock.  Only ResourceManager may create these pointers.
         *
         * @tparam ResourceType    The SharedResource subclass type.
         *
         * @param  resourceHolder  A reference to the ScopedPointer holding
         *                         the resource.
         *
         * @param  resourceLock    A reference to the resource's lock.
         *
         * @param  lockType        Sets if the resource should be locked for
         *                         reading or for writing.
         */
        template <class ResourceType>
        LockedResourcePtr(
                juce::ScopedPointer<ResourceType>& resourceHolder,
                ReadWriteLock& resourceLock,
                LockType lockType) : 
        resourceHolder(resourceHolder),
        resourceLock(resourceLock),
        lockType(lockType)
        {
            if(lockType == readLock)
            {
                resourceLock.enterRead();
            }
            else if(lockType == writeLock)
            {
                resourceLock.enterWrite();
        }
                
        juce::ScopedPointer<ResourceType>& resourceHolder;
        juce::ReadWriteLock& resourceLock;
    };
    
    /**
     * Gets a pointer to the managed resource, acquiring a read lock for as long as
     * this pointer exists.
     *
     * @tparam ResourceType  The SharedResource subclass type.
     *
     * @return  A pointer to the shared resource.
     */
    template <class ResourceType>
    LockedResourcePtr<ResourceType> getResourceForRead()
    {
        return LockedResourcePtr<ResourceType>(resourceHolder, resourceLock, readLock);
    }
      
    /**
     * Gets a pointer to the managed resource, acquiring a write lock for as long as
     * this pointer exists.
     *
     * @tparam ResourceType  The SharedResource subclass type.
     *
     * @return  A pointer to the shared resource.
     */
    template <class ResourceType>
    LockedResourcePtr<ResourceType> getResourceForWrite()
    {
        return LockedResourcePtr<ResourceType>(resourceHolder, resourceLock, writeLock);
    }

private:
    /**
     * Needed in order to update the resource's reference list when this
     * object is destroyed.
     */
    juce::ScopedPointer<ResourceType>& classResource;

    /**
     * Shared by all instances of the ResourceManager, used to control access to
     * the shared resource object.
     */
    juce::ReadWriteLock& resourceLock;
};
