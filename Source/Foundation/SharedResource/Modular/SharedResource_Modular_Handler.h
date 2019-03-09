#pragma once
#include "SharedResource_Handler.h"
#include "SharedResource_Modular_LockedPtr.h"

namespace SharedResource { namespace Modular { 
        template<class ResourceType> class Module; } }
namespace SharedResource { namespace Modular { 
        template<class ResourceType, class ModuleType> class Handler; } }
namespace SharedResource { namespace Modular { 
        template<class ResourceType, class ModuleType> class LockedPtr; } }

template <class ResourceType, class ModuleType>
class SharedResource::Modular::Handler : 
        public SharedResource::Handler<ResourceType>
{
public:
    virtual ~Handler() { }

protected:
    using SharedResource::Handler<ResourceType>::Handler;

    LockedPtr<ResourceType, const ModuleType> getReadLockedResource() const
    {
        return LockedPtr<ResourceType, const ModuleType>
                (Handler::getResourceKey(), LockType::read);
    }

    LockedPtr<ResourceType, ModuleType> getWriteLockedResource() const
    {
        return LockedPtr<ResourceType, ModuleType>
                (Handler::getResourceKey(), LockType::write);
    }
};
