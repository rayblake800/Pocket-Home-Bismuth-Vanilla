#include "GLib/Object.h"

/*
 * Gets this object's reference count.  Only use this for debugging.
 */
int GLib::Object::getReferenceCount() const
{
    GObject* object = getGObject();
    if(object == NULL)
    {
        return 0;
    }
    int refCount = object->ref_count - 1;
    g_clear_object(&object);
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
    GObject* object = getGObject();
    bool result = (object == nullptr);
    g_clear_object(&object);
    return result;
}

/*
 * Checks if this Object and another share the same GObject data.
 */
bool GLib::Object::operator==(const Object& rhs) const
{
    GObject* thisObject = getGObject();
    GObject* rhsObject = rhs.getGObject();
    bool result = (thisObject == rhsObject);
    g_clear_object(&thisObject);
    g_clear_object(&rhsObject);
    return result;
}

/*
 * Checks if this Object holds a particular GObject pointer.
 */
bool GLib::Object::operator==(GObject* rhs) const
{
    GObject* heldObject = getGObject();
    bool result = (heldObject == rhs);
    g_clear_object(&heldObject);
    return result;
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
    int refCount = rhs.getReferenceCount();
    setGObject(rhs);
    int newCount = rhs.getReferenceCount();
    jassert(newCount = refCount +1 || newCount == 0);
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
    if(toAssign == nullptr)
    {
        clearGObject();
        return;
    }
    if(!isValidType(toAssign))
    {
        return;
    }
    GObject* oldData = getGObject();
    if(toAssign != oldData)
    {
        clearGObject();
        if(g_object_is_floating(toAssign))
        {
            g_object_ref_sink(toAssign);
        }
        objectRef = toAssign;
    }
    g_clear_object(&oldData);
}

/*
 * Assigns new GObject data to this Object.
 */
void GLib::Object::setGObject(const Object& toCopy)
{
    juce::ScopedLock lockData(objectLock);
    GObject* copiedObject = toCopy.getGObject();
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
    GObject* object = getGObject();
    objectRef = NULL;
    if(object != nullptr)
    {
        g_object_unref(object);
    }
    g_clear_object(&object);
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
