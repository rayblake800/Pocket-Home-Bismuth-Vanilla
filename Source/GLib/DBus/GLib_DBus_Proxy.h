#pragma once
/**
 * @file  GLib_DBus_Proxy.h
 * 
 * @brief  Accesses and controls a remote DBus object.
 */

#include "GLib_Owned_Object.h"
#include "GLib_Signal_Handler.h"
#include "GLib_VariantConverter.h"
#include "JuceHeader.h"
#include <gio/gio.h>

namespace GLib 
{ 
    namespace DBus 
    { 
        class Proxy;
        class ThreadResource;
    }
}

/**
 * @brief  Accesses and controls a remote DBus object.
 *
 *  This object connects to a remote DBus object on construction. Once 
 * connected, it can get and set the object's properties, and call the object's
 * methods. Proxy functions as a wrapper for the GIO library's GDBusProxy
 * objects, and new Proxy objects can be created around existing GDBusProxy*
 * values.
 * 
 *  DBus Proxy objects are not meant to be interacted with directly. Each DBus
 * interface type should be implemented as a class inheriting DBus::Proxy.
 */
class GLib::DBus::Proxy : public GLib::Owned::Object 
{
protected:
    /**
     * @brief  Opens a new DBus Proxy.
     * 
     * @param name        The name of the bus providing the DBus interface and
     *                    object. This is usually something like 
     *                    "com.Group.ServiceName"
     * 
     * @param path        The DBus path to the remote object. This is usually
     *                    something like "/com/Group/ServiceName/SomeObjType/1"
     * 
     * @param interface   The interface type used by the object at the given
     *                    path. This is usually something like
     *                    "com.Group.ServiceName.SomeObjType"
     */
    Proxy(const char* name, const char* path,
            const char* interface);
    
    /**
     * @brief  Creates a proxy as a wrapper for an existing GIO proxy.
     * 
     * @param proxy  An initialized GIO proxy object.  
     */
    Proxy(GDBusProxy * proxy);
    
    /**
     * @brief  Creates the Proxy as a copy of another Proxy. Both proxies will
     *         hold the same connection to the remote DBus object.
     * 
     * @param proxy  The Proxy object to copy.
     */
    Proxy(const Proxy& proxy);
    
public:
    virtual ~Proxy() { }

protected:
    /**
     * @brief  Checks if the DBus object has a property with a particular name.
     * 
     * @param propertyName  A string value to search for as a property name.
     * 
     * @return              Whether the DBus object is connected and has a 
     *                      property with the given name.
     */
    bool hasProperty(const char*  propertyName) const;

    /**
     * @brief  Attempts to read and return a property from the DBus object.
     * 
     * @tparam T            The property type to read. This must be one of the 
     *                      property types supported by 
     *                      GVariantConverter::getValue
     * 
     * @param propertyName  The name of the property value to read.
     * 
     * @return              The interface property value, or the default value 
     *                      of type T if the property couldn't be read.
     * 
     * @see GVariantConverter.h
     */
    template<typename T> T getProperty(const char*  propertyName) const
    {
        GDBusProxy * proxy = G_DBUS_PROXY(getGObject());
        if(proxy == nullptr)
        {
            return T();
        }
        GVariant* property = g_dbus_proxy_get_cached_property(proxy,
                propertyName);
        g_object_unref(G_OBJECT(proxy));
        if(property == nullptr)
        {
            return T();
        }
        T propertyVal = VariantConverter::getValue<T>(property);
        g_variant_unref(property);
        return propertyVal;
    }

    /**
     * @brief  Attempts to set one of the DBus object's properties.
     * 
     * @tparam T            The property type to set. This must be one of the 
     *                      property types supported by 
     *                      GVariantConverter::getVariant
     * 
     * @param propertyName  The name of a property of type T defined by the DBus
     *                      object's interface.
     * 
     * @param newVal        The value to apply to the property selected by 
     *                      propertyName.
     * 
     * @see GVariantConverter.h
     */
    template<typename T> void setProperty(const char*  propertyName, T newVal)
    {
        GDBusProxy * proxy = G_DBUS_PROXY(getGObject());
        if(proxy != nullptr)
        {
            GVariant* property = VariantConverter::getVariant<T>(newVal);
            g_dbus_proxy_set_cached_property
                    (proxy, propertyName, property);
            g_object_unref(G_OBJECT(proxy));
        }
    }

    /**
     * @brief  Calls one of the methods provided by this DBus interface.
     *
     *  Any non-null value returned by this method should eventually be freed 
     * with g_variant_unref.
     * 
     * @param methodName  The ID of a method to call on the DBus object.
     * 
     * @param params      GVariant parameters to pass in with the method call,
     *                    or nullptr if the method takes no parameters.
     *                    If passing multiple parameters, they should be bound
     *                    together in a GVariant tuple object.  If the method
     *                    only takes a single parameter, the DBusProxy
     *                    will handle packaging it in a tuple if necessary.
     * 
     * @param error       The address of a null GError*, or nullptr. If this
     *                    parameter is not null and an error occurs, the GError*
     *                    will be set to the address of a new GError object.
     *                    If this happens, the caller should free the error with
     *                    g_clear_error(GError**) when it's no longer needed.
     * 
     * @return            The return value of the method. Returns nullptr 
     *                    instead if the method call failed, the method returned
     *                    nothing, or the method returned an empty container 
     *                    object. If the method returned a single GVariant 
     *                    inside of a tuple, that variant will be extracted from 
     *                    its container and returned alone. 
     */
    GVariant* callMethod(const char* methodName, GVariant* params = nullptr,
            GError** error = nullptr) const;

    JUCE_LEAK_DETECTOR(GLib::DBus::Proxy);
};
