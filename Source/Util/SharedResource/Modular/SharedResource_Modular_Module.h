#pragma once
#include "SharedResource_Modular_Handler.h"
#include <functional>

namespace SharedResource { namespace Modular { 
        template<class ResourceType> class Module; } }

template<class ResourceType>
class SharedResource::Modular::Module
{
private:
    ResourceType& resource;

protected:
    Module(ResourceType& resource) : resource(resource) { }

    template<class HandlerType>
    void foreachHandler(const std::function<void(HandlerType*)> notifyAction)
    {
        resource. template foreachHandler<HandlerType>(notifyAction);
    }
};
