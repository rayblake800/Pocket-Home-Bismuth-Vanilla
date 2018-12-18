#pragma once
#include "SharedResource/SharedResource.h"
#include "SharedResource/Implementation/LockedInstancePtr.h"

/**
 * @file  SharedResource/LockedPtr.h
 *
 * @brief  Provides access to a specific SharedResource::Resource, keeping it
 *         locked for as long as the LockedPtr exists.
 *
 * @tparam ResourceType  The SharedResource::Resource subclass this LockedPtr
 *                       accesses.
 */
template<class ResourceType>
class SharedResource::LockedPtr : public LockedInstancePtr
{
private:
    /**
     * @brief  Locks the ResourceType resource for as long as the LockedPtr
     *         exists.
     *
     * @param resourceKey  The unique key string identifying the ResourceType
     *                     Resource class.
     *
     * @param lockType     The type of lock used to secure the resource.
     */
    LockedPtr(const juce::Identifier& resourceKey, const LockType lockType) : 
        LockedInstancePtr(resourceKey, lockType) { }

public:
    virtual ~LockedPtr() { }

    /* LockedPtr objects should only be created and used internally by 
       Handler objects. */
    template <class LockType> friend class Handler;

    /**
     * @brief  Accesses the locked resource's methods or data.
     *
     * @return  The address of the locked resource object instance.
     */
    ResourceType* operator->() const
    {
        return static_cast<ResourceType*>(getInstance());
    }
};

