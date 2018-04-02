#include "GDBusProxyObject.h"

GDBusProxyObject::GDBusProxyObject
(const char* name, const char* path, const char* interface)
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
    }
}

GVariant* GDBusProxyObject::callMethod(String methodName, GVariant* params)
{
    GError * error = nullptr;
    GVariant* result = g_dbus_proxy_call_sync(proxy,
            methodName.toRawUTF8(),
            params,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error);
    if (error != nullptr)
    {
        DBG(__func__ << "calling DBus adapter proxy method "
                << methodName << " failed!");
        DBG("Error: " << String(error->message));
    }
    return result;
}