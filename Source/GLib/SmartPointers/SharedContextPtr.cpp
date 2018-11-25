#include "GLib/SmartPointers/SharedContextPtr.h"

/*
 * Creates a context pointer with an initial context value. 
 */
GLib::SharedContextPtr::SharedContextPtr(GMainContext* context) :
    context(context) { }

/*
 * Creates a context pointer using another context pointer's GMainContext* 
 * value.
 */
GLib::SharedContextPtr::SharedContextPtr(const SharedContextPtr& contextPtr)
{
    if(*contextPtr != nullptr)
    {
        g_main_context_ref(*contextPtr);
        context = *contextPtr;
    }
}

/*
 * Unreferences the internal GMainContext* on destruction if it holds a non-null
 * value.
 */ 
GLib::SharedContextPtr::~SharedContextPtr()
{
    if(context != nullptr)
    {
        g_main_context_unref(context);
        context = nullptr;
    }
}

/*
 * Checks if this context pointer holds a specific GMainContext. 
 */
bool GLib::SharedContextPtr::operator==(const GMainContext* rhs) const
{
    return context == rhs;
}

/*
 * Checks if this context pointer and another hold identical GMainContext 
 * pointers.
 */
bool GLib::SharedContextPtr::operator==(const SharedContextPtr& rhs) const
{
    return context == rhs.context;
}

/*
 * Checks if this context pointer does not hold a specific GMainContext.
 */
bool GLib::SharedContextPtr::operator!=(const GMainContext* rhs) const
{
    return context != rhs;
}


/*
 * Checks if this context pointer and another do not hold identical GMainContext
 * pointers.
 */
bool GLib::SharedContextPtr::operator!=(const SharedContextPtr& rhs) const
{
    return context != rhs.context;
}

/*
 * Assigns a new GLib context pointer to this SharedContextPtr. 
 */
GLib::SharedContextPtr& GLib::SharedContextPtr::operator=
(GMainContext* const rhs)
{
    if(context != nullptr)
    {
        g_main_context_unref(context);
    }
    context = rhs;
}

/*
 * Assigns a new GLib context pointer to this SharedContextPtr. 
 */
GLib::SharedContextPtr& GLib::SharedContextPtr::operator=
(const SharedContextPtr& rhs)
{
    if(context != nullptr)
    {
        g_main_context_unref(context);
        context = nullptr;
    }
    if(rhs.context != nullptr)
    {
        g_main_context_ref(rhs.context);
        context = rhs.context;
    }
    return *this;
}

/*
 * Gets this SharedContextPtr's internal GMainContext* value. 
 */
GMainContext* GLib::SharedContextPtr::operator*() const
{
    return context;
}
