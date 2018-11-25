#include "GLib/Object.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/*
 * Gets this object's reference count.  Only use this for debugging.
 */
int GLib::Object::getReferenceCount() const
{
    ObjectPtr<> object(getGObject());
    if(object == NULL)
    {
        return 0;
    }
    int refCount = ((GObject*) object)->ref_count - 1;
    return refCount;
}

/*
 * Creates a null Object, with no internal GObject.
 */
GLib::Object::Object(const GType objectType) : objectType(objectType) { }

/*
 * Creates a new Object as a reference to existing object data.
 */
GLib::Object::Object(const Object& toCopy, const GType objectType) 
: objectType(objectType) 
{
    setGObject(toCopy);
}

/*
 * Creates an Object from GObject* data.
 */
GLib::Object::Object(GObject* toAssign, const GType objectType) :
objectType(objectType)
{
    setGObject(toAssign);
}

/*
 * Unreferences the Object's GObject* data on destruction.
 */
GLib::Object::~Object()
{
    clearGObject(); 
}

/*
 * Checks if this object holds valid GObject data.
 */
bool GLib::Object::isNull() const
{
    ObjectPtr<> object(getGObject());
    return object == nullptr;
}

/*
 * Checks if this Object and another share the same GObject data.
 */
bool GLib::Object::operator==(const Object& rhs) const
{
    ObjectPtr<> thisObject(getGObject());
    ObjectPtr<> rhsObject(rhs.getGObject());
    return thisObject == rhsObject;
}

/*
 * Checks if this Object holds a particular GObject pointer.
 */
bool GLib::Object::operator==(GObject* rhs) const
{
    ObjectPtr<> heldObject(getGObject());
    return heldObject == rhs;
}

/*
 * Checks if this Object and another don't share the same GObject data.
 */
bool GLib::Object::operator!=(const Object& rhs) const
{
    return !(*this == rhs);
}

/*
 * Checks if this Object does not hold a particular GObject pointer.
 */
bool GLib::Object::operator!=(GObject* rhs) const 
{
    return !(*this == rhs);
}

/*
 * Sets this Object's data container to a new reference of another Object's 
 * stored GObject.
 */
void GLib::Object::operator=(const Object& rhs)
{
    if(*this == rhs)
    {
        return;
    }
    setGObject(rhs);
}

/*
 * Sets this Object's stored GObject data.
 */
void GLib::Object::operator=(GObject* rhs)
{
    setGObject(rhs);
}
 
/*
 * Gets a pointer to this object's data in a thread-safe manner.
 */
GObject* GLib::Object::getGObject() const
{
    return objectRef.getObject();
}

/*
 * Assigns new GObject data to this Object.
 */
void GLib::Object::setGObject(GObject* toAssign)
{ 
    juce::ScopedLock lockData(objectLock);
    if(toAssign == nullptr)
    {
        clearGObject();
        return;
    }
    if(!isValidType(toAssign))
    {
        return;
    }
    ObjectPtr<> oldData(getGObject());
    if(toAssign != oldData)
    {
        clearGObject();
        if(g_object_is_floating(toAssign))
        {
            g_object_ref_sink(toAssign);
        }
        objectRef = toAssign;
    }
}

/*
 * Assigns new GObject data to this Object.
 */
void GLib::Object::setGObject(const Object& toCopy)
{
    ObjectPtr<> copiedObject(toCopy.getGObject());
    setGObject(copiedObject);
}

/*
 * Allows Object subclasses to access GObject data within other Objects.
 */
GObject* GLib::Object::getOtherGObject(const Object& source) const 
{
    return source.getGObject();
}

/*
 * Removes this object's GObject data, clearing all associated references.
 */
void GLib::Object::clearGObject()
{
    juce::ScopedLock lockData(objectLock);
    ObjectPtr<> object(getGObject());
    objectRef = NULL;
    if(object != nullptr)
    {
        g_object_unref(object);
    }
}

/*
 * Gets the GType assigned to this Object class.
 */
GType GLib::Object::getType() const
{
    return objectType;
}

/*
 * Checks if a GObject's type allows it to be held by this object.
 */
bool GLib::Object::isValidType(GObject* toCheck) const
{
    return G_TYPE_CHECK_INSTANCE_TYPE(toCheck, objectType);
}
