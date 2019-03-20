#pragma once
/**
 * @file  GLib_Owned_Object.h
 * 
 * @brief  A C++ interface and RAII container for owned GLib GObject* data.
 */

#include "GLib_Object.h"
#include "GLib_WeakRef.h"
#include "JuceHeader.h"

namespace GLib { namespace Owned { class Object; } }

/**
 * @brief  Manages Owned GLib GObject* data. 
 *
 *  Owned GObject* data is GLib object data that solely belongs to GLib::Object 
 * instances. Owned GObject* data will only be deleted when every GLib::Object 
 * that holds it has been destroyed or reassigned.
 */
class GLib::Owned::Object : public GLib::Object
{
protected:
    /**
     * @brief  Creates a null Object, with no internal GObject.
     * 
     * @param objectType   Sets the type of GObject this Object may hold.
     */
    Object(const GType objectType);

    /**
     * @brief  Creates a new Object as a reference to existing object data,
     *         using the same GType.
     * 
     * @param toCopy  This object's GObject* will be shared, and the GObject's 
     *                reference count will be increased.
     */
    Object(const Object& toCopy);
    
    /**
     * @brief  Creates a new Object using data moved from a temporary Object.
     * 
     * @param toMove  This object's GObject* will be moved to the new Object.
     */
    Object(Object&& toMove);

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
     *                     sink the reference. Otherwise, a new reference will
     *                     be added.
     * 
     * @param objectType   Sets the type of GObject this Object holds.
     */
    Object(GObject* toAssign, const GType objectType);

public:
    /**
     * @brief Unreferences the Object's GObject* data on destruction.
     */
    virtual ~Object();
    
    /**
     * @brief  Sets this Object's data container to a new reference of another 
     *         Object's stored GObject.
     * 
     * @param rhs  Another Object instance. If rhs is a null object, this 
     *             object's data will be removed.
     *
     * @return     This Object instance.
     */
    Object& operator=(const Object& rhs);
    
    /**
     * @brief  Sets this Object's stored GObject data.
     * 
     * @param rhs  A valid GObject to store, or nullptr to convert this 
     *             Object into a null object. If this value is floating, 
     *             the floating reference will be claimed by the Object. 
     *             Otherwise, the reference count will be increased.
     *
     * @return     This Object instance.
     */
    Object& operator=(GObject* rhs);

    /**
     * @brief  Checks if the object is locally Owned.
     *
     * @return  true
     */
    virtual bool isOwned() const override { return true; }   

    /**
     * @brief  Gets a pointer to this object's data.
     * 
     * @return  A pointer to the held GObject data. If this data is not null,
     *          the GObject's reference count will increase by one, and the
     *          caller is responsible for unreferencing the object once it is
     *          no longer needed. 
     */
    virtual GObject* getGObject() const override;
    
protected:   
    /**
     * @brief  Assigns new GObject data to this Object. 
     *
     *  Unless the new GObject to assign is already held by this Object, any 
     * references to the Object's previous GObject data will be removed.
     * 
     * @param toAssign  GObject data to store in this Object.  If this GObject* 
     *                  has a floating reference, the Object will sink the 
     *                  reference.  Otherwise, a new reference will be added.  
     * 
     *                   If the GType of this object is invalid, or the same 
     *                  GObject* is already held by this Object, the object's 
     *                  reference count will not be changed.
     */
    virtual void setGObject(GObject* toAssign) override;
    
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
    virtual void setGObject(const GLib::Object& toCopy) override;

    /**
     * @brief  Removes this object's GObject data, clearing all associated 
     *         references.
     */
    virtual void clearGObject() override;
    
private:  
    /* Holds a weak reference to the GObject stored in this Object. This 
       prevents attempts to access GObject data after it is deleted. */
    WeakRef objectRef;

    JUCE_LEAK_DETECTOR(GLib::Owned::Object);
};
