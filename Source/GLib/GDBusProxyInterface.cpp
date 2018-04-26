#include "GDBusProxyInterface.h"
#include "GLibSignalHandler.h"

/*
 * Removes all signal handlers and dereferences the GDBusProxy object.
 */
GDBusProxyInterface::~GDBusProxyInterface()
{
    
}

GDBusProxyInterface::GDBusProxyInterface
(const char* name, const char* path, const char* interface)
{
    if(name == nullptr || path == nullptr || interface == nullptr)
    {
        //creating an invalid/null interface, skip initialization
        return;
    }

    GLibSignalHandler signalThread;
    signalThread.gLibCall([this, name, path, interface]()
    {
        GError * error = nullptr;
        proxy = g_dbus_proxy_new_for_bus_sync(
                G_BUS_TYPE_SYSTEM,
                G_DBUS_PROXY_FLAGS_NONE,
                nullptr,
                name,
                path,
                interface,
                nullptr,
                &error);
        if (error != nullptr)
        {
            DBG(__func__ << "Opening DBus adapter proxy "
                    << name << " failed!");
            DBG("Error: " << String(error->message));
            proxy = nullptr;
            g_clear_error(&error);
        }
    });
    
}

/*
 * Calls one of the methods provided by this interface.
 */
GVariant* GDBusProxyInterface::callMethod
(const char *  methodName, GVariant* params, GError ** error)
{
    if(params != nullptr && !g_variant_is_of_type(params, G_VARIANT_TYPE_TUPLE))
    {
        GVariant* tuple = g_variant_new_tuple(&params, 1);
        params = tuple;
    }
    GError * defaultError = nullptr;
    GVariant* result = g_dbus_proxy_call_sync(proxy,
            methodName,
            params,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            (error==nullptr ? &defaultError: error));
    if (defaultError != nullptr)
    {
        DBG("GDBusProxyInterface::" << __func__ 
                << ": calling DBus adapter proxy method "
                << methodName << " failed!");
        DBG("Error: " << String(defaultError->message));
        g_clear_error(&defaultError);
    }
    if(result != nullptr  && g_variant_is_container(result))
    {
        gsize resultSize = g_variant_n_children(result);
        if(resultSize == 0)
        {
            g_variant_unref(result);
            return nullptr;
        }
        else if(resultSize == 1)
        {
            GVariant* extracted = nullptr;
            g_variant_get(result, "(*)" , &extracted);
            g_variant_unref(result);
            result = extracted;
        }
    }
    return result;
}