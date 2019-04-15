#include "GLib_Owned_Object.h"
using OwnedObject = GLib::Owned::Object;


// Creates a null Object, with no internal GObject.
OwnedObject::Object(const GType objectType) : GLib::Object(objectType) { }


// Creates a new Object as a reference to existing object data, using the same
// GType.
OwnedObject::Object(const Object& toCopy)
: GLib::Object(toCopy.getType())
{
    setGObject(toCopy);
}


// Creates a new Object using data moved from a temporary Object.
OwnedObject::Object(Object&& toMove) :
GLib::Object(toMove.getType())
{
    objectRef = std::move(toMove.objectRef);
}


// Creates a new Object as a reference to existing object data.
OwnedObject::Object(const Object& toCopy, const GType objectType)
: GLib::Object(objectType)
{
    setGObject(toCopy);
}


// Creates an Object from GObject* data.
OwnedObject::Object(GObject* toAssign, const GType objectType) :
GLib::Object(objectType)
{
    setGObject(toAssign);
}


// Unreferences the Object's GObject* data on destruction.
OwnedObject::~Object()
{
    clearGObject();
}


// Sets this Object's data container to a new reference of another Object's
// stored GObject.
OwnedObject& OwnedObject::operator= (const Object& rhs)
{
    if (*this != rhs)
    {
        setGObject(rhs);
    }
    return *this;
}


// Sets this Object's stored GObject data.
OwnedObject& OwnedObject::operator= (GObject* rhs)
{
    setGObject(rhs);
    return *this;
}


// Gets a pointer to this object's data in a thread-safe manner.
GObject* OwnedObject::getGObject() const
{
    return objectRef.getObject();
}


// Assigns new GObject data to this Object.
void OwnedObject::setGObject(GObject* toAssign)
{
    if (toAssign == nullptr)
    {
        clearGObject();
        return;
    }
    if (!isValidType(toAssign))
    {
        return;
    }
    ObjectPtr oldData(*this);
    if (toAssign != oldData)
    {
        clearGObject();
        if (g_object_is_floating(toAssign))
        {
            g_object_ref_sink(toAssign);
        }
        objectRef = toAssign;
    }
}


// Assigns new GObject data to this Object.
void OwnedObject::setGObject(const GLib::Object& toCopy)
{
    setGObject(toCopy.getGObject());
}


// Removes this object's GObject data, clearing all associated references.
void OwnedObject::clearGObject()
{
    ObjectPtr object(*this);
    objectRef = NULL;
    if (object != nullptr)
    {
        g_object_unref(object);
    }
}
