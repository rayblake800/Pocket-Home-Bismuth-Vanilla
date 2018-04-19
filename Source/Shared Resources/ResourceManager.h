#pragma once
#include "JuceHeader.h"

/**
 * @file ResourceManager.h
 *
 * @brief ResourceManager is an abstract class that provides a framework for
 *        classes that share a single resource between all class instances.
 *
 * This shared resource inherits from ResourceManager::SharedResource.  When 
 * a subclass of ResourceManager is instantiated, the SharedResource will only
 * be created if no other instance of that class exists.  That shared resource
 * will only be destroyed when the last instance of its ResourceManager is 
 * destroyed.
 *
 * As this application is multi-threaded, all SharedResource implementations
 * should disallow concurrent access.  This is relatively simple to do safely, 
 * as long as all implementations strictly follow these rules:
 * 
 * 1. ResourceManager may only interact with the shared resource after acquiring
 *    the resource lock.  
 * 
 * 2. While it holds the resource lock, the ResourceManager cannot call any 
 *    other method that interacts with the shared resource.
 * 
 * 3. SharedResource should never share references or pointers to its internal
 *    data with any other object.
 * 
 * 4. ResourceManagers and SharedResources must never have any circular 
 *    dependencies.
 */

class ResourceManager
{
public:

    class SharedResource
    {
    public:
        friend class ResourceManager;
        /**
         * Throws an error if the resource is destroyed while the reference
         * list is not empty.
         */
        virtual ~SharedResource();

    protected:

        SharedResource() { }

    private:
        //Holds a reference to every existing ResourceManager that uses this
        //resource.
        Array<ResourceManager*> referenceList;
    };

    /**
     * @param classResource    This must be a reference to a static 
     *                          ScopedPointer that will hold the class 
     *                          SharedResource. This will initialize the 
     *                          resource if necessary, and add the ResourceManager
     *                          to the SharedResource's reference. 
     *                        
     *                          ResourceManager instances should never assign the
     *                          classResource ScopedPointer themselves.
     * 
     * @param resourceLock     A reference to a static CriticalSection to be
     *                          shared by the class.  This must be locked any
     *                          time the sharedResource is accessed. 
     * 
     * @param resourceCreator  This function will be used to create the 
     *                          SharedResource if necessary.  This should not
     *                          acquire the resource.
     */
    ResourceManager(
            ScopedPointer<SharedResource>& classResource,
            CriticalSection& resourceLock,
            std::function<SharedResource*()> resourceCreator);


    /**
     * This will remove itself from the sharedResource's reference list, and
     * destroy the resource if this was the last reference in the list.
     */
    virtual ~ResourceManager();

private:

    /**
     * Needed in order to update the resource's reference list when this
     * object is destroyed.
     */
    ScopedPointer<SharedResource>& classResource;

    /**
     * Shared by all instances of the ResourceManager, used to control access to
     * the shared resource object.
     */
    CriticalSection& resourceLock;


};
