#include "GPPWeakRef.h"

GPPWeakRef::GPPWeakRef()
{
    initRef(nullptr);
}

/*
 * Initialize the weak reference with another GPPWeakRef's data.
 */
GPPWeakRef::GPPWeakRef(const GPPWeakRef& toAssign)
{
    GObject* newData = toAssign.getObject();
    initRef(newData);
    g_clear_object(&newData);
}

/*
 * Initialize the weak reference with a GObject value. 
 */ 
GPPWeakRef::GPPWeakRef(GObject* value)
{
   initRef(value); 
}

GPPWeakRef::~GPPWeakRef()
{
    using namespace juce;
    const ScopedWriteLock destroyLock(referenceLock);
    if(refInitialized && !refCleared)
    {
        g_weak_ref_clear(&weakRef);
    }
    refCleared = true;
}

/*
 * Assign new object data to this weak reference.
 */
GPPWeakRef& GPPWeakRef::operator=(const GPPWeakRef& rhs)
{
    GObject* newData = rhs.getObject();
    *this = newData;
    g_clear_object(&newData);
    return *this;
}

/*
 * Assigns new object data to this weak reference.
 */
GPPWeakRef& GPPWeakRef::operator=(const GObject* rhs)
{

    using namespace juce;
    const ScopedReadLock assignLock(referenceLock);
    if(!refInitialized)
    {
        DBG("GPPWeakRef::" << __func__ << ": weak reference not initialized!");
    }
    else if(refCleared)
    {
        DBG("GPPWeakRef::" << __func__ << ": weak reference already cleared!");
    }
    else
    {
        g_weak_ref_set(&weakRef, const_cast<GObject*>(rhs));
    }
    return *this;
}

/*
 * Checks if two GPPWeakRefs reference the same data.
 */
bool GPPWeakRef::operator==(const GPPWeakRef& rhs) const
{
    GObject* thisData = getObject();
    GObject* rhsData = rhs.getObject();
    bool result = (thisData == rhsData);
    g_clear_object(&thisData);
    g_clear_object(&rhsData);
    return result;
}

/*
 * Checks if this object holds a specific GObject.
 */
bool GPPWeakRef::operator==(const GObject* rhs) const
{
    GObject* object = getObject();
    bool result = (rhs == object);
    g_clear_object(&object);
    return result;
}

/*
 * Casts stored object pointer data to an unsigned long, so GPPWeakRefs
 * can be used as hash keys.
 */
GPPWeakRef::operator juce::uint64() const
{
    GObject* object = getObject();
    if(object != NULL)
    {
        g_object_unref(object);
    }
    return (juce::uint64) object;
}


/*
 * Attempts to return the GObject referenced by this GPPWeakRef.  If a
 * non-null value is returned, an additional strong reference will have been
 * added to the returned GObject.
 */
GObject* GPPWeakRef::getObject() const
{
    using namespace juce;
    const ScopedReadLock assignLock(referenceLock);
    if(!refInitialized)
    {
        DBG("GPPWeakRef::" << __func__ << ": weak reference not initialized!");
    }
    else if(refCleared)
    {
        DBG("GPPWeakRef::" << __func__ << ": weak reference already cleared!");
    }
    else
    {
        gpointer object = g_weak_ref_get(const_cast<GWeakRef*>(&weakRef));
        return (object == NULL) ? nullptr : G_OBJECT(object);
    }
    return nullptr;
}

/*
 * Initializes the object's GWeakRef. Only constructors should call this.
 */
void GPPWeakRef::initRef(const GObject* initialValue)
{
    using namespace juce;
    const ScopedWriteLock initLock(referenceLock);
    if(!refInitialized && !refCleared)
    {
        g_weak_ref_init(&weakRef, const_cast<GObject*>(initialValue));
        refInitialized = true;
    }
} 
