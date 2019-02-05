#define DBUS_THREAD_IMPLEMENTATION
#include "GLib_DBus_ThreadResource.h"
#include "GLib_SharedContextPtr.h"

const juce::Identifier GLib::DBus::ThreadResource::resourceKey 
        = "GLib_DBus_ThreadResource";

/*
 * Create the ThreadResource as a GLib::SharedThread resource using its own 
 * context.
 */
GLib::DBus::ThreadResource::ThreadResource() : 
    SharedThread(resourceKey.toString(), g_main_context_new()),
    SharedResource::Resource(resourceKey) { }
    
/*
 * Gets the thread resource's SharedResource instance key.
 */
const juce::Identifier& GLib::DBus::ThreadResource::getThreadResourceKey() const
{
    return resourceKey;
}

/*
 * Gets the number of references connected to this ThreadResource.
 */
int GLib::DBus::ThreadResource::getThreadReferenceCount() const
{
    return getReferenceCount();
}
