#include "Utils.h"
#include "GLib/SmartPointers/ObjectPtr.h"
#include "GLib/SmartPointers/ErrorPtr.h"
#include "GLib/SmartPointers/VariantPtr.h"
#include "GLib/DBus/DBusProxy.h"

typedef GLib::ObjectPtr<GDBusProxy*> DBusProxyPtr;

GLib::DBusProxy::DBusProxy
(const char* name, const char* path, const char* interface) :
GLib::Object(G_TYPE_DBUS_PROXY)
{
    using namespace juce;
    if(name == nullptr || path == nullptr || interface == nullptr)
    {
        //creating an invalid/null interface, skip initialization
        return;
    }

    ErrorPtr error([name](GError* error)
    {
        DBG(__func__ << "Opening DBus adapter proxy "
                << name << " failed!");
        DBG("Error: " << String(error->message));
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
}
 
/*
 * Creates an object holding an existing GDBusProxy.
 */
GLib::DBusProxy::DBusProxy(GDBusProxy * proxy) :
GLib::Object(G_OBJECT(proxy), G_TYPE_DBUS_PROXY) { }
        
/*
 * Subscribes to all DBus signals and property changes emitted by this
 * signal source.
 */
void GLib::DBusProxy::DBusSignalHandler::connectAllSignals(GObject* source)
{
    if(source != nullptr && G_IS_DBUS_PROXY(source))
    {
        connectSignal(source, "g-signal", G_CALLBACK(dBusSignalCallback));
        connectSignal(source, "g-properties-changed", 
                G_CALLBACK(dBusPropertiesChanged));
    }
}

/*
 * This will be called whenever the DBus object emits a signal.
 */
void GLib::DBusProxy::DBusSignalHandler::dBusSignalReceived(DBusProxy& source, 
        juce::String senderName, juce::String signalName, GVariant* parameters)
{
    DBG("GLib::DBusProxy::DBusSignalHandler::" << __func__ <<
            ": Received un-handled signal " << signalName << " from "
            << senderName);
}

/*
 * This will be called whenever a property of the DBus object changes. 
 */
void GLib::DBusProxy::DBusSignalHandler::dBusPropertyChanged
(DBusProxy& source, juce::String propertyName, GVariant* newValue)
{   
    DBG("GLib::DBusProxy::DBusSignalHandler::" << __func__ <<
            ": Received un-handled change to property" << propertyName);
}

/*
 * This will be called whenever a property of the DBus object becomes invalid. 
 */
void GLib::DBusProxy::DBusSignalHandler::dBusPropertyInvalidated
(DBusProxy& source, juce::String propertyName)
{   
    DBG("GLib::DBusProxy::DBusSignalHandler::" << __func__ <<
            ": Received un-handled invalidation message for " << propertyName);
}

/*
 * Calls one of the methods provided by this DBus interface.
 */
GVariant* GLib::DBusProxy::callMethod
(const char* methodName, GVariant* params, GError ** error) const
{
    if(params != nullptr && !g_variant_is_of_type(params, G_VARIANT_TYPE_TUPLE))
    {
        GVariant* tuple = g_variant_new_tuple(&params, 1);
        params = tuple;
    }
    DBusProxyPtr proxy(G_DBUS_PROXY(getGObject()));
    if(proxy == nullptr)
    {
        DBG("GLib::DBusProxy::" << __func__ << ": invalid DBus proxy!");
        DBG("GLib::DBusProxy::" << __func__ 
                << ": params will now be unreferenced if non-null.");
        if(params != nullptr)
        {
            g_variant_unref(params);
        }
        return nullptr;
    }
    ErrorPtr defaultError([methodName](GError* error)
    {
        DBG("DBusProxy::" << __func__ 
                << ": calling DBus adapter proxy method "
                << methodName << " failed!");
        DBG("Error: " << juce::String(error->message));
    });
    GVariant* result = g_dbus_proxy_call_sync(
            proxy,
            methodName,
            params,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            (error==nullptr ? defaultError.getAddress() : error));
    if(result != nullptr && g_variant_is_container(result))
    {
        gsize resultSize = g_variant_n_children(result);
        if(resultSize == 0)
        {
            g_variant_unref(result);
            result = nullptr;
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


/*
 * Checks if the DBus interface has a property with a particular name.
 */
bool GLib::DBusProxy::hasProperty(const char *  propertyName) const
{
    DBusProxyPtr proxy(G_DBUS_PROXY(getGObject()));
    if(proxy == nullptr)
    {
        return false;
    }
    VariantPtr property(g_dbus_proxy_get_cached_property(proxy, propertyName));
    return property != nullptr;
}

/*
 * Registers a signal handler to receive DBus signals and property updates.
 */
void GLib::DBusProxy::connectSignalHandler
(DBusProxy::DBusSignalHandler& signalHandler)
{
    GObject* proxy = getGObject();
    if(proxy != nullptr)
    {
        signalHandler.connectAllSignals(proxy);
    }
    g_clear_object(&proxy);
}

/*
 * A callback function for handling all DBus signals.
 */
void GLib::DBusProxy::dBusSignalCallback(GDBusProxy* proxy,
        gchar* senderName,
        gchar* signalName,
        GVariant* parameters,
        DBusSignalHandler* handler)
{
    DBusProxy proxyWrapper(proxy);
    handler->dBusSignalReceived(proxyWrapper,
            juce::String(senderName),
            juce::String(signalName),
            parameters);
}

/*
 * A callback function for handling DBus property change signals.
 */
void GLib::DBusProxy::dBusPropertiesChanged(GDBusProxy* proxy,
        GVariant* changedProperties,
        GStrv invalidatedProperties,
        DBusSignalHandler* handler)
{
    using namespace GVariantConverter;
    using juce::String;
    DBusProxy proxyWrapper(proxy);
    iterateDict(changedProperties,[&proxyWrapper,handler]
            (GVariant* key, GVariant* property)
    {
       String propName = getValue<String>(key);
       handler->dBusPropertyChanged(proxyWrapper, propName, property);
    });
    for(int i = 0; invalidatedProperties[i] != nullptr; i++)
    {
        String invalidProp(invalidatedProperties[i]);
        handler->dBusPropertyInvalidated(proxyWrapper, invalidProp);
    }
    g_variant_unref(changedProperties);
}
