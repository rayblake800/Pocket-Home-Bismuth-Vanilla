#include "Utils.h"
#include "GLibSignalHandler.h"
#include "GPPObject.h"

Array<GPPObject::SignalHandler*, CriticalSection> GPPObject::signalHandlers;

/*
 * Creates a null GPPObject, with no internal GObject.
 */
GPPObject::GPPObject() 
{ 
    objectRef = new GWeakRef;
    objectData.set(nullptr);
    g_weak_ref_init(objectRef.get(), nullptr);
    //DBG("Created: " << addressID(this) << ":" << addressID(objectData.get()));
}

/*
 * Creates a new GPPObject as a reference to existing object data.
 */
GPPObject::GPPObject(const GPPObject& toCopy)
{
    objectRef = new GWeakRef;
    objectData.set(nullptr);
    g_weak_ref_init(objectRef.get(), nullptr);
    setGObject(toCopy);
    DBG("Created: " << addressID(this) << ":" << addressID(objectData.get())
            << "(from " << addressID(&toCopy) << ")");
}

/*
 * Creates a GPPObject from GObject* data.
 */
GPPObject::GPPObject(GObject* toAssign)
{
    objectRef = new GWeakRef;
    objectData.set(nullptr);
    g_weak_ref_init(objectRef.get(), nullptr);
    setGObject(toAssign);
    DBG("Created: " << addressID(this) << ":" << addressID(objectData.get()));
}

/*
 * When this GPPObject is destroyed, unreference its GObject data, and 
 * remove all signal handlers added through this GPPObject.
 */
GPPObject::~GPPObject()
{
    removeData();
    
    DBG("Destroyed: " << addressID(this) << ":" << addressID(objectData.get()));
}

/*
 * Checks if this object holds valid GObject data.
 */
bool GPPObject::isNull() const
{
    return objectData.get() == nullptr;
}

/*
 * Adds the signal handler's address to the list of valid signal
 * handlers.
 */
GPPObject::SignalHandler::SignalHandler()
{
    //DBG("Created handler " << addressID(this));
    GPPObject::signalHandlers.addIfNotAlreadyThere(this);
}


        
/*
 * Copies an existing signal handler, subscribing to all of that
 * handler's tracked signals.
 */
GPPObject::SignalHandler::SignalHandler(const SignalHandler& rhs)
{   
    GPPObject::signalHandlers.addIfNotAlreadyThere(this);
    for(GPPObject* signalSource : sources)
    {
        signalSource->addSignalHandler(this);
    }
}
        
/*
 * Removes all of its signal handling callback functions from within the
 * signal context thread, and remove this signal handler's address from
 * the list of valid signal handlers.
 */
GPPObject::SignalHandler::~SignalHandler()
{
    GPPObject::signalHandlers.removeAllInstancesOf(this);
    while(!sources.isEmpty())
    {
        GPPObject* source = sources[0];
        if(source != nullptr)
        {
            if(source->isNull())
            {
                DBG("GPPObject::SignalHandler::" << __func__ 
                        << ": Error: Found null signal source");
                sources.removeAllInstancesOf(source);
            }
            else
            {
                source->removeSignalHandler(this);
            }
        }
    }
    
    //DBG("Destroyed handler " << addressID(this));
}

/*
 * Callback function for handling property change notification signals.
 * Subclasses should override this to handle all specific property 
 * change notifications that they support.
 */
void GPPObject::SignalHandler::propertyChanged
(GPPObject* source, String property)
{
    DBG("GPPObject::SignalHandler::" << __func__ << ": Unexpected notification"
            << "for property " << property); 
}
    
/*
 * Adds a signal handler to all of this object's tracked signals.
 */
void GPPObject::addSignalHandler(SignalHandler* signalHandler)
{
    DBG("GPPObject::" << __func__ << ": Error: GPPObject subclass with GType " 
            << g_type_name(getType())
            << " did not override addListener()");
}

/*
 * Un-subscribe a signal handler from all of this object's signals
 */
void GPPObject::removeSignalHandler(SignalHandler* signalHandler)
{
    callInMainContext([this,signalHandler]()
    {
        signalHandler->sources.removeAllInstancesOf(this);
        GObject* object = getGObject();
        if(object != nullptr)
        {
            Array<gulong> signalIDs;
            for(auto it = registeredSignals.begin(); 
                it != registeredSignals.end(); it++)
            {
                if(it->second == signalHandler)
                {
                    signalIDs.add(it->first);
                }
            }
            for(const gulong& signalID : signalIDs)
            {
                g_signal_handler_disconnect(object, signalID);
                registeredSignals.erase(signalID);
                DBG(addressID(this) << ":" << addressID(objectData.get())
                        << "  removed " << signalID << ":" << addressID(signalHandler));
            }
            g_clear_object(&object);
        }
        else
        {
            DBG(addressID(this) << ":" << addressID(objectData.get())
                    << "  tried removing ?:" << addressID(signalHandler));
            //DBG("GPPObject::removeSignalHandler: Tried to remove signal "
            //        << "handler from null object.");
        }
    });
}

/*
 * Checks if this GPPObject and another share the same GObject data.
 */
bool GPPObject::operator==(const GPPObject& rhs) const
{
    GObject* thisObject = objectData.get();
    GObject* othrObject = rhs.objectData.get();
    return thisObject == othrObject;
}

/*
 * Checks if this GPPObject holds a particular GObject pointer.
 */
bool GPPObject::operator==(GObject* rhs) const
{
    GObject* heldObject = objectData.get();
    return heldObject == rhs;
}

/*
 * Checks if this GPPObject and another don't share the same GObject data.
 */
bool GPPObject::operator!=(const GPPObject& rhs) const
{
    GObject* thisObject = objectData.get();
    GObject* othrObject = rhs.objectData.get();
    return thisObject != othrObject;
}

/*
 * Checks if this GPPObject does not hold a particular GObject pointer.
 */
bool GPPObject::operator!=(GObject* rhs) const
{
    GObject* heldObject = objectData.get();
    return heldObject != rhs;
}

/*
 * Sets this GPPObject's stored data to a new reference of another 
 * GPPObject's stored GObject.
 */
void GPPObject::operator=(const GPPObject& rhs)
{
    setGObject(rhs);
}

/*
 * Sets this GPPObject's stored GObject data.
 */
void GPPObject::operator=(GObject* rhs)
{
    setGObject(rhs);
}

/*
 * Gets a pointer to this object's data in a thread-safe manner.  Don't use
 * this outside of GPPObject implementations.  This method is only public
 * to allow different GPPObject subclasses to share GObjects for library
 * function calls.
 */
GObject* GPPObject::getGObject() const
{
    GWeakRef* weakRef = objectRef.get();
    if(weakRef == nullptr)
    {
        return nullptr;
    }
    return G_OBJECT(g_weak_ref_get(weakRef));
}

/*
 * Assigns new GObject data to this GPPObject.  Unless the new object
 * already holds the same data as this object, any references or signal
 * handlers associated with previous GObject data will be removed.
 */
void GPPObject::setGObject(GObject* toAssign, bool transferSignalHandlers)
{
    setData(toAssign, true, transferSignalHandlers);
}

/*
 * Assigns new GObject data to this GPPObject.  Unless the new object
 * already holds the same data as this object, any references or signal
 * handlers associated with previous GObject data will be removed from the old
 * GObject.
 */ 
void GPPObject::setGObject(const GPPObject& toCopy, bool transferSignalHandlers)
{   
    DBG(addressID(this) << " = " << addressID(&toCopy));
    if(g_type_is_a(getType(), toCopy.getType()) && *this != toCopy)
    {
        GObject* newData = toCopy.getGObject();
        setData(newData, false, transferSignalHandlers);
    }
}
    
/**
 * Allows GPPObject subclasses to access GObject data within other 
 * GPPObjects. Avoid using this for anything other than calling library 
 * functions that need GObject* parameter data.
 */
GObject* GPPObject::getOtherGObject(const GPPObject& source) const
{
    return source.getGObject();
}


/*
 * Remove this object's GObject data, clearing all associated references 
 * and signal handlers.
 */
void GPPObject::removeData()
{
    callInMainContext([this]
    {
        if(!isNull())
        {
            while(!registeredSignals.empty())
            {
                SignalHandler* toRemove = registeredSignals.begin()->second;
                removeSignalHandler(toRemove);
            }
            GObject* object = objectData.get();
            g_weak_ref_set(objectRef.get(), nullptr);
            if(object != nullptr)
            {
                g_object_unref(object);
                objectData.set(nullptr);
                
                DBG(addressID(this) << ":" << addressID(object)
                        << "  -> " << addressID(this) << ":0");
            }
        }
    });
}

/*
 * Call an arbitrary function from within the context assigned to this
 * GObject.  This defaults to using the global default context.  Override
 * this if the object should operate on a different context and thread.
 */
void GPPObject::callInMainContext(std::function<void()> call) const
{
    GLibSignalHandler globalDefault;
    globalDefault.gLibCall(call);
}

/*
 * Call an arbitrary function from within the context assigned to this
 * GObject, passing in GObject data as a parameter.  This makes use of the
 * zero parameter callInMainContext function, so it does not need to be
 * overridden to operate in a different context.
 */
void GPPObject::callInMainContext(std::function<void(GObject*)> call,
        bool skipCallIfNull) const
{
    GObject* data = getGObject();
    callInMainContext([&data, &call, skipCallIfNull]()
    {
        if(data != nullptr)
        {
            call(data);
            g_clear_object(&data);
        }
        else if(!skipCallIfNull)
        {
            call(nullptr);
        }
    });
}
 
/*
 * Register a signal handler for any signal the GObject can send.
 */
void GPPObject::connectSignalHandler(SignalHandler* handler,
        const char* signalName, GCallback callback)
{
    callInMainContext([this, handler, signalName, callback]()
    {
        GObject* object = getGObject();
        if(object != nullptr)
        {
            if(signalHandlers.contains(handler) && callback != nullptr)
            {
                gulong handlerID = g_signal_connect(object,
                        signalName, 
                        callback,
                        handler);
                if(handlerID > 0)
                {

                    DBG(addressID(this) << ":" << addressID(objectData.get())
                            << "  added " << handlerID << ":" << addressID(handler));
                    registeredSignals[handlerID] = handler;
                    handler->sources.addIfNotAlreadyThere(this);
                }
                else
                {
                    DBG("GPPObject::addSignalHandler: Received invalid ID when"
                            << "adding handler for signal " << signalName);
                }
            }
            g_clear_object(&object);
        }
        else
        {
            DBG("GPPObject::addSignalHandler: Tried to add signal "
                    << signalName << " to null object.");
        }
    });
}

/*
 * Checks if a specific SignalHandler still exists.  Signal callback
 * functions should never dereference SignalHandler data without first
 * making sure this function returns true.
 */
bool GPPObject::isSignalHandlerValid(GPPObject::SignalHandler* handler)
{
    return handler != nullptr && signalHandlers.contains(handler);
}

/**
 * Uses signal callback data to find the address of the GPPObject that
 * contains the signal source.
 */
GPPObject* GPPObject::findObjectWrapper(GObject* objectData,
        GPPObject::SignalHandler* signalHandler)
{
    GPPObject* objectWrapper = nullptr;
    if(isSignalHandlerValid(signalHandler))
    {
        for(GPPObject* obj : signalHandler->sources)
        {
            if(*obj == objectData)
            {
                objectWrapper = obj;
                break;
            }
        }
    }
    else
    {
        DBG("GPPObject::" << __func__ << ": Couldn't find valid GPPObject!");
    }
    return objectWrapper; 
}

/*
 * Adds a signal handler to receive notifications when a specific object 
 * property changes.
 */
void GPPObject::addNotifySignalHandler(GPPObject::SignalHandler* signalHandler,
        const char* propertyName)
{
    String signal("notify::");
    signal += propertyName;
    connectSignalHandler(signalHandler, signal.toRawUTF8(),
            G_CALLBACK(notifyCallback));
}

/*
 * Callback to handle property change signals.
 */
void GPPObject::notifyCallback(GObject* objectData, GParamSpec* pSpec,
        GPPObject::SignalHandler* signalHandler)
{
    const char* signalName = pSpec->name;
    if(isSignalHandlerValid(signalHandler))
    {
        GPPObject* objectWrapper = findObjectWrapper(objectData,
                signalHandler);
        if(objectWrapper != nullptr)
        {
            signalHandler->propertyChanged(objectWrapper, String(signalName)
                    .fromLastOccurrenceOf(String("notify::"), false, false));
        }
        else
        {
            DBG("GPPObject::" << __func__ << ": No callback for signal "
                    << signalName << "!");
        }
    }
    else
    {
        DBG("GPPObject::" << __func__ << ": Invalid signal handler called for "
                << signalName << "!");
    }
    
}

/*
 * Gets all signal handlers registered with this object's GObject data.
 */
Array<GPPObject::SignalHandler*> GPPObject::getSignalHandlers()
{
    Array<SignalHandler*> handlers;
    for(auto it = registeredSignals.begin(); 
        it != registeredSignals.end(); it++)
    {
        handlers.addIfNotAlreadyThere(it->second);
    }
    return handlers;
}

/*
 * Sets this object's GObject data to a new value.  Unless the object data 
 * given equals the object's old data, any signal handlers attached to the
 * old GObject data will be removed.
 */
void GPPObject::setData(GObject* data, bool refNeeded, bool moveSignalHandlers)
{
    callInMainContext([this, data, refNeeded, moveSignalHandlers]()
    {
        if(data != nullptr && isValidType(data) && objectData.get() != data)
        {
            Array<SignalHandler*> toTransfer;
            if(moveSignalHandlers)
            {
                toTransfer = getSignalHandlers();
            }
            GObject* oldData = objectData.get();
            removeData();
            DBG(addressID(this) << ":" << addressID(oldData)
                    << "  -> " << addressID(this) << ":" << addressID(data));
            objectData.set(data);
            
            g_weak_ref_set(objectRef.get(), data);
            if(refNeeded || g_object_is_floating(data))
            {
                g_object_ref_sink(data);
            }
            if(!toTransfer.isEmpty())
            {
                for(SignalHandler* handler : toTransfer)
                {
                    addSignalHandler(handler);
                }
            }
        }
        else if(data == nullptr)
        {
            removeData();
        }
    });
}