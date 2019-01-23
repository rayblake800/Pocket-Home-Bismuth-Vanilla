#pragma once

#include "SharedResource_LockedInstancePtr.h"

namespace SharedResource { namespace Modular { 
        template<class ResourceType, class ModuleType> class LockedPtr; } }

template <class ResourceType, class ModuleType>
class SharedResource::Modular::LockedPtr : public LockedInstancePtr
{
private:
    /* LockedPtr objects should only be created and used internally by 
       Handler objects. */
    template <class LockedResource, class LockedModule> friend class Handler;

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

    /**
     * @brief  Accesses the locked resource module's methods or data.
     *
     * @return  The address of the locked resource object instance.
     */
    ModuleType* operator->() const
    {
        return static_cast<ResourceType*>(getInstance())->
                template getModule<ModuleType>();
    }
};
