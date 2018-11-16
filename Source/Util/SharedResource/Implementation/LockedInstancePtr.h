#pragma once
#include "JuceHeader.h"
#include "SharedResource/Implementation/Implementation.h"

/**
 * @file  SharedResource/Implementation/LockedInstancePtr.h
 *
 * @brief  Provides access to a resource Instance, keeping the resource locked
 *         for as long as the LockedInstancePtr exists.
 */
class SharedResource::LockedInstancePtr
{
public:

    /* LockedInstancePtr may only be created as the parent class of 
       a LockedPtr<ResourceType> object. */
    template<class ResourceType>
    friend class LockedPtr;

private:
    /**
     * @brief  Initializes the instance pointer, locking the resource.
     *
     * @param resourceKey  Identifies the resource Instance this pointer 
     *                     will access.
     *
     * @param lockType     The type of lock used to secure the resource.
     */
    LockedInstancePtr(const juce::Identifier& resourceKey,
            const LockType lockType);

public:
    /**
     * @brief  Unlocks the resource when the LockedInstancePtr is 
     *         destroyed.
     */
    virtual ~LockedInstancePtr();

    /**
     * @brief  Accesses the resource Instance's methods or data.
     *
     * @return  The address of the resource Instance.
     */
    Instance* operator->() const;

private:
    /* The type of resource lock the LockedInstancePtr maintains. */
    const LockType lockType;
    /* Identifies the resource accessed by this LockedInstancePtr. */
    const juce::Identifier& resourceKey;
};
