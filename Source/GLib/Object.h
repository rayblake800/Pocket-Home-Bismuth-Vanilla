#pragma once
#include <glib-object.h>
#include <map>
#include "JuceHeader.h"
#include "GLib/WeakRef.h"
#include "GLib/GLib.h"

/**
 * @file  GLib/Object.h
 * 
 * @brief A C++ interface and RAII container for GLib GObject data.
 * 
 *  GLib::Object holds a GObject*, providing methods for setting and getting 
 * object properties, and adding and removing object signal handlers. 
 * GLib::Object classes allow code to access GLib libraries through thread-safe,
 * RAII-managed C++ objects, reducing the amount of necessary C-style code.  
 * 
 *  GLib::Objects ensure that GObject pointers are referenced when acquired, and
 * unreferenced when removed. If the GObject data is null or otherwise invalid,
 * isNull() will return true, and all other methods will return default data or 
 * do nothing.
 * 
 *  GLib::Object subclasses should each handle a single type of GObject, 
 * providing methods to interact with that object. GLib::Objects should avoid 
 * holding any extra data, to ensure that all objects holding the same GObject 
 * data are completely interchangeable.
 */
class GLib::Object
{
public:
    /**
     * @brief  Gets this object's reference count.  Only use this for debugging.
     * 
     * @return  The stored object's reference count, or 0 if this object is
     *          null.
     */
    int getReferenceCount() const;
    
protected:
    /**
     * @brief  Creates a null Object, with no internal GObject.
     * 
     * @param objectType   Sets the type of GObject this Object may hold.
     */
    Object(const GType objectType);
    
    /**
     * @brief  Creates a new Object as a reference to existing object data.
     * 
     * @param toCopy      As long as this Object holds a compatible GObject, its
     *                    GObject* will be shared, and the GObject's reference 
     *                    count will be increased.
     * 
     * @param objectType  Sets the type of GObject this Object holds.
     */
    Object(const Object& toCopy, const GType objectType);
    
    /**
     * @brief  Creates an Object from GObject* data.
     * 
     * @param toAssign     GObject data to store in this Object.  If this 
     *                     GObject* has a floating reference, Object will 
     *                     sink the reference.  Otherwise, a new reference will
     *                     be added.
     * 
     * @param objectType   Sets the type of GObject this Object holds.
     */
    Object(GObject* toAssign, const GType objectType);

public:
    /**
     * @brief  Unreferences the Object's GObject* data on destruction.
     */
    virtual ~Object();
    
    /**
     * @brief  Checks if this object holds valid GObject data.
     * 
     * @return  Whether this Object's GObject* is null or otherwise invalid.
     */
    bool isNull() const;
    
    /**
     * @brief  Checks if this Object and another share the same GObject data.
     * 
     * @param rhs  Another Object instance.
     * 
     * @return     Whether both objects hold pointers to the same GObject data.
     */
    bool operator==(const Object& rhs) const;
    
    /**
     * @brief Checks if this Object holds a particular GObject pointer.
     * 
     * @param rhs  A pointer to GObject data, or nullptr.
     * 
     * @return     Whether rhs holds the same address as this Object.
     */
    bool operator==(GObject* rhs) const;
                
    /**
     * @brief  Checks if this Object and another don't share the same GObject 
     *         data.
     * 
     * @param rhs  Another Object instance.
     * 
     * @return     Whether both objects don't hold pointers to the same GObject 
     *             data.
     */
    bool operator!=(const Object& rhs) const;
    
    /**
     * @brief  Checks if this Object does not hold a particular GObject pointer.
     * 
     * @param rhs  A pointer to GObject data, or nullptr.
     * 
     * @return     Whether rhs does not hold the same address as this Object.
     */
    bool operator!=(GObject* rhs) const;
    
    /**
     * @brief  Sets this Object's data container to a new reference of another 
     *         Object's stored GObject.
     * 
     * @param rhs  Another Object instance. If rhs is a null object, this 
     *             object's data will be removed.
     */
    void operator=(const Object& rhs);
    
    /**
     * @brief  Sets this Object's stored GObject data.
     * 
     * @param rhs  A valid GObject to store, or nullptr to convert this 
     *             Object into a null object. If this value is floating, 
     *             the floating reference will be claimed by the Object. 
     *             Otherwise, the reference count will be increased.
     */
    void operator=(GObject* rhs);
    
protected:   
    /**
     * @brief  Gets a pointer to this object's data in a thread-safe manner.
     * 
     * @return  A pointer to the held GObject data. If this data is not null,
     *          the GObject's reference count will increase by one, and the
     *          caller is responsible for unreferencing the object once it is
     *          no longer needed. 
     */
    GObject* getGObject() const;
    
    /**
     * @brief  Assigns new GObject data to this Object. 
     *
     * Unless the new GObject to assign is already held by this Object, any 
     * references to the Object's previous GObject data will be removed.
     * 
     * @param toAssign  GObject data to store in this Object.  If this GObject* 
     *                  has a floating reference, the Object will sink the 
     *                  reference.  Otherwise, a new reference will be added.  
     * 
     *                  If the GType of this object is invalid, or the same 
     *                  GObject* is already held by this Object, the object's 
     *                  reference count will not be changed.
     */
    void setGObject(GObject* toAssign);
    
    /**
     * @brief  Assigns new GObject data to this Object.  
     *
     * Unless the new GObject to assign is already held by this Object, any 
     * references to the Object's previous GObject data will be removed.
     * 
     * @param toCopy  Any other Object. If toCopy holds the same data as this 
     *                Object, or its GType is not compatible with this object's 
     *                type, nothing will happen.
     */
    void setGObject(const Object& toCopy);
    
    /**
     * @brief  Allows Object subclasses to access GObject data within other 
     *         Objects. 
     *
     * Avoid using this for anything other than calling library functions that 
     * need GObject* parameter data.
     * 
     * @param source  Another Object.
     * 
     * @return        The GObject* data held by source, or nullptr if source is 
     *                null. Any valid GObject returned by this function will 
     *                have its reference count incremented, and the caller is 
     *                responsible for unreferencing it when it is no longer 
     *                needed.
     */
    GObject* getOtherGObject(const Object& source) const;

    /**
     * @brief  Removes this object's GObject data, clearing all associated 
     *         references.
     */
    void clearGObject();
    
    /**
     * @brief  Calls an arbitrary function from within the context assigned to 
     *         this Object. 
     *
     * GLib::Object subclasses will need to implement this to use an appropriate
     * context, probably through a GLib::ThreadResource.
     * 
     * @param call  A function to synchronously run on the GMainLoop that owns 
     *              this object's GMainContext.
     */
    virtual void callInMainContext(std::function<void()> call) const = 0;
    
    /**
     * @brief  Calls an arbitrary function from within the context assigned to 
     *         this Object, passing in GObject data as a parameter.  
     *
     * This makes use of the zero parameter callInMainContext function, so it 
     * does not need to be overridden to operate in a different context.
     * 
     * @param call            This function will synchronously run on the 
     *                        GMainLoop that owns this object's GMainContext.  
     *                        This object's GObject* data will be passed in as a
     *                        parameter. Unlike getGObject, the data provided 
     *                        through this parameter does not need to be 
     *                        unreferenced by the caller.
     * 
     * @param skipCallIfNull  If this value is true and this Object is null,
     *                        the call function parameter will not run.
     */
    void callInMainContext(std::function<void(GObject*)> call,
            bool skipCallIfNull = true) const;
    
    /**
     * @brief  Gets the GType assigned to this Object class.
     * 
     * @return  The GType of GObjects held by this Object class.
     */
    GType getType() const;
    
    /**
     * @brief  Checks if a GObject's type allows it to be held by this Object.
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return         Whether toCheck shares a type with this object, or is 
     *                 null. 
     */
    bool isValidType(GObject* toCheck) const;
    
    /**
     * @brief  Gets a pointer to one of the property values stored by this 
     *         Object.
     * 
     * @tparam T        The property type that's being requested.
     * 
     * @param property  A name identifying some property held by the GObject.
     * 
     * @return          A pointer to a copy of the parameter data, or nullptr if 
     *                  the parameter wasn't found.  If this value is a pointer 
     *                  type it must be freed with g_free() or g_object_unref(),
     *                  depending on its type.
     */
    template<typename T> T getProperty(const char* property) const
    {
        GObject* object = getGObject();
        T pValue;
        if(object != nullptr)
        {
            g_object_get(object, property, &pValue, nullptr);
            g_object_unref(object);
        }
        return pValue;
    }
    
    /**
     * @brief  Sets one of the property values stored by this object.
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
    /* Holds the GObject type used by this Object */
    const GType objectType;
    
    /* Holds a weak reference to the GObject stored in this Object. This 
       prevents attempts to access GObject data after it is deleted. */
    WeakRef objectRef;

    /* Only allow one thread to change the stored GObject at once. */
    juce::CriticalSection objectLock;
};
