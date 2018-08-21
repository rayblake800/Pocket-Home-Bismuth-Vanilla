#pragma once
#include <glib.h>
#include <map>
#include "JuceHeader.h"

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
     */
    GPPObject();
    
    /**
     * Create a new GPPObject as a reference to existing object data.
     * 
     *@param toCopy  This object's GObject* will be shared, and its reference
     *               count will be increased.
     */
    GPPObject(const GPPObject<GPPObjectType>& toCopy)
    {
        objectRef = new GWeakRef;
        objectData.set(nullptr);
        g_weak_ref_init(objectRef.get(), nullptr);
        setGObject(toCopy);
    }
    
    /**
     * Create a GPPObject from GObject* data.
     * 
     * @param toAssign  GObject data to store in this GPPObject.  If this 
     *                  GObject* has a floating reference, GPPObject will sink
     *                  the reference.  Otherwise, a new reference will be
     *                  added.
     */
    GPPObject(GObject* toAssign);

public:
    /**
     * When this GPPObject is destroyed, unreference its GObject data.
     */
    virtual ~GPPObject();
    
    /**
     * Checks if this object holds valid GObject data.
     * 
     * @return true iff this object's GObject* is null or otherwise invalid
     */
    bool isNull() const;

    template<class HandlerObjectType> class SignalHandler;
    
    /**
     * Adds a signal handler to all of this object's tracked signals.
     *
     * @param signalHandler  A signal handler object that will listen to this
     *                       object's signals.
     */
    virtual void addSignalHandler(SignalHandler<GPPObjectType>& signalHandler)
            = 0;
      
    /**
     * Un-subscribe a signal handler from all of this object's signals
     * 
     * @param signalHandler  A signal handler object that should no longer
     *                       receive signals from this GPPObject.
     */
    virtual void removeSignalHandler
    (SignalHandler<GPPObjectType>& signalHandler);
           
    /**
     * Checks if this GPPObject and another share the same GObject data.
     * 
     * @param rhs  Another GPPObject instance.
     * 
     * @return true iff both objects hold pointers to the same GObject data.
     */
    bool operator==(const GPPObject<GPPObjectType>& rhs) const;
    
    /**
     * Checks if this GPPObject holds a particular GObject pointer.
     * 
     * @rhs  A pointer to GObject data, or nullptr.
     * 
     * @return true iff rhs holds the same address as this GPPObject.
     */
    bool operator==(GObject* rhs) const;
                
    /**
     * Checks if this GPPObject and another don't share the same GObject data.
     * 
     * @param rhs  Another GPPObject instance.
     * 
     * @return true iff both objects don't hold pointers to the same GObject 
     *         data.
     */
    bool operator!=(const GPPObject<GPPObjectType>& rhs) const;
    
    /**
     * Checks if this GPPObject does not hold a particular GObject pointer.
     * 
     * @rhs  A pointer to GObject data, or nullptr.
     * 
     * @return true iff rhs does not hold the same address as this GPPObject.
     */
    bool operator!=(GObject* rhs) const;
    
    /**
     * Sets this GPPObject's stored data to a new reference of another 
     * GPPObject's stored GObject.
     * 
     * @param rhs  Another GPPObject instance.  If rhs is a null object, this 
     *             object's data will be removed.
     */
    void operator=(const GPPObject<GPPObjectType>& rhs);
    
    /**
     * Sets this GPPObject's stored GObject data.
     * 
     * @param rhs  A valid GObject to store, or nullptr to convert this 
     *             GPPObject into a null object. If this value is floating, 
     *             the floating reference will be claimed by the GPPObject. 
     *             Otherwise, the reference count will be increased.
     */
    void operator=(GObject* rhs);
    
protected:
    /**
     * Gets a pointer to this object's data in a thread-safe manner.
     * 
     * @return  a pointer to the held GObject data.  If this data is not null,
     *          the GObject's reference count will increase by one, and the
     *          caller is responsible for un-referencing the object once it is
     *          no longer needed. 
     */
    GObject* getGObject() const;
    
    /**
     * Assigns new GObject data to this GPPObject.  Unless the new object
     * already holds the same data as this object, any references or signal
     * handlers associated with previous GObject data will be removed from the
     * old GObject.
     * 
     * @param toAssign                 This should point to either a new, valid 
     *                                 GObject, or be set to nullptr. 
     *                                 Otherwise, this assignment will do 
     *                                 nothing.
     * 
     * @param transferSignalHandlers   If toAssign is not null and this
     *                                 parameter is true, any signal handlers
     *                                 attached to the old GObject data will
     *                                 be re-assigned to the new GObject data.
     */
    void setGObject(GObject* toAssign, 
            bool transferSignalHandlers = true);
    
    /**
     * Assigns new GObject data to this GPPObject.  Unless the new object
     * already holds the same data as this object, any references or signal
     * handlers associated with previous GObject data will be removed from the
     * old GObject.
     * 
     * @param toCopy                   Any other GPPObject with a compatible
     *                                 GType.  If toCopy holds the same data as 
     *                                 this GPPObject, nothing will happen.
     * 
     * @param transferSignalHandlers   If toCopy is not null and this
     *                                 parameter is true, any signal handlers
     *                                 attached to the old GObject data will
     *                                 be re-assigned to the new GObject data.
     */
    void setGObject(const GPPObject<GPPObjectType>& toCopy,
            bool transferSignalHandlers = true);
    
    /**
     * Allows GPPObject subclasses to access GObject data within other 
     * GPPObjects. Avoid using this for anything other than calling library 
     * functions that need GObject* parameter data.
     * 
     * @param source  Another GPPObject.
     * 
     * @return  the GObject* data held by source, or nullptr if source is null.
     *          Any valid GObject returned by this function will have its
     *          reference count incremented, and the caller is responsible for
     *          unreferencing it when it is no longer needed.
     */
    GObject* getOtherGObject(const GPPObject<GPPObjectType>& source) const;
    
    /**
     * Remove this object's GObject data, clearing all associated references 
     * and signal handlers.
     */
    void removeData();
    
    /**
     * Call an arbitrary function from within the context assigned to this
     * GObject.  This defaults to using the global default context.  Override
     * this if the object should operate on a different context and thread.
     * 
     * @param call  This function will synchronously run on the GMainLoop
     *              that owns this object's GMainContext.
     */
    virtual void callInMainContext(std::function<void()> call) const;
    
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
            bool skipCallIfNull = true) const;
    
    /**
     * Get the GType assigned to this object.  This must be implemented by 
     * GPPObject subclasses to return the type of the GObject contained by
     * the subclass.  
     */
    virtual GType getType() const = 0;
    
    /**
     * Check if a GObject's type allows it to be held by this object.  This
     * must be implemented by GPPObject subclasses to check the data type
     * with the relevant GObject type checking macro.
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return  true iff toCheck shares a type with this object, or is null. 
     */
    virtual bool isValidType(GObject* toCheck) const = 0;
    
    /**
     * Gets a pointer to one of the property values stored by this object.
     * 
     * @tparam T        The property type that's being requested.
     * 
     * @param property  A name identifying some property held by the GObject.
     * 
     * @return  a pointer to a copy of the parameter data, or nullptr if the
     *          parameter wasn't found.  If this value isn't null, it must be
     *          freed with g_free() or g_object_unref(), depending on its type.
     */
    template<typename T> T* getProperty(const char* property)
    {
        gpointer object = getGObject();
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
        gpointer object = getGObject();
        if(object != nullptr)
        {
            g_object_set(object, property, value, nullptr);
            g_object_unref(object);
        }
    }
       
    /**
     * Subscribe a signal handler to any signal the GObject can send.
     * 
     * @param handler     A signal handler that can provide a valid GCallback 
     *                    for the chosen signal.
     * 
     * @param signalName  The signal that handler should receive.
     * 
     * @param callback    A signal-specific callback function.  This should be a
     *                    static void function that takes parameters specific
     *                    to the subscribed signal.  A pointer to the signal
     *                    handler will be passed to the callback as data.
     */
    void connectSignalHandler(SignalHandler<GPPObjectType>* handler,
            const char* signalName, GCallback callback);
    
    /**
     * Add a signal handler to receive notifications when a specific object 
     * property changes.  
     * 
     * Signal handler callback function template:
     * 
     * static void handleNotify(SignalHandler* self, GPPObject* signalSource,
     *         String changedProperty);
     * 
     * @param signalHandler  A signalHandler that can provide a callback
     *                       function for the specific notification signal.
     * 
     * @param propertyName  A valid property of this object.
     */
    void addNotifySignalHandler(SignalHandler* signalHandler,
            const char* propertyName);

    
    /**
     * Callback to handle property change signals.
     * 
     * @param objectData    The GObject sending the signal.
     * 
     * @param pSpec         Specifications for the updated object property.
     * 
     * @param signalHandler The SignalHandler object being notified of the
     *                      property change.
     */
    static void notifyCallback(GObject* objectData, GParamSpec* pSpec,
            SignalHandler* signalHandler);
    
private:
    
    /**
     * Set this object's GObject data to a new value.
     * 
     * @param data                A GObject of a type that is compatible with 
     *                            this GPPObject. 
     *
     * @param refNeeded           Indicates if the GPPObject needs to increment 
     *                            the GObject data reference count.  This should
     *                            only be set to false if an extra reference was
     *                            added prior to calling setData.
     */
    void setData(GObject* data, bool refNeeded = true);
    
    
    juce::ScopedPointer<GWeakRef> objectRef = nullptr;
    
    
    juce::Atomic<GObject*> objectData;
};
