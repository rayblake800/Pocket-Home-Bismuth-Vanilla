#include "GLib_Object.h"

// Creates a null Object, with no internal GObject.
GLib::Object::Object(const GType objectType) : objectType(objectType) { }


// Checks if this object holds valid GObject data.
bool GLib::Object::isNull() const
{
    ObjectPtr objectPtr(*this);
    return objectPtr == nullptr;
}


// Checks if this Object and another share the same GObject data.
bool GLib::Object::operator== (const Object& rhs) const
{
    return ObjectPtr(*this) == ObjectPtr(rhs);
}


// Checks if this Object holds a particular GObject pointer.
bool GLib::Object::operator== (GObject* rhs) const
{
    ObjectPtr heldObject(*this);
    return heldObject == rhs;
}


// Checks if this Object and another don't share the same GObject data.
bool GLib::Object::operator!= (const Object& rhs) const
{
    return ! (*this == rhs);
}


// Checks if this Object does not hold a particular GObject pointer.
bool GLib::Object::operator!= (GObject* rhs) const
{
    return ! (*this == rhs);
}


// Gets this object's reference count. Only use this for debugging.
int GLib::Object::getReferenceCount() const
{
    ObjectPtr objectPtr(*this);
    if (objectPtr == nullptr)
    {
        return 0;
    }
    int refCount = ( (GObject*) objectPtr)->ref_count;
    return (isOwned() ? refCount - 1 : refCount);
}


// Gets the GType assigned to this Object class.
GType GLib::Object::getType() const
{
    return objectType;
}


// Checks if a GObject's type allows it to be held by this Object.
bool GLib::Object::isValidType(GObject* toCheck) const
{
    return G_TYPE_CHECK_INSTANCE_TYPE(toCheck, objectType);
}
