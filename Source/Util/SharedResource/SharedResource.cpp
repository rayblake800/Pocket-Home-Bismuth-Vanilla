#include <map>
#include "SharedResource.h"

/* Holds each SharedResource subclass's single object. */
static std::map<juce::Identifier, SharedResource*> 
    resourceMap;

/* Locks the resource map when resources are being created or destroyed. */
static juce::ReadWriteLock resourceMapLock;

/*
 * Creates the single object instance shared by a ResourceManager
 * subclass.
 */
SharedResource::SharedResource(const juce::Identifier& resourceKey) :
resourceKey(resourceKey)
{
    using namespace juce;
    const ScopedWriteLock resourceInitLock(resourceMapLock);
    jassert(resourceMap[resourceKey] == nullptr);
    resourceMap[resourceKey] = this;
    DBG("SharedResource: Creating resource \"" << resourceKey.toString()
            << "\"");

    /* The handler creating this resource can't add itself to the resource's
     * handler list until all of the SharedResource's classes have been 
     * constructed.  This means that if one of those constructors creates and
     * then destroys a Handler, it could inadvertently destroy itself.  To
     * prevent this, the handler list is created with an initial null handler,
     * which the creating Handler will replace.
     */
    resourceHandlers.add(nullptr);
}

/*
 * In debug builds, print an error message if the resource is destroyed
 * while its ResourceManager list is not empty.
 */
SharedResource::~SharedResource()
{
    using namespace juce;
    if(!resourceHandlers.isEmpty())
    {
        DBG("SharedResource::~SharedResource: Destroying resource while list of"
                << " ResourceManagers is not empty!");
    }    
    DBG("SharedResource: Destroying resource \"" << resourceKey.toString()
            << "\"");
}

/*
 * Creates a new Handler for a SharedResource, initializing the resource
 * if necessary.
 */
SharedResource::Handler::Handler(
        const juce::Identifier& resourceKey,
        const std::function<SharedResource*()> createResource) :
resourceKey(resourceKey)
{
    SharedResource* classResource = getClassResource();
    if(classResource == nullptr)
    {
        classResource = createResource();
        const juce::ScopedWriteLock addHandlerLock(getResourceLock());
        jassert(classResource->resourceHandlers[0] == nullptr);
        classResource->resourceHandlers.set(0, this);
    }
    else
    {
        const juce::ScopedWriteLock addHandlerLock(getResourceLock());
        classResource->resourceHandlers.add(this);
    }
}


/*
 * Removes a handler from the handler list, destroying the resource if
 * no handlers remain.
 */
SharedResource::Handler::~Handler()
{
    SharedResource* classResource = getClassResource();
    const juce::ReadWriteLock& resourceLock = getResourceLock();
    resourceLock.enterWrite();
    classResource->resourceHandlers.removeAllInstancesOf(this);
    if(classResource->resourceHandlers.isEmpty())
    {
        const juce::ScopedWriteLock removalLock(resourceMapLock);
        // Set map value to null before releasing lock
        SharedResource* toDelete = resourceMap[resourceKey]();
        resourceMap[resourceKey] = nullptr;
        // Release lock before deleting resource
        resourceLock.exitWrite();

        
        jassert(toDelete != nullptr);
        jassert(toDelete->resourceHandlers.isEmpty());
        delete toDelete;
        return;
    }
    resourceLock.exitWrite();
}

/*
 * Gets a reference to the SharedResource object shared by all
 * objects of this ResourceManager subclass.
 */
SharedResource*
SharedResource::Handler::getClassResource() const
{
    const juce::ScopedReadLock mapLock(resourceMapLock);
    return resourceMap[resourceKey].get();
}

/**
 * Gets a reference to the lock used to control access to the
 * shared resource.
 */
const juce::ReadWriteLock& SharedResource::Handler::getResourceLock() const
{
    return getClassResource()->resourceLock;;
}

/*
 * Packages an asynchronous action so that it will check if the 
 * SharedResource instance that created it stil valid, and if so,
 * ensure it remains valid while the action is executed.
 */
std::function<void()> SharedResource::buildAsyncFunction(
        SharedResource::LockType lockType,
        std::function<void()> action,
        std::function<void()> ifDestroyed)
{
    const juce::Identifier& resKey = resourceKey;
    return [this, lockType, resKey, action, ifDestroyed]()
    {
        std::unique_ptr<Handler> resourceProtector = nullptr;
        SharedResource* resource;
        {
            const juce::ScopedReadLock mapLock(resourceMapLock);
            resource = resourceMap[resourceKey].get();
            if(resource != nullptr && resource == this)
            {
                resourceProtector.reset(new Handler(resKey,
                        []()->SharedResource*{return nullptr;}));
            }
        }

        if(resourceProtector != nullptr)
        {
            if(lockType == SharedResource::LockType::write)
            {
                const juce::ScopedWriteLock writeLock(resource->resourceLock);
                action();
            }
            else if(lockType == SharedResource::LockType::read)
            {
                const juce::ScopedReadLock readLock(resource->resourceLock);
                action();
            }
        }
        else
        {
            DBG("SharedResource: Cancelled async function, resource "
                    << resKey.toString() << " was destroyed.");
            ifDestroyed();
        }
    };
}


/*
 * Runs an arbitrary function on each Handler object connected to the
 * SharedResource.
 */
void SharedResource::foreachHandler
(std::function<void(Handler*)> handlerAction)
{
    const juce::ScopedWriteLock handlerLock(resourceLock);
    const int handlerCount = resourceHandlers.size();
    for(int i = 0; i < handlerCount; i++)
    {
        //check for changes to the handler list during the loop
        jassert(resourceHandlers.size() == handlerCount);
        if(resourceHandlers[i] != nullptr)
        {
            handlerAction(resourceHandlers[i]);
            resourceHandlers[i]->resourceUpdate();
        }
    }
}

/*
 * Gets the unique key identifying the SharedResource object.
 */
const juce::Identifier& SharedResource::getResourceKey()
{
    return resourceKey;
}

