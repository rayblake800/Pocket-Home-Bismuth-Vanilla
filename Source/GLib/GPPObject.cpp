#include "GLibSignalHandler.h"
#include "GPPObject.h"

Array<GPPObject::SignalHandler*, CriticalSection> GPPObject::signalHandlers;

/**
 * Create a void GPPObject, with no internal GObject.
 */
GPPObject::GPPObject() 
{ 
    objectRef = new GWeakRef;
    g_weak_ref_init(objectRef.get(), nullptr);
}

/*
 * Create a new GPPObject as a reference to existing object data.
 */
GPPObject::GPPObject(GPPObject& toCopy)
{
    objectRef = new GWeakRef;
    g_weak_ref_init(objectRef.get(), nullptr);
    copyData(toCopy);
}

/*
 * Create a GPPObject from GObject* data.
 */
GPPObject::GPPObject(GObject* toAssign)
{
    objectRef = new GWeakRef;
    g_weak_ref_init(objectRef.get(), nullptr);
    assignData(toAssign);
}

/*
 * When this GPPObject is destroyed, dereference its GObject data, and 
 * remove all signal handlers added through this GPPObject.
 */
GPPObject::~GPPObject()
{
    removeData();
}

/*
 * Checks if this object holds valid GObject data.
 */
bool GPPObject::isVoid() const
{
    return objectData.get() != nullptr;
}

/**
 * Adds the signal handler's address to the list of valid signal
 * handlers.
 */
GPPObject::SignalHandler::SignalHandler()
{
    GPPObject::signalHandlers.addIfNotAlreadyThere(this);
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
            source->removeSignalHandler(this);
        }
    }
}


/**
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
 * Add a signal handler to receive notifications when a specific object 
 * property changes.
 */
void GPPObject::addNotifySignalHandler(GPPObject::SignalHandler* signalHandler,
        const char* propertyName)
{
    callInMainContext([this, signalHandler, propertyName]()
    {
        GObject* object = getGObject();
        if(object != nullptr)
        {
            if(signalHandlers.contains(signalHandler))
            {
                String signal("notify::");
                signal += propertyName;
                gulong handlerID = g_signal_connect(object,
                        signal.toRawUTF8(), 
                        G_CALLBACK(notifyCallback),
                        signalHandler);
                if(handlerID > 0)
                {
                    registeredSignals[handlerID] = signalHandler;
                    signalHandler->sources.addIfNotAlreadyThere(this);
                }
                else
                {
                    DBG("GPPObject::addNotifySignalHandler: Received invalid "
                            << "ID when adding handler for signal notify::"
                            << propertyName);
                }
            }
            g_clear_object(&object);
        }
        else
        {
            DBG("GPPObject::addNotifySignalHandler: Tried to add signal "
                    << "notify::" << propertyName 
                    << "to void object.");
        }
    });
}

/*
 * Un-subscribe a signal handler from all of this object's signals
 */
void GPPObject::removeSignalHandler(SignalHandler* signalHandler)
{
    callInMainContext([this,signalHandler]()
    {
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
            }
            g_clear_object(&object);
        }
        else
        {
            DBG("GPPObject::removeSignalHandler: Tried to remove signal "
                    << "handler from void object.");
        }
    });
}

/**
 * Call an arbitrary function from within the context assigned to this
 * GObject.  This defaults to using the global default context.  Override
 * this if the object should operate on a different context and thread.
 */
void GPPObject::callInMainContext(std::function<void()> call)
{
    GLibSignalHandler globalDefault;
    globalDefault.gLibCall(call);
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
bool GPPObject::operator==(const GObject* rhs) const
{
    GObject* heldObject = objectData.get();
    return heldObject == rhs;
}

/*
 * Assigns new GObject data to this GPPObject.  Unless the new object
 * already holds the same data as this object, any references or signal
 * handlers associated with previous GObject data will be removed.
 */ 
void GPPObject::copyData(GPPObject& toCopy)
{   
    callInMainContext([this,&toCopy]()
    {
        if(g_type_is_a(getType(), toCopy.getType()))
        {
            GObject* copiedData = toCopy.getGObject();
            if(copiedData != nullptr && copiedData != objectData.get())
            {
                removeData();
                objectData.set(copiedData);
                g_weak_ref_set(objectRef.get(), copiedData);
                if(g_object_is_floating(copiedData))
                {
                    //Claim the floating reference, and remove the reference 
                    //added by getGObject()
                    g_object_ref_sink(copiedData);
                    g_object_unref(copiedData);
                }
                //otherwise, keep the reference added by getGObject.
            }
        }
        else
        {
            DBG("GPPObject::" << __func__ << ":Invalid assignment of type "
                    << g_type_name(toCopy.getType()) << " to type "
                    << g_type_name(getType()));
        }
    });
}

/*
 * Assigns new GObject data to this GPPObject.  Unless the new object
 * already holds the same data as this object, any references or signal
 * handlers associated with previous GObject data will be removed.
 */
void GPPObject::assignData(GObject* toAssign)
{
    callInMainContext([this,toAssign]
    {
        if(toAssign != nullptr && objectData.get() != toAssign)
        {
            removeData();
            objectData.set(toAssign);
            g_weak_ref_set(objectRef.get(), toAssign);
            g_object_ref_sink(toAssign);
        }
    });
}

/*
 * Gets a pointer to this object's data in a thread-safe manner.
 */
GObject* GPPObject::getGObject() const
{
    return G_OBJECT(g_weak_ref_get(objectRef.get()));
}

    
/*
 * Remove this object's GObject data, clearing all associated references 
 * and signal handlers.
 */
void GPPObject::removeData()
{
    callInMainContext([this]
    {
        g_weak_ref_set(objectRef.get(), nullptr);
        while(!registeredSignals.empty())
        {
            SignalHandler* toRemove = registeredSignals.begin()->second;
            removeSignalHandler(toRemove);
        }
        g_object_unref(objectData.get());
        objectData.set(nullptr);
    });
}
 
/*
 * Register a signal handler for any signal the GObject can send.
 */
void GPPObject::addSignalHandler(SignalHandler* handler,
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
                    << signalName << "to void object.");
        }
    });
}

/*
 * Check if a specific SignalHandler still exists.  Signal callback
 * functions should never dereference SignalHandler data without first
 * making sure this function returns true.
 */
bool GPPObject::isSignalHandlerValid(GPPObject::SignalHandler* handler)
{
    return handler != nullptr && signalHandlers.contains(handler);
}

/**
 * Use signal callback data to find the address of the GPPObject that
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
