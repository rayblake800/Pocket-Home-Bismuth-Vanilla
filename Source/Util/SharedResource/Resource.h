#pragma once
#include "JuceHeader.h"
#include "SharedResource/Implementation/Instance.h"

/**
 * @file SharedResource/Resource.h
 *
 * @brief  A basis for threadsafe, RAII-managed Singleton objects.
 *
 * @see SharedResource.h
 *
 * Resource object subclasses define access-controlled, reference counted
 * singletons.  Each Reference subclass is automatically created and destroyed
 * by its corresponding SharedResource::Handler class or classes, and may only
 * be accessed through these Handler classes.
 *
 * Each concrete Resource subclass must declare a unique, constant identifying
 * key, publically available as a juce::Identifier named resourceKey. Resource
 * subclasses must also only use the default constructor.
 */
class SharedResource::Resource : public Instance
{
public: 
    /**
     * @brief  Creates the single resource object instance.
     *
     * @param resourceKey  The unique key used to identify this resource.     
     */
    Resource(const juce::Identifier& resourceKey);

    virtual ~Resource() { }

protected:
    /**
     * @brief  Packages an asynchronous action so that it will check if the 
     *         SharedResource instance that created it is still valid.  
     *
     *  If it is valid,  the SharedResource instance will be locked and the 
     *  action will be executed.
     *
     * @param lockType     Sets how the resource will be locked while the 
     *                     action function runs.
     *
     * @param action       A function to call if the SharedResource 
     *                     still exists.
     *
     * @param ifDestroyed  An optional alternate function to call if the
     *                     SharedResource was destroyed.
     */
    std::function<void()> buildAsyncFunction(
            LockType lockType, 
            std::function<void()> action,
            std::function<void()> ifDestroyed = [](){});

    /**
     * @brief  Runs an arbitrary function on each Handler object with type
     *         HandlerType connected to the resource.
     *
     * @tparam HandlerType   The handlerAction will only run for Handlers that
     *                       have this type.
     *
     * @param handlerAction  Some action that should run for every HandlerType
     *                       connected to this SharedResource, passing in a
     *                       pointer to the HandlerType as a parameter.
     */
    template<class HandlerType>
    void foreachHandler(const std::function<void(HandlerType*)> handlerAction)
    {
        foreachReference([&handlerAction](Reference* reference)
        {
            HandlerType* handler = dynamic_cast<HandlerType*>(reference);
            if(handler != nullptr)
            {
                handlerAction(handler);
            }
        });
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Resource)
};
