#define DBUS_THREAD_IMPLEMENTATION
#include "GLib/DBus/DBusProxy.h"
#include "GLib/DBus/DBusThread.h"
#include "GLib/SmartPointers/VariantPtr.h"
#include "GLib/SmartPointers/ErrorPtr.h"
#include "GLib/SmartPointers/ObjectPtr.h"
#include "Utils.h"

/*
 * Opens a new DBusProxy.
 */
GLib::DBusProxy::DBusProxy
(const char* name, const char* path, const char* interface) :
GLib::Owned::Object(G_TYPE_DBUS_PROXY),
GLib::ThreadHandler(DBusThread::resourceKey,
        []()->ThreadResource* { return new DBusThread(); })
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
            DBG(__func__ << "Opening DBus adapter proxy "
                    << name << " failed!");
            DBG("Error: " << juce::String(error->message));
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
        DBG("GLib::DBusProxy::DBusProxy: Opening proxy failed, couldn't "
                << "access GLib thread.");
    });
}
 
/*
 * Creates an object holding an existing GDBusProxy.
 */
GLib::DBusProxy::DBusProxy(GDBusProxy * proxy) :
GLib::Owned::Object(G_OBJECT(proxy), G_TYPE_DBUS_PROXY),
GLib::ThreadHandler(DBusThread::resourceKey,
        []()->ThreadResource* { return new DBusThread(); }) { }
        
/*
 * Creates a DBusProxy as a copy of another DBusProxy
 */
GLib::DBusProxy::DBusProxy(const DBusProxy& proxy) : 
GLib::Owned::Object(proxy, G_TYPE_DBUS_PROXY),
GLib::ThreadHandler(DBusThread::resourceKey,
        []()->ThreadResource* { return new DBusThread(); }) { }

/*
 * Subscribes to all DBus signals and property changes emitted by this
 * signal source.
 */
void GLib::DBusProxy::DBusSignalHandler::connectAllSignals(GObject* source)
{
    if(source != nullptr && G_IS_DBUS_PROXY(source))
    {
        connectSignal(source, "g-signal", G_CALLBACK(dBusSignalCallback), true);
        connectSignal(source, "g-properties-changed", 
                G_CALLBACK(dBusPropertiesChanged), true);
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
                DBG("GLib::DBusProxy::" << __func__ << ": invalid DBus proxy!");
                DBG("GLib::DBusProxy::" << __func__ 
                        << ": params will now be unreferenced if non-null.");
                if(params != nullptr)
                {
                    g_variant_unref(params);
                }
                return;
            }

            ErrorPtr defaultError([methodName](GError* error)
            {
                DBG("DBusProxy::" << __func__ 
                        << ": calling DBus adapter proxy method "
                        << methodName << " failed!");
                DBG("Error: " << juce::String(error->message));
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
            DBG("GLib::DBusProxy::callMethod: Couldn't call method "
                    << methodName << ", failed to access the DBus thread.");
        });
    return result;
}


/*
 * Checks if the DBus interface has a property with a particular name.
 */
bool GLib::DBusProxy::hasProperty(const char *  propertyName) const
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
            DBG("GLib::DBusProxy::callMethod: Couldn't check property "
                    << propertyName << ", failed to access the DBus thread.");

        });
    return hasProperty;
}

/*
 * Registers a signal handler to receive DBus signals and property updates.
 */
void GLib::DBusProxy::connectSignalHandler
(DBusProxy::DBusSignalHandler& signalHandler)
{
    ObjectPtr proxy(getGObject());
    if(proxy != nullptr)
    {
        signalHandler.connectAllSignals(proxy);
    }
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
    using namespace VariantConverter;
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
