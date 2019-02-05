#pragma once
//TODO: Update and transfer module documentation and get rid of this file.
#error "Do not include SharedResource.h, module header files are being phased out"


/**
 * @file  SharedResource/SharedResource.h
 *
 * @brief  The public interface to the SharedResource module, used to implement
 *         threadsafe, reference counted singleton classes.
 *
 * Each Resource subclass has at most one object instance. Resources may only be
 * directly interacted with through subclasses of the SharedResource::Handler 
 * class.  Sharing a single object like this ensures that concurrent acccess to 
 * system resources can be controlled, and expensive objects are not duplicated 
 * unnecessarily.
 *
 * Each Resource subclass has at least one SharedResource::Handler class 
 * assigned to it. These handlers control all access to the Resource object 
 * instance, and control its lifecycle. The SharedResource object is
 * only created when one of its Handler objects is created, and it is only 
 * destroyed when all of its handlers have been destroyed. This allows 
 * SharedResource initialization and destruction to be controlled using 
 * RAII techniques, by declaring a Handlers at the highest scope where the 
 * resource will be needed.  
 *
 * Each Resource object instance can only be accessed by its handlers while it
 * is locked, either for reading or writing, as defined by juce::ReadWriteLock.
 * This ensures that Resource data will not be changed while it is being read, 
 * or while another thread is already making changes.  This allows Handler 
 * objects across multiple threads to share a single resource object safely.  
 * To prevent deadlocks, Handler objects with different resources should 
 * interact with eachother very carefully, taking extra care not to introduce
 * circular dependencies. 
 *
 * Resources are able to selectively apply functions to all of their Handler
 * objects, or to only Handler objects with a specific subclass.  This allows
 * Handler objects to be implemented as Listener objects, receiving updates 
 * whenever the Resource requires.  This process locks the resource for reading
 * until every Handler has been notified, so Handlers must take care not to 
 * synchronously lock the resource for writing or create new Handlers in
 * response to these signals.
 */
namespace SharedResource
{
    /**
     * @brief  A basis for shared, threadsafe, reference counted singleton 
     *         classes.
     *
     * @see  SharedResource/Resource.h
     */
    class Resource;

    /**
     * @brief  A basis for Resource classes that provide a shared program
     *         thread.
     *
     * @see  SharedResource/ThreadResource.h
     */
    class ThreadResource;

    /**
     * @brief  A basis for classes that access a shared resource and manage its
     *         lifespan.
     *
     * @tparam ResourceType  The Resource subclass accessed and managed by the 
     *                       Handler.
     */
    template<class ResourceType> class Handler;

    /**
     * @brief  A custom pointer object used by Handler classes to simultaneously
     *         access and lock their Resource.
     *
     * @tparam ResourceType  The Resource subclass locked and accessed by the
     *                       LockedPtr.
     */
    template<class ResourceType> class LockedPtr;

    /**
     * @brief  Describes the types of locks that can be applied to a Resource.
     */
    enum class LockType
    {
        // Allow multiple threads to read from the resource, but keep them
        // from writing to the resource until all read locks are released.
        read,
        // Allows a single thread to read from and write to the resource,
        // blocking all others until the write lock is released.
        write
    };
}
