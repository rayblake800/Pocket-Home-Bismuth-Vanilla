/* Only include when implementing SharedResource! */
#ifdef SHARED_RESOURCE_IMPLEMENTATION
#pragma once
#include <map>
#include "JuceHeader.h"
#include "SharedResource/SharedResource.h"

/**
 * @file  SharedResource/Holder.h
 *
 * @brief  Holds each SharedResource subclass's single object instance. 
 */
namespace SharedResource
{
    /* Holds all data needed for managing a Resource's single object instance.*/
    class InstanceData;

    class Holder
    {
    private:
        Holder() { }

    public:
        /**
         * @brief  Clears all resource locks.
         *
         * This should never run before all resources are cleared. In debug
         * builds, this will verify that each InstanceData pointer is null.
         */
        ~Holder();

        /**
         * @brief  Gets the program's sole SharedResource::Holder instance.
         *
         * @return  The holder managing all InstanceData objects.
         */
        static Holder* getHolderInstance();

        /**
         * @brief  Finds and gets an InstanceData pointer using its resource 
         *         key.
         *
         * @param resourceKey  A unique key identifying a Resource subclass.
         *
         * @return             A pointer to the requested resource's
         *                     InstanceData object. This value will be null if 
         *                     the Resource has not yet been initialized. 
         */
        InstanceData* getResource(const juce::Identifier& resourceKey);

        /**
         * @brief  Saves the address of a new resource instance. This should
         *         only be called when the existing resource instance at the
         *         given ID is null, or to set the pointer to null while
         *         destroying the resource.
         *
         * @param resourceKey  A unique key identifying a Resource subclass.
         *
         * @param resource     A pointer to the new resource instance, or 
         *                     nullptr if destroying the existing resource. 
         */
        void setResource(const juce::Identifier& resourceKey,
                InstanceData* resource);

        /**
         * @brief  Gets the lock used to control access to a resource instance.
         *
         * @param resourceKey  A unique key identifying a Resource subclass.
         *
         * @return             The lock used to control access to that resource. 
         */
        const juce::ReadWriteLock& getResourceLock
        (const juce::Identifier& resourceKey);

    private:
        /**
         * @brief  Gets the index where a resource and its lock are stored in
         *         the resourceList and resourceLocks.  If necessary, this will
         *         initialize the resource container, creating a new lock and
         *         assigning an index.
         *
         * @param resourceKey  A unique key identifying a Resource subclass.
         *
         * @return  The index needed to find the resource index and its lock.
         */
        int getResourceIndex(const juce::Identifier& resourceKey);

        /**
         * @brief  Initializes a resource container, assigning it an index, 
         *         creating its lock, and setting the initial resource instance 
         *         address as nullptr.
         *
         * @param resourceKey  A unique key identifying a Resource subclass.
         */
        void initResource(const juce::Identifier& resourceKey);

        /* Prevents concurrent access to the containers holding resource data.*/
        static juce::CriticalSection holderLock;

        /* Maps each resource ID to the index where its InstanceData and lock
           are stored. */
        std::map<juce::Identifier, int> resourceIndices;

        /* Holds all resource instances. */
        juce::Array<InstanceData*> resourceList;
        
        /* Holds all resource locks. */
        juce::OwnedArray<juce::ReadWriteLock> resourceLocks;
    };
}
/* Only include when implementing SharedResource! */
#endif
