#define DBUS_THREAD_IMPLEMENTATION
#include "GLib_DBus_Proxy.h"
#include "GLib_DBus_ThreadResource.h"
#include "GLib_VariantPtr.h"
#include "GLib_ErrorPtr.h"
#include "GLib_ObjectPtr.h"
#include "Utils.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug statements: */
static const constexpr char* dbgPrefix = "GLib::DBus::Proxy::";
#endif


/*
 * Opens a new DBus Proxy.
 */
GLib::DBus::Proxy::Proxy
(const char* name, const char* path, const char* interface) :
GLib::Owned::Object(G_TYPE_DBUS_PROXY),
GLib::ThreadHandler<SharedResource::Handler<ThreadResource>>
        (ThreadResource::resourceKey)
{
    if(name == nullptr || path == nullptr || interface == nullptr)
    {
        //creating an invalid/null interface, skip initialization
        return;
    }

    call([this, name, path, interface]()
    {
        ErrorPtr error([name](GError* error)
        {
            DBG(dbgPrefix << __func__ << ": Opening DBus adapter proxy "
                    << name << " failed!");
            DBG(dbgPrefix << __func__ << "Error: " 
                    << juce::String(error->message));
        });
        GDBusProxy * proxy = g_dbus_proxy_new_for_bus_sync(
                G_BUS_TYPE_SYSTEM,
                G_DBUS_PROXY_FLAGS_NONE,
                nullptr,
                name,
                path,
                interface,
                nullptr,
                error.getAddress());
        if(proxy != nullptr)
        {
            setGObject(G_OBJECT(proxy));
        }
    },
    [this]()
    {
        DBG(dbgPrefix << __func__ << ": Opening proxy failed, couldn't "
                << "access GLib thread.");
    });
}
 
/*
 * Creates a proxy as a wrapper for an existing GIO proxy.
 */
GLib::DBus::Proxy::Proxy(GDBusProxy * proxy) :
GLib::Owned::Object(G_OBJECT(proxy), G_TYPE_DBUS_PROXY),
GLib::ThreadHandler<SharedResource::Handler<ThreadResource>>
        (ThreadResource::resourceKey) { }
        
/*
 * Creates the Proxy as a copy of another DBus Proxy
 */
GLib::DBus::Proxy::Proxy(const Proxy& proxy) : 
GLib::Owned::Object(proxy, G_TYPE_DBUS_PROXY),
GLib::ThreadHandler<SharedResource::Handler<ThreadResource>>
        (ThreadResource::resourceKey) { }

/*
 * Calls one of the methods provided by this DBus interface.
 */
GVariant* GLib::DBus::Proxy::callMethod
(const char* methodName, GVariant* params, GError ** error) const
{
    GVariant* result = nullptr;
    call([this, &result, methodName, &params, error]()
        {
            // DBus parameters must be packaged in a tuple, even if there's only
            // one.
            if(params != nullptr && !g_variant_is_of_type
                    (params, G_VARIANT_TYPE_TUPLE))
            {
                GVariant* tuple = g_variant_new_tuple(&params, 1);
                params = tuple;
            }

            ObjectPtr proxy(*this);
            if(proxy == nullptr)
            {
                DBG(dbgPrefix << __func__ << ": invalid DBus proxy!");
                DBG(dbgPrefix << __func__ 
                        << ": params will now be unreferenced if non-null.");
                if(params != nullptr)
                {
                    g_variant_unref(params);
                }
                return;
            }

            ErrorPtr defaultError([methodName](GError* error)
            {
                DBG(dbgPrefix << __func__ 
                        << ": calling DBus adapter proxy method "
                        << methodName << " failed!");
                DBG(dbgPrefix << __func__ << ": Error: " 
                        << juce::String(error->message));
            });

            result = g_dbus_proxy_call_sync(
                    G_DBUS_PROXY((GObject*) proxy),
                    methodName,
                    params,
                    G_DBUS_CALL_FLAGS_NONE,
                    -1,
                    nullptr,
                    (error==nullptr ? defaultError.getAddress() : error));
            if(result != nullptr && g_variant_is_container(result))
            {
                // DBus methods may return an empty container if there's no
                // values to return. Detect and unreference these. 
                gsize resultSize = g_variant_n_children(result);
                if(resultSize == 0)
                {
                    g_variant_unref(result);
                    result = nullptr;
                }
                // If the return value is a container with a single item,
                // extract and return that item.
                else if(resultSize == 1)
                {
                    GVariant* extracted = nullptr;
                    g_variant_get(result, "(*)" , &extracted);
                    g_variant_unref(result);
                    result = extracted;
                }
            }
        },
        [methodName]()
        {
            DBG(dbgPrefix << __func__ << ": Couldn't call method "
                    << methodName << ", failed to access the DBus thread.");
        });
    return result;
}


/*
 * Checks if the DBus interface has a property with a particular name.
 */
bool GLib::DBus::Proxy::hasProperty(const char *  propertyName) const
{
    bool hasProperty = false;
    call([this, propertyName, &hasProperty]()
        {
            ObjectPtr proxy(*this);
            if(proxy != nullptr)
            {
                VariantPtr property(g_dbus_proxy_get_cached_property
                        (G_DBUS_PROXY((GObject*) proxy), propertyName));
                hasProperty = (property != nullptr);
            }
        },
        [propertyName]()
        {
            DBG(dbgPrefix << __func__ << ": Couldn't check property "
                    << propertyName << ", failed to access the DBus thread.");

        });
    return hasProperty;
}

