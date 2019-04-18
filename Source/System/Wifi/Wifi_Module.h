#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Module.h
 *
 * @brief  A basis for submodules of the shared Wifi resource.
 */

#include "SharedResource_Modular_Module.h"
#include "JuceHeader.h"

namespace Wifi
{
    class Resource;
    class Module;
}

/**
 * @brief  A basis for module objects that handle part of the responsibilities
 *         of the Wifi::Resource object.
 */
class Wifi::Module : public SharedResource::Modular::Module<Resource>
{
public:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    Module(Resource& parentResource);

    virtual ~Module() { }

protected:
    /**
     * @brief  Packages an asynchronous action so that it will check if the
     *         Wifi::Resource instance that created it is still valid.
     *
     *  If it is valid, the Resource instance will be locked and the action
     * will be executed.
     *
     * @param lockType     Sets how the resource will be locked while the
     *                     action function runs.
     *
     * @param action       A function to call if the Resource still exists.
     */
    std::function<void()> buildAsyncFunction(SharedResource::LockType lockType,
            std::function<void()> action);

    /**
     * @brief  Runs a callback function on the JUCE message thread for each of
     *         this module's handler objects that share a specific class.
     *
     * @tparam HandlerType   The type of handler that should be acted on.
     *
     * @param handlerAction  A function to run on each valid module handler of
     *                       type HandlerType.
     */
    template <class HandlerType>
    void foreachModuleHandler
    (const std::function<void(HandlerType*)> handlerAction)
    {
        std::function<void()> asyncAction = buildAsyncFunction(
                SharedResource::LockType::read,
                [this, handlerAction]()
                {
                    foreachHandler<HandlerType>(handlerAction);
                });
        juce::MessageManager::callAsync(asyncAction);
    }
};
