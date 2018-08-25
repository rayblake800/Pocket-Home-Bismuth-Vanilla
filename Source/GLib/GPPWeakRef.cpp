#include "GPPWeakRef.h"

GPPWeakRef::GPPWeakRef()
{
   g_weak_ref_init(&weakRef, NULL); 
}

/*
 * Initialize the weak reference with a GObject value. 
 */ 
GPPWeakRef::GPPWeakRef(GObject* value)
{
    g_weak_ref_init(&weakRef, value);
}

GPPWeakRef::~GPPWeakRef()
{
    g_weak_ref_clear(&weakRef);
}

/*
 * Assign new object data to this weak reference.
 */
GPPWeakRef& GPPWeakRef::operator=(const GPPWeakRef& rhs)
{
    GObject* newData = rhs.getObject();
    g_weak_ref_set(&weakRef, newData);
    g_clear_object(&newData);
    return *this;
}

/*
 * Assigns new object data to this weak reference.
 */
GPPWeakRef& GPPWeakRef::operator=(const GObject* rhs)
{
    g_weak_ref_set(&weakRef, const_cast<GObject*>(rhs));
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
    gpointer object = g_weak_ref_get(const_cast<GWeakRef*>(&weakRef));
    return (object == NULL) ? NULL : G_OBJECT(object);
}
