#pragma once
#include <glib.h>
#include <map>
#include "JuceHeader.h"
#include "GLibSignalThread.h"

/**
 * @file GPPObject.h
 * 
 * @brief A C++ interface and RAII container for GLib GObject data.
 * 
 *  GPPObject holds a GObject*, providing methods for setting and getting object
 * properties, and adding and removing object signal handlers.  GPPObject
 * classes allow code to access GLib libraries through thread-safe C++ objects,
 * reducing the amount of necessary C-style code.  
 * 
 *  GPPObjects ensure that GObject pointers are referenced when acquired, and
 * unreferenced when removed.  If the GObject data is null or otherwise invalid,
 * isNull() will return true, and all other methods will return default data or 
 * do nothing.
 * 
 *  GPPObject subclasses should each handle a single type of GObject, providing
 * methods to interact with that object.  GPPObjects should avoid holding any
 * extra data, to ensure that all objects holding the same GObject data are
 * completely interchangeable.
 */
template <class GPPObjectType>
class GPPObject
{
protected:
    /**
     * Create a null GPPObject, with no internal GObject.
     * 
     * @param objectType   Sets the type of GObject this GPPObject holds.
     */
    GPPObject(GType objectType) : objectType(objectType) { }
    
    /**
     * Create a new GPPObject as a reference to existing object data.
     * 
     * @tparam GPPOtherType  The GPPObject type of the GPPObject being copied.
     *                       GPPOtherType's GType must either equal objectType,
     *                       or be a sub-type of objectType.
     * 
     * @param toCopy         As long as this GPPObject holds a compatible 
     *                       GObject, its GObject* will be shared, and the 
     *                       GObject's reference count will be increased.
     * 
     * @param objectType     Sets the type of GObject this GPPObject holds.
     */
    template <class GPPOtherType>
    GPPObject(GPPOtherType& toCopy, GType objectType) 
    : objectType(objectType) 
    {
        setGObject<GPPOtherType>(&toCopy);
    }
    
    /**
     * Create a GPPObject from GObject* data.
     * 
     * @param toAssign     GObject data to store in this GPPObject.  If this 
     *                     GObject* has a floating reference, GPPObject will 
     *                     sink the reference.  Otherwise, a new reference will
     *                     be added.
     * 
     * @param objectType   Sets the type of GObject this GPPObject holds.
     */
    GPPObject(GObject* toAssign, GType objectType) : objectType(objectType)
    {
        setGObject(toAssign);
    }

public:
    /**
     * When this GPPObject is destroyed, unreference its GObject data.
     */
    virtual ~GPPObject()
    {
        clearGObject(); 
    }
    
    /**
     * Checks if this object holds valid GObject data.
     * 
     * @return true iff this object's GObject* is null or otherwise invalid
     */
    bool isNull() const
    {
        GObject* object = getGObject();
        bool result = (object == nullptr);
        g_clear_object(&object);
        return result;
    }

    /**
     * Classes handling this object's signals must implement this interface.
     * This ensures that a SignalHandler<GPPObjectType> can only have signals
     * added to it by a GPPObjectType object.
     */
    class SignalHandler
    {
    private:
        friend GPPObjectType;
            
        /**
         * Subscribe the signal handler to a single signal.
         * 
         * @param source      A GOject signal source.  The signal handler will
         *                    acquire a reference to the source's GObject, if it 
         *                    doesn't have one already.
         * 
         * @param signalName  The signal that handler should receive.
         * 
         * @param callback    A signal-specific callback function.  This should
         *                    be a static void function that takes parameters 
         *                    specific to the subscribed signal.  A pointer to 
         *                    this signal handler will be passed to the callback
         *                    as data.
         */
        virtual void connectSignal(GObject* source,
                const char* signalName, GCallback callback) = 0;   
        
        /**
         * Subscribe a signal handler to receive notifications when a specific 
         * object property changes.
         * 
         * @param source      A GOject signal source.  The signal handler will
         *                    acquire a reference to the source's GObject, if it 
         *                    doesn't have one already.
         * 
         * @param propertyName  A valid property of this object.
         */
        void connectNotifySignal(GObject* source, const char* propertyName)
        {   
            if(propertyName != nullptr)
            {
                juce::String signal("notify::");
                signal += propertyName;
                connectSignal(source, signal.toRawUTF8(), 
                        G_CALLBACK(notifyCallback));
            }
        }
        
        /**
         * Callback to handle property change signals.
         * 
         * @param objectData     The GObject signal source.
         * 
         * @param pSpec          Holds information about the updated parameter.
         * 
         * @param signalHandler  The object subscribed to the GObject's signals
         */
        static void notifyCallback(GObject* objectData, GParamSpec* pSpec,
                SignalHandler* signalHandler);
        
        /**
         * Callback function for handling property change notification signals.
         * Signal handlers should override this to handle all specific property 
         * change notifications that they support.
         * 
         * @param source    Holds the GObject that emitted the signal.
         * 
         * @param property  The name of the object property that changed.
         */
        virtual void propertyChanged(GPPObjectType& source,
                juce::String property) { }
    };
    
    
    /**
     * Adds a signal handler to all of this object's tracked signals.
     *
     * @param signalHandler  A signal handler object that will listen to this
     *                       object's signals.
     */
    virtual void connectSignalHandler(SignalHandler* signalHandler) = 0;
    
    /**
     * Checks if this GPPObject and another share the same GObject data.
     * 
     * @param rhs  Another GPPObject instance.
     * 
     * @return true iff both objects hold pointers to the same GObject data.
     */
    bool operator==(const GPPObject<GPPObjectType>& rhs)
    {
        GObject* thisObject = getGObject();
        GObject* rhsObject = rhs.getGObject();
        bool result = (thisObject == rhsObject);
        g_clear_object(&thisObject);
        g_clear_object(&rhsObject);
        return result;
    }
    
    /**
     * Checks if this GPPObject holds a particular GObject pointer.
     * 
     * @rhs  A pointer to GObject data, or nullptr.
     * 
     * @return true iff rhs holds the same address as this GPPObject.
     */
    bool operator==(GObject* rhs)
    {
        GObject* heldObject = getGObject();
        bool result = (heldObject == rhs);
        g_clear_object(&heldObject);
        return result;
    }
                
    /**
     * Checks if this GPPObject and another don't share the same GObject data.
     * 
     * @param rhs  Another GPPObject instance.
     * 
     * @return true iff both objects don't hold pointers to the same GObject 
     *         data.
     */
    bool operator!=(const GPPObject<GPPObjectType>& rhs)
    {
        return !(*this == rhs);
    }
    
    /**
     * Checks if this GPPObject does not hold a particular GObject pointer.
     * 
     * @rhs  A pointer to GObject data, or nullptr.
     * 
     * @return true iff rhs does not hold the same address as this GPPObject.
     */
    bool operator!=(GObject* rhs) const
    {
        return !(*this == rhs);
    }
    
    /**
     * Sets this GPPObject's stored data to a new reference of another 
     * GPPObject's stored GObject.
     * 
     * @param rhs  Another GPPObject instance.  If rhs is a null object, this 
     *             object's data will be removed.
     */
    void operator=(const GPPObject<GPPObjectType>& rhs)
    {
        setGObject(rhs);
    }
    
    /**
     * Sets this GPPObject's stored GObject data.
     * 
     * @param rhs  A valid GObject to store, or nullptr to convert this 
     *             GPPObject into a null object. If this value is floating, 
     *             the floating reference will be claimed by the GPPObject. 
     *             Otherwise, the reference count will be increased.
     */
    void operator=(GObject* rhs)
    {
        setGObject(rhs);
    }
    
protected:   
    /**
     * Gets a pointer to this object's data in a thread-safe manner.
     * 
     * @return  a pointer to the held GObject data.  If this data is not null,
     *          the GObject's reference count will increase by one, and the
     *          caller is responsible for unreferencing the object once it is
     *          no longer needed. 
     */
    GObject* getGObject()
    {
        return G_OBJECT(g_weak_ref_get(&objectRef));
    }
    
    /**
     * Assigns new GObject data to this GPPObject.  Unless the new object
     * already holds the same data as this object, any references to the
     * previous GObject data will be removed.
     * 
     * @param toAssign     GObject data to store in this GPPObject.  If this 
     *                     GObject* has a floating reference, GPPObject will 
     *                     sink the reference.  Otherwise, a new reference will
     *                     be added.  
     * 
     *                     If the GType of this object is invalid, or the same
     *                     GObject* is already held by this GPPObject, the
     *                     object's reference count will not be changed.
     */
    void setGObject(GObject* toAssign)
    {
        if(!isValidType(toAssign))
        {
            return;
        }
        if(toAssign == nullptr)
        {
            clearGObject();
            return;
        }
        GObject* oldData = getGObject();
        if(toAssign != oldData)
        {
            clearGObject();
            g_object_ref_sink(toAssign);
            g_weak_ref_set(&objectRef, toAssign);      
        }
        g_clear_object(&oldData);
    }
    
    /**
     * Assigns new GObject data to this GPPObject.  Unless the new object
     * already holds the same data as this object, any references to the
     * previous GObject data will be removed.
     * 
     * $tparam GPPOtherType  The class of the GPPObject being copied. This
     *                       class must have either the same GType as this
     *                       GPPObject, or its GType must be a sub-type of
     *                       this object's type.  
     * 
     * @param toCopy         Any other GPPObject.  If toCopy holds the same data
     *                       as this GPPObject, or its GType is not compatible
     *                       with this object's type, nothing will happen.
     */
    template <class GPPOtherType>
    void setGObject(GPPObject<GPPOtherType>& toCopy)
    {
        juce::ScopedLock lockData(objectLock);
        GObject* newData = toCopy.getGObject();
        if(isValidType(newData))
        {
            setGObject(newData);
        }
        g_clear_object(&newData);
    }
    
    /**
     * Allows GPPObject subclasses to access GObject data within other 
     * GPPObjects. Avoid using this for anything other than calling library 
     * functions that need GObject* parameter data.
     * 
     * @tparam OtherObjectType  Any other type of GPPObject.
     * 
     * @param source            Another GPPObject.
     * 
     * @return  The GObject* data held by source, or nullptr if source is null.
     *          Any valid GObject returned by this function will have its
     *          reference count incremented, and the caller is responsible for
     *          unreferencing it when it is no longer needed.
     */
    template<class OtherObjectType>
    GObject* getOtherGObject(GPPObject<OtherObjectType>& source) const
    {
        return source.getGObject();
    }

    /**
     * Remove this object's GObject data, clearing all associated references.
     */
    void clearGObject()
    {
        juce::ScopedLock lockData(objectLock);
        GObject* object = getGObject();
        if(object != nullptr)
        {
            g_weak_ref_set(&objectRef, nullptr);
            g_object_unref(object);
        }
        g_clear_object(&object);
    }
    
    /**
     * Call an arbitrary function from within the context assigned to this
     * GObject.  This defaults to using the global default context.  Override
     * this if the object should operate on a different context and thread.
     * 
     * @param call  This function will synchronously run on the GMainLoop
     *              that owns this object's GMainContext.
     */
    virtual void callInMainContext(std::function<void()> call) const
    {
        GLibSignalThread globalDefault;
        globalDefault.gLibCall(call);
    }
    
    /**
     * Call an arbitrary function from within the context assigned to this
     * GObject, passing in GObject data as a parameter.  This makes use of the
     * zero parameter callInMainContext function, so it does not need to be
     * overridden to operate in a different context.
     * 
     * @param call            This function will synchronously run on the 
     *                        GMainLoop that owns this object's GMainContext.  
     *                        This object's GObject* data will be passed in as a
     *                        parameter. Unlike getGObject, the data provided 
     *                        through this parameter does not need to be 
     *                        unreferenced by the caller.
     * 
     * @param skipCallIfNull  If this value is true and this GPPObject is null,
     *                        the call function parameter will not run.
     */
    void callInMainContext(std::function<void(GObject*)> call,
            bool skipCallIfNull = true) const
    {
        GObject* data = getGObject();
        callInMainContext([&data, &call, skipCallIfNull]()
        {
            if(data != nullptr || !skipCallIfNull)
            {
                call(data);
            }
            g_clear_object(&data);
        });
    }
    
    /**
     * Get the GType assigned to this GPPObject class.
     * 
     * @return The GType of GObjects held by this GPPObject class.
     */
    GType getType() const
    {
        return objectType;
    }
    
    /**
     * Check if a GObject's type allows it to be held by this object.
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return  True iff toCheck shares a type with this object, or is null. 
     */
    bool isValidType(GObject* toCheck) const
    {
        return G_TYPE_CHECK_INSTANCE_TYPE(toCheck, objectType);
    }
    
    /**
     * Gets a pointer to one of the property values stored by this object.
     * 
     * @tparam T        The property type that's being requested.
     * 
     * @param property  A name identifying some property held by the GObject.
     * 
     * @return  A pointer to a copy of the parameter data, or nullptr if the
     *          parameter wasn't found.  If this value isn't null, it must be
     *          freed with g_free() or g_object_unref(), depending on its type.
     */
    template<typename T> T* getProperty(const char* property)
    {
        GObject* object = getGObject();
        if(object != nullptr)
        {
            T* pValue = nullptr;
            g_object_get(object, property, &pValue, nullptr);
            g_object_unref(object);
            return pValue;
        }
        return nullptr;
    }
    
    /**
     * Sets one of the property values stored by this object.
     * 
     * @tparam T        The type of the property that's being set.
     * 
     * @param property  A name identifying some property held by the GObject.
     * 
     * @param value     A valid property value to copy into the GObject.
     */
    template<typename T> void setProperty(const char* property, T value)
    {
        GObject* object = getGObject();
        if(object != nullptr)
        {
            g_object_set(object, property, value, nullptr);
            g_object_unref(object);
        }
    } 
    
private:  
    /**
     * Holds the GObject type used by this GPPObject
     */
    const GType objectType;
    
    /**
     * Holds a weak reference to the GObject stored in this GPPObject.  This is
     * needed for safely accessing and referencing the stored GObject.
     */
    GWeakRef objectRef;
    
    /**
     * Only allow one thread to change the stored GObject at once.
     */
    juce::CriticalSection objectLock;
};

/**
 * Callback to handle property change signals.
 */
template<class GPPObjectType>
void GPPObject<GPPObjectType>::SignalHandler::notifyCallback
(GObject* objectData, GParamSpec* pSpec, 
        GPPObject<GPPObjectType>::SignalHandler* signalHandler)
{
    GPPObjectType tempObjectHolder(objectData);
    juce::String property(pSpec->name);
    signalHandler->propertyChanged(tempObjectHolder, property);
}