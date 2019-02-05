#pragma once
#include "SharedResource_Modular_Handler.h"
#include <functional>

namespace SharedResource { namespace Modular { 
        template<class ResourceType> class Module; } }

template<class ResourceType>
class SharedResource::Modular::Module
{
private:
    ResourceType& parentResource;

protected:
    Module(ResourceType& parentResource) : parentResource(parentResource) { }

    template<class HandlerType>
    void foreachHandler(const std::function<void(HandlerType*)> notifyAction)
    {
        parentResource.template foreachHandler<HandlerType>(notifyAction);
    }

    ResourceType& getResource()
    {
        return parentResource;
    }

    const ResourceType& getConstResource() const
    {
        return parentResource;
    }
    
    template<class ModuleType>
    ModuleType* getSiblingModule()
    {
        return parentResource.template getModule<ModuleType>();
    }
    
    template<class ModuleType>
    const ModuleType* getConstSiblingModule() const
    {
        return parentResource.template getModule<const ModuleType>();
    }
};
