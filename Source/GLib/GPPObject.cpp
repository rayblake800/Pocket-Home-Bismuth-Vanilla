#include "GLibSignalThread.h"
#include "GPPObject.h"

/*
 * Gets this object's reference count.  Only use this for debugging.
 * 
 */
int GPPObject::getReferenceCount() const
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
 * Create a null GPPObject, with no internal GObject.
 */
GPPObject::GPPObject(const GType objectType) : objectType(objectType) 
{
}

/*
 * Create a new GPPObject as a reference to existing object data.
 */
GPPObject::GPPObject(const GPPObject& toCopy, const GType objectType) 
: objectType(objectType) 
{
    setGObject(toCopy);
}

/*
 * Create a GPPObject from GObject* data.
 */
GPPObject::GPPObject(GObject* toAssign, const GType objectType) :
objectType(objectType)
{
    setGObject(toAssign);
}

/*
 * When this GPPObject is destroyed, unreference its GObject data.
 */
GPPObject::~GPPObject()
{
    clearGObject(); 
}

/*
 * Checks if this object holds valid GObject data.
 */
bool GPPObject::isNull() const
{
    GObject* object = getGObject();
    bool result = (object == nullptr);
    g_clear_object(&object);
    return result;
}

/*
 * Checks if this GPPObject and another share the same GObject data.
 */
bool GPPObject::operator==(const GPPObject& rhs) const
{
    GObject* thisObject = getGObject();
    GObject* rhsObject = rhs.getGObject();
    bool result = (thisObject == rhsObject);
    g_clear_object(&thisObject);
    g_clear_object(&rhsObject);
    return result;
}

/*
 * Checks if this GPPObject holds a particular GObject pointer.
 */
bool GPPObject::operator==(GObject* rhs) const
{
    GObject* heldObject = getGObject();
    bool result = (heldObject == rhs);
    g_clear_object(&heldObject);
    return result;
}

/*
 * Checks if this GPPObject and another don't share the same GObject data.
 */
bool GPPObject::operator!=(const GPPObject& rhs) const
{
    return !(*this == rhs);
}

/*
 * Checks if this GPPObject does not hold a particular GObject pointer.
 */
bool GPPObject::operator!=(GObject* rhs) const 
{
    return !(*this == rhs);
}

/*
 * Sets this GPPObject's stored data to a new reference of another 
 * GPPObject's stored GObject.
 */
void GPPObject::operator=(const GPPObject& rhs)
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
 * Sets this GPPObject's stored GObject data.
 */
void GPPObject::operator=(GObject* rhs)
{
    setGObject(rhs);
}
 
/*
 * Gets a pointer to this object's data in a thread-safe manner.
 */
GObject* GPPObject::getGObject() const
{
    return objectRef.getObject();
}

/*
 * Assigns new GObject data to this GPPObject.  Unless the new object
 * already holds the same data as this object, any references to the
 * previous GObject data will be removed.
 */
void GPPObject::setGObject(GObject* toAssign)
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
 * Assigns new GObject data to this GPPObject.  Unless the new object
 * already holds the same data as this object, any references to the
 * previous GObject data will be removed.
 */
void GPPObject::setGObject(const GPPObject& toCopy)
{
    juce::ScopedLock lockData(objectLock);
    GObject* copiedObject = toCopy.getGObject();
    setGObject(copiedObject);
}

/*
 * Allows GPPObject subclasses to access GObject data within other 
 * GPPObjects. Avoid using this for anything other than calling library 
 * functions that need GObject* parameter data.
 */
GObject* GPPObject::getOtherGObject(const GPPObject& source) const 
{
    return source.getGObject();
}

/*
 * Remove this object's GObject data, clearing all associated references.
 */
void GPPObject::clearGObject()
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
 * Call an arbitrary function from within the context assigned to this
 * GObject.  This defaults to using the global default context.  Override
 * this if the object should operate on a different context and thread.
 */
void GPPObject::callInMainContext(std::function<void()> call) const 
{
    GLibSignalThread globalDefault;
    globalDefault.gLibCall(call);
}

/*
 * Call an arbitrary function from within the context assigned to this
 * GObject, passing in GObject data as a parameter.  This makes use of the
 * zero parameter callInMainContext function, so it does not need to be
 * overridden to operate in a different context.
 */
void GPPObject::callInMainContext(std::function<void(GObject*)> call,
        bool skipCallIfNull) const 
{
    GObject* data = getGObject();
    callInMainContext([&data, &call, skipCallIfNull]()
    {
        if(data != nullptr || !skipCallIfNull)
        {
            call(data);
        }
        g_clear_object(&data);
    });
}

/*
 * Get the GType assigned to this GPPObject class.
 */
GType GPPObject::getType() const
{
    return objectType;
}

/*
 * Check if a GObject's type allows it to be held by this object.
 */
bool GPPObject::isValidType(GObject* toCheck) const
{
    return G_TYPE_CHECK_INSTANCE_TYPE(toCheck, objectType);
}
