#pragma once
#include "JuceHeader.h"
#include "SharedResource/SharedResource.h"
#include "SharedResource/LockedPtr.h"
#include "SharedResource/Implementation/Reference.h"

/**
 * @file Handler.h
 *
 * @brief  Provides safe access to a specific SharedResource::Resource subclass,
 *         ensuring the single Resource object instance exists as long as any of
 *         its Handlers exists, destroying it when its last handler is 
 *         destroyed.
 *
 * @tparam ResourceType  The specific SharedResource::Resource subclass managed 
 *                       by the ResourceHandler.
 */

template <class ResourceType>
class SharedResource::Handler : public SharedResource::Reference
{
protected:
    /**
     * @brief  Creates a new Handler, initializing its Resource if necessary.
     *
     * This constructor determines the resource key directly from the
     * ResourceType, and should not be used to implement abstract Resource
     * subclasses.
     *
     * @param createResource  An optional function that will create the
     *                        resource.  If no function is given, the default
     *                        constructor for ResourceType will be used.
     */
    Handler(const std::function<ResourceType*()> createResource
            = [](){ return new ResourceType(); }) : 
    Reference(ResourceType::resourceKey, 
            [createResource]()->Instance* 
            { 
                return static_cast<Instance*>(createResource());
            }),
    resourceKey(ResourceType::resourceKey) { }

    /**
     * @brief  Creates a new Handler, initializing its Resource if necessary.
     *
     * This constructor should be used when implementing abstract Resource
     * subclasses, where the resourceKey cannot be determined automatically.
     *
     * @param resourceKey     The unique key string identifying the ResourceType
     *                        Resource class.
     *
     * @param createResource  An optional function that will create the
     *                        resource.  If no function is given, the default
     *                        constructor for ResourceType will be used.
     */
    Handler(const juce::Identifier& resourceKey,
            const std::function<ResourceType*()> createResource
                = [](){ return new ResourceType(); }) : 
    Reference(resourceKey, 
            [createResource]()->Instance* 
            { 
                return static_cast<Instance*>(createResource());
            }),
    resourceKey(resourceKey) { }

public:
    virtual ~Handler() { }

protected:
    /**
     * @brief  Gets a pointer to the class resource, locking it for reading.
     *
     * Handlers should use this to access their Resource whenever they need to 
     * read data from it without changing it.
     *
     * @return  A read-locked pointer to the class Resource instance.
     */
    LockedPtr<ResourceType> getReadLockedResource() const
    {
        return LockedPtr<ResourceType>(resourceKey, LockType::read);
    }

    /**
     * @brief  Gets a pointer to the class resource, locking it for writing.
     *
     * Handlers should use this to access their Resource whenever they need to 
     * write to (or otherwise change) the resource.
     *
     * @return  A write-locked pointer to the class Resource instance.
     */
    LockedPtr<ResourceType> getWriteLockedResource() const
    {
        return LockedPtr<ResourceType>(resourceKey, LockType::write);
    }

private:
    /* The unique key identifier used to find the connected Resource instance.*/
    const juce::Identifier& resourceKey;
};
