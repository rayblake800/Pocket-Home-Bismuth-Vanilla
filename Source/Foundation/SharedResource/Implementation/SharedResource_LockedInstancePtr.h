#pragma once
/**
 * @file  SharedResource/Implementation/LockedInstancePtr.h
 *
 * @brief  Provides access to a resource Instance, keeping the resource locked
 *         for as long as the LockedInstancePtr exists.
 */

#include "JuceHeader.h"
#include "SharedResource_LockType.h"

namespace SharedResource { class LockedInstancePtr; }
namespace SharedResource { class Instance; }
namespace SharedResource { 
        template<class ResourceType> class LockedPtr; }
namespace SharedResource { namespace Modular { 
        template<class ResourceType, class ModuleType> class LockedPtr; } }

class SharedResource::LockedInstancePtr
{
public:

    /* LockedInstancePtr may only be created as the parent class of a LockedPtr 
       object. */
    template<class ResourceType> 
            friend class LockedPtr;
    template<class ResourceType, class ModuleType> 
            friend class Modular::LockedPtr;

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
     * @brief  Unlocks the resource. Once the resource is unlocked, the 
     *         LockedInstancePtr may no longer access the resource Instance or
     *         lock.
     */
    void unlock();

    /**
     * @brief  Checks if the resource is still locked by this pointer.
     *
     * @return  True if the resource is locked and available, false if it is
     *          unlocked and unavailable.
     */
    bool isLocked() const;

protected:
    /**
     * @brief  Accesses the resource Instance's methods or data.
     *
     * @return  The address of the resource Instance, or nullptr if the
     *          resource is unlocked.
     */
    Instance* getInstance() const;

private:
    /* The type of resource lock the LockedInstancePtr maintains. */
    const LockType lockType;
    /* Identifies the resource accessed by this LockedInstancePtr. */
    const juce::Identifier& resourceKey;
    /* Stores if the resource is currently locked and may be accessed. */
    bool locked = false;
};
