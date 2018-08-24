#pragma once
#include <glib-object.h>
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
class GPPObject
{
protected:
    /**
     * Create a null GPPObject, with no internal GObject.
     * 
     * @param objectType   Sets the type of GObject this GPPObject holds.
     */
    GPPObject(const GType objectType);
    
    /**
     * Create a new GPPObject as a reference to existing object data.
     * 
     * @param toCopy         As long as this GPPObject holds a compatible 
     *                       GObject, its GObject* will be shared, and the 
     *                       GObject's reference count will be increased.
     * 
     * @param objectType     Sets the type of GObject this GPPObject holds.
     */
    GPPObject(const GPPObject& toCopy, const GType objectType);
    
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
    GPPObject(GObject* toAssign, const GType objectType);

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
    
    /**
     * Checks if this GPPObject and another share the same GObject data.
     * 
     * @param rhs  Another GPPObject instance.
     * 
     * @return true iff both objects hold pointers to the same GObject data.
     */
    bool operator==(const GPPObject& rhs) const;
    
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
    bool operator!=(const GPPObject& rhs) const;
    
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
    void operator=(const GPPObject& rhs);
    
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
     *          caller is responsible for unreferencing the object once it is
     *          no longer needed. 
     */
    GObject* getGObject() const;
    
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
    void setGObject(GObject* toAssign);
    
    /**
     * Assigns new GObject data to this GPPObject.  Unless the new object
     * already holds the same data as this object, any references to the
     * previous GObject data will be removed.
     * 
     * @param toCopy         Any other GPPObject.  If toCopy holds the same data
     *                       as this GPPObject, or its GType is not compatible
     *                       with this object's type, nothing will happen.
     */
    void setGObject(const GPPObject& toCopy);
    
    /**
     * Allows GPPObject subclasses to access GObject data within other 
     * GPPObjects. Avoid using this for anything other than calling library 
     * functions that need GObject* parameter data.
     * 
     * @param source            Another GPPObject.
     * 
     * @return  The GObject* data held by source, or nullptr if source is null.
     *          Any valid GObject returned by this function will have its
     *          reference count incremented, and the caller is responsible for
     *          unreferencing it when it is no longer needed.
     */
    GObject* getOtherGObject(const GPPObject& source) const;

    /**
     * Remove this object's GObject data, clearing all associated references.
     */
    void clearGObject();
    
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
     * Get the GType assigned to this GPPObject class.
     * 
     * @return The GType of GObjects held by this GPPObject class.
     */
    GType getType() const;
    
    /**
     * Check if a GObject's type allows it to be held by this object.
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return  True iff toCheck shares a type with this object, or is null. 
     */
    bool isValidType(GObject* toCheck) const;
    
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
    template<typename T> T* getProperty(const char* property) const
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
