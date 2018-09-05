#pragma once
#include "JuceHeader.h"
#include "ThreadLock.h"

/**
 * @file SharedResource.h
 *
 * @brief  A basis for threadsafe, RAII-managed Singleton objects.
 *
 * Each SharedResource subclass has at most one object instance. SharedResources
 * may only be directly interacted with through subclasses of the 
 * SharedResource::Handler class.  Sharing a single object like this ensures 
 * that concurrent acccess to system resources can be controlled, and expensive
 * objects are not duplicated unnecessarily.
 *
 * Each SharedResource subclass has at least one SharedResource::Handler class
 * assigned to it.  These managers define the lifecycle of the SharedResource 
 * object, and provide access to it. The SharedResource object is
 * only created when one of its Handler objects is created, and it is only 
 * destroyed when all of its handlers have been destroyed. This allows 
 * SharedResource initialization and destruction to be controlled using 
 * RAII techniques, by declaring a Handlers at the highest scope where the 
 * resource will be needed.  
 *
 * Each SharedResource object type has a single ThreadLock object, available to
 * all of its handlers.  This is used to ensure that SharedResource data 
 * will not be changed while it is being read, or while another thread is
 * already making changes.  This allows Handler objects across multiple threads
 * to share a single resource object safely.  To prevent deadlocks, Handler
 * objects should interact with eachother very carefully, or not at all.
 *
 * Each SharedResource subclass also has a single corresponding 
 * SharedResource::Listener subclass, defining objects that receive updates when 
 * the SharedResource changes.  Listeners automatically subscribe themselves to
 * updates from their resource, and prevent the resource from being destroyed
 * before they are. The SharedResource sends notifications to its listeners
 * asynchronously, locking itself for writing before doing so.
 *
 * SharedResources may access their handler list, allowing handlers to be used
 * as Listener objects.
 *
 * @see ResourceManager.h, ResourceListener.h, ThreadLock.h
 */

class SharedResource
{
public: 
    /**
     * Creates the single resource object instance.
     *
     * @param resourceKey     A unique key string used to identify this
     *                        resource.     
     */
    SharedResource(const juce::Identifier& resourceKey);

    /**
     * In debug builds, print an error message if the resource is destroyed
     * while its Handler list is not empty.
     */
    virtual ~SharedResource();

    /**
     * Interface for objects that directly access a SharedResource object.
     */
    class Handler
    {
    protected:
        /**
         * Creates a new Handler for a SharedResource, initializing the resource
         * if necessary.
         *
         * @param resourceKey     A unique key string used to identify the 
         *                        Handler's SharedResource type.
         *
         * @param createResource  A function that will create the resource if it
         *                        has not yet been initialized.
         */
        Handler(const juce::Identifier& resourceKey,
                std::function<SharedResource*()> createResource);
    
    public:
        /**
         * Removes a handler from the handler list, destroying the resource if
         * no handlers remain.
         */
        ~Handler();

    protected:
        /**
         * Gets a pointer to the SharedResource object shared by all objects of 
         * this Handler subclass.
         *
         * @return  The class SharedResource pointer.
         */
        SharedResource* getClassResource();

        /**
         * Gets a reference to the lock used to control access to the shared
         * resource.
         *
         * @return  The class resource lock.
         */
        ThreadLock& getResourceLock();

    private:
        // The key used to select this object's SharedResource.
        const juce::Identifier& resourceKey;        
    };
    
    //Selects a type of lock to use to protect the SharedResource.
    enum LockType
    {
        read,
        write
    };

protected:
    /**
     * Packages an asynchronous action so that it will check if the 
     * SharedResource instance that created it stil valid.  If it is valid,
     * the SharedResource instance will be locked and the action will be
     * executed.
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
private:
    /*
     * Holds pointers to every Handler that accesses this SharedResource.  This 
     * is used to ensure the SharedResource is never destroyed before all of its
     * Manager objects.
     */
    juce::Array<const SharedResource::Handler*> resourceHandlers;

    /*
     * The lock used to control access to this SharedResource.
     */
    ThreadLock resourceLock;
      
    // The key string identifying this SharedResource instance.
    const juce::Identifier& resourceKey;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SharedResource)
};
