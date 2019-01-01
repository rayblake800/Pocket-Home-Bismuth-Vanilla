#pragma once

/**
 * @file  GLib/Borrowed/Borrowed_Object.h
 *
 * @brief  A C++ interface for borrowed GLib GObject* data.
 */
#include "GLib/Object.h"
#include "GLib/Borrowed/SharedContainer.h"

namespace GLib { namespace Borrowed { class Object; } }
namespace GLib { namespace Borrowed { class ObjectLender; } }

/**
 *  GLib::Borrowed::Object manages Borrowed GLib GObject* data.  Borrowed 
 * GObject* data is GLib object data that belongs to an external library.
 * The lifecycle of borrowed GObject* data is controlled by its library owner.
 * Borrowed GObject* data may be deleted by its library owner at any time,
 * regardless of its reference count.
 */
class GLib::Borrowed::Object : public GLib::Object
{
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
     *                    GObject* will be shared with the new object.
     * 
     * @param objectType  Sets the type of GObject this Object holds.
     */
    Object(const Object& toCopy, const GType objectType);

public:
    virtual ~Object();

    /**
     * @brief  Sets this Object's data container to a new reference of another 
     *         Object's stored GObject*.
     * 
     * @param rhs  Another Object instance. If rhs is a null object, this 
     *             object's data will be removed.
     */
    void operator=(const Object& rhs);

    /**
     * @brief  Checks if the object is locally Owned.
     *
     * @return  false
     */
    virtual bool isOwned() const override { return false; }   

protected:
    /**
     * @brief  Gets a pointer to this object's data.
     * 
     * @return  A pointer to the held GObject data.
     */
    virtual GObject* getGObject() const override;

    /**
     * @brief  Assigns new GObject* data to this Object. 
     * 
     * @param toAssign  Another object used to replace the Borrowed::Object
     *                  instance's GObject data.
     */
    virtual void setGObject(const GLib::Object& toCopy) override;

    /**
     * @brief  Clears all held GObject* data from this Object instance.
     */
    virtual void clearGObject() override;

private:
    /* Only the ObjectLender may add new valid GObject* values and invalidate
       existing values. */
    friend class ObjectLender;

    /**
     * @brief  Creates an Object from GObject* data.
     * 
     * @param toAssign     GObject data to store in this Object.
     * 
     * @param objectType   Sets the type of GObject this Object holds.
     */
    Object(GObject* toAssign, const GType objectType);

    /**
     * @brief  Removes this object's stored GObject* data from every borrowed
     *         Object that contains that data.
     *
     *  The ObjectLender that initially wrapped the GObject* in a 
     * Borrowed::Object is responsible for calling this method when the GObject*
     * data is destroyed by the library that owns it.
     */
    void invalidateObject();

    /* Holds the borrowed GObject* data. */
    SharedContainer::Ptr objectContainer;
};
