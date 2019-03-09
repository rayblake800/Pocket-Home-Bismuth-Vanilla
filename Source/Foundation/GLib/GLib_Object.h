#pragma once
/**
 * @file  GLib_Object.h
 * 
 * @brief  A C++ interface and RAII container for GLib GObject data.
 */

#include "GLib_ObjectPtr.h"
#include "JuceHeader.h"

namespace GLib { class Object; }

/**
 * @brief  Holds a GObject*, providing methods for accessing object data,
 *         and setting and getting object properties.
 * 
 *  GLib::Object subclasses should each handle a single type of GObject, 
 * providing methods to interact with that object type. 
 */
class GLib::Object
{
protected:
    /**
     * @brief  Creates a null Object, with no internal GObject.
     * 
     * @param objectType   Sets the type of GObject this Object may hold.
     */
    Object(const GType objectType);

public:
    virtual ~Object() { }

    /**
     * @brief  Checks if this object is an Owned object that manages its own
     *         life cycle, or a Borrowed object that has its life cycle managed
     *         by a GLib library.
     *
     * @return  Whether this object is Owned.
     */
    virtual bool isOwned() const = 0;
    
    /**
     * @brief  Gets a pointer to this object's data in a thread-safe manner.
     * 
     * @return  A pointer to the held GObject data.
     */
    virtual GObject* getGObject() const = 0;

    /**
     * @brief  Checks if this object holds valid GObject data.
     * 
     * @return  Whether this Object's GObject* is null or otherwise invalid.
     */
    virtual bool isNull() const;

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
     * @brief  Gets this object's reference count.  Only use this for debugging.
     * 
     * @return  The stored object's reference count, or 0 if this object is
     *          null.
     */
    int getReferenceCount() const;
    
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
    
protected:   
    /**
     * @brief  Assigns new GObject data to this Object. 
     *
     *  Unless the new GObject to assign is already held by this Object, any 
     * references to the Object's previous GObject data will be removed.
     * 
     * @param toAssign  GObject data to store in this Object.
     */
    virtual void setGObject(GObject* toAssign) = 0;
    
    /**
     * @brief  Assigns new GObject data to this Object.  
     *
     *  Unless the new GObject to assign is already held by this Object, any 
     * references to the Object's previous GObject data will be removed.
     * 
     * @param toCopy  Any other Object. If toCopy holds the same data as this 
     *                Object, or its GType is not compatible with this object's 
     *                type, nothing will happen.
     */
    virtual void setGObject(const Object& toCopy) = 0;
    
    /**
     * @brief  Removes this object's GObject data.
     */
    virtual void clearGObject() = 0;
    
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
        ObjectPtr objectPtr(*this);
        T propertyValue;
        if(objectPtr != nullptr)
        {
            g_object_get(objectPtr, property, &propertyValue, nullptr);
        }
        return propertyValue;
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
        ObjectPtr objectPtr(*this);
        if(objectPtr != nullptr)
        {
            g_object_set(objectPtr, property, value, nullptr);
        }
    } 
    
private:  
    /* Holds the GObject type used by this Object */
    const GType objectType;

    JUCE_LEAK_DETECTOR(GLib::Object);
};
