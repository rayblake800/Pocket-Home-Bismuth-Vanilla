#include "Utils.h"
#include "GLibSignalThread.h"
#include "GPPObject.h"

juce::Array<GPPObject::SignalHandler*, juce::CriticalSection> 
GPPObject::signalHandlers;

/*
 * Creates a null GPPObject, with no internal GObject.
 */
GPPObject::GPPObject() 
{ 
    objectRef = new GWeakRef;
    objectData.set(nullptr);
    g_weak_ref_init(objectRef.get(), nullptr);
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
}

/*
 * When this GPPObject is destroyed, unreference its GObject data.
 */
GPPObject::~GPPObject()
{
    removeData(); 
}

/*
 * Checks if this object holds valid GObject data.
 */
bool GPPObject::isNull() const
{
    return objectData.get() == nullptr;
}


/*
 * Un-subscribe a signal handler from all of this object's signals
 */
void GPPObject::removeSignalHandler(SignalHandler* signalHandler)
{
    using namespace juce;
    const juce::ScopedLock objectLock(signalLock);
    //ADDR_LOG(this, "removing signal handler ", signalHandler);
    //ADDR_LOG(signalHandler, " removing from signal source ", this);
    if(isSignalHandlerValid(signalHandler))
    {
        signalHandler->sources.removeAllInstancesOf(this);
    }
    else
    {
        DBG("GPPObject::" << __func__ 
                << ": removing invalid signal handler " 
                << String((unsigned long)signalHandler));
    }
    GObject* object = getGObject();
    if(object != nullptr && signalHandler != nullptr)
    {
        Array<gulong> signalIDs = registeredSignals[signalHandler];
        for(const gulong& signalID : signalIDs)
        {
            g_signal_handler_disconnect(object, signalID);
            
            //ADDR_LOG(this, String("signal ID ") +String(signalID) 
            //        + String(" removed, handler was "), signalHandler);
            //ADDR_LOG(signalHandler, String("signal ID ") +String(signalID) 
            //        + String(" removed, source was "), object);
            //ADDR_LOG(object, String("signal ID ") +String(signalID) 
            //        + String(" removed, handler was "), signalHandler);
            
        }
        registeredSignals.erase(signalHandler);
        g_clear_object(&object);
    }
    else
    {
        //ADDR_LOG(this, "GPPObject is null, but tried removing ",
        //        signalHandler);
        DBG("GPPObject::removeSignalHandler: Null object and/or SignalHandler,"
                << "Object=" << String((unsigned long) object)
                << ", SignalHandler=" << String((unsigned long) signalHandler));
    }
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
    
    //ADDR_LOG(this, "Copying data from GPPObject ", &toCopy);
    //ADDR_LOG(&toCopy, "Sharing data with GPPObject ", this);
    if(g_type_is_a(getType(), toCopy.getType()) && *this != toCopy)
    {
        GObject* newData = toCopy.getGObject();
        setData(newData, false, transferSignalHandlers);
    }
//    else
//    {
//        ADDR_LOG(this, "Couldn't copy data from GPPObject ", &toCopy);
//        ADDR_LOG(&toCopy, "Couldn't share data with with GPPObject ", this);
//        String types = "Source type:";
//        types += g_type_name(toCopy.getType());
//        types += ", Dest. type:";
//        types += g_type_name(getType());
//        ADDR_LOG(this,types);
//        ADDR_LOG(&toCopy,types);
//    }
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
    const juce::ScopedLock objectLock(signalLock);
    if(!isNull())
    {
        while(!registeredSignals.empty())
        {
            auto signalIter = registeredSignals.begin();
            SignalHandler* toClear = signalIter->first;
            if(isSignalHandlerValid(toClear))
            {
                toClear->sources.removeAllInstancesOf(this);
            }
            registeredSignals.erase(signalIter);
        }
        GObject* object = objectData.get();
        //ADDR_LOG(object, "Removing from GPPObject ", this);
        g_weak_ref_set(objectRef.get(), nullptr);
        if(object != nullptr)
        {
            g_object_unref(object);
            objectData.set(nullptr);
            //ADDR_LOG(this, "Removed GObject ",object);
        }
    }
}

/*
 * Call an arbitrary function from within the context assigned to this
 * GObject.  This defaults to using the global default context.  Override
 * this if the object should operate on a different context and thread.
 */
void GPPObject::callInMainContext(std::function<void()> call) const
{
    GLibSignalThread globalDefault;
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
        const juce::ScopedLock objectLock(signalLock);
        GObject* object = getGObject();
        if(object != nullptr)
        {
            if(isSignalHandlerValid(handler) && callback != nullptr)
            {
                gulong handlerID = g_signal_connect(object,
                        signalName, 
                        callback,
                        handler);
                if(handlerID > 0)
                {
//                    ADDR_LOG(this, "Connected signal handler ", handler);
//                    ADDR_LOG(handler, "Connected to GPPObject ", this);
//                    ADDR_LOG(handler, "and attached to GObject ", object);
//                    ADDR_LOG(object, "Added signal handler ", handler);
//                    ADDR_LOG(object, "through GPPObject ", this);

                    registeredSignals[handler].add(handlerID);
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
    using namespace juce;
    jassert(propertyName != nullptr);
    if(isSignalHandlerValid(signalHandler))
    {
        String signal("notify::");
        signal += propertyName;
        const char * cSignal = signal.toRawUTF8();
        connectSignalHandler(signalHandler, cSignal,
                G_CALLBACK(notifyCallback));
    }
    else
    {
        DBG("GPPObject::" << __func__
                << ": Signal handler no longer exists, will not be added to "
                << "notify::" << propertyName);
    }
}

/*
 * Callback to handle property change signals.
 */
void GPPObject::notifyCallback(GObject* objectData, GParamSpec* pSpec,
        GPPObject::SignalHandler* signalHandler)
{
    using namespace juce;
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
juce::Array<GPPObject::SignalHandler*> GPPObject::getSignalHandlers()
{
    using namespace juce;
    Array<SignalHandler*> handlers;
    for(int i = 0; i < signalHandlers.size(); i++)
    {
        SignalHandler* handler = signalHandlers[i];
        if(!registeredSignals[handler].isEmpty())
        {
            handlers.addIfNotAlreadyThere(handler);
        }
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
    using namespace juce;
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
//            if(oldData != nullptr)
//            {
//                ADDR_LOG(oldData, "Removed from GPPObject ", this);
//                ADDR_LOG(this, "Removed GObject ", oldData);
//            }
//            ADDR_LOG(data, "Added to GPPObject ", this);
//            ADDR_LOG(this, "Set GObject ", data);
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
//        else
//        {
//            ADDR_LOG(this, "Failed to get GObject ",data);
//        }
    });
}
