#include "GLib_WeakRef.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "GLib::WeakRef::";
#endif

/*
 * Creates a weak reference with no initial data.
 */
GLib::WeakRef::WeakRef()
{
    initRef(nullptr);
}

/*
 * Initializes the weak reference with another WeakRef's data.
 */
GLib::WeakRef::WeakRef(const WeakRef& toAssign)
{
    GObject* newData = toAssign.getObject();
    initRef(newData);
    g_clear_object(&newData);
}

/*
 * Initializes the weak reference from a temporary WeakRef.
 */
GLib::WeakRef::WeakRef(WeakRef&& toMove)
{
    weakRef = toMove.weakRef;
    refInitialized = toMove.refInitialized;
    toMove.refCleared = refInitialized;
}

/*
 * Initializes the weak reference with a GObject value. 
 */ 
GLib::WeakRef::WeakRef(GObject* value)
{
   initRef(value); 
}

/*
 * Clears the reference on destruction.
 */
GLib::WeakRef::~WeakRef()
{
    const juce::ScopedWriteLock destroyLock(referenceLock);
    if(refInitialized && !refCleared)
    {
        g_weak_ref_clear(&weakRef);
    }
    refCleared = true;
}

/*
 * Assigns new object data to this weak reference.
 */
GLib::WeakRef& GLib::WeakRef::operator=(const WeakRef& rhs)
{
    GObject* newData = rhs.getObject();
    *this = newData;
    g_clear_object(&newData);
    return *this;
}

/*
 * Assigns new object data to this weak reference.
 */
GLib::WeakRef& GLib::WeakRef::operator=(const GObject* rhs)
{
    const juce::ScopedReadLock assignLock(referenceLock);
    if(!refInitialized)
    {
        DBG(dbgPrefix << __func__ << ": weak reference not initialized!");
    }
    else if(refCleared)
    {
        DBG(dbgPrefix << __func__ 
                << ": weak reference already cleared!");
    }
    else
    {
        g_weak_ref_set(&weakRef, const_cast<GObject*>(rhs));
    }
    return *this;
}

/*
 * Checks if two WeakRefs reference the same data.
 */
bool GLib::WeakRef::operator==(const WeakRef& rhs) const
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
bool GLib::WeakRef::operator==(const GObject* rhs) const
{
    GObject* object = getObject();
    bool result = (rhs == object);
    g_clear_object(&object);
    return result;
}

/*
 * Attempts to return the GObject referenced by this WeakRef.
 */
GObject* GLib::WeakRef::getObject() const
{
    const juce::ScopedReadLock assignLock(referenceLock);
    if(!refInitialized)
    {
        DBG(dbgPrefix << __func__ 
                << ": weak reference not initialized!");
    }
    else if(refCleared)
    {
        DBG(dbgPrefix << __func__ 
                << ": weak reference already cleared!");
    }
    else
    {
        gpointer object = g_weak_ref_get(const_cast<GWeakRef*>(&weakRef));
        return (object == NULL || !G_IS_OBJECT(object)) 
            ? nullptr : G_OBJECT(object);
    }
    return nullptr;
}

/*
 * Initializes the object's GWeakRef. Only constructors should call this.
 */
void GLib::WeakRef::initRef(const GObject* initialValue)
{
    const juce::ScopedWriteLock initLock(referenceLock);
    if(!refInitialized && !refCleared)
    {
        g_weak_ref_init(&weakRef, const_cast<GObject*>(initialValue));
        refInitialized = true;
    }
} 
