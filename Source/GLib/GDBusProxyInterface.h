/**
 * @file GDBusProxyInterface
 * 
 * @brief Provides an interface for easier access to a GDBusProxy.
 * 
 * This object connects to a DBus interface on construction.  Once connected,
 * it can get and set properties on that interface, and call interface methods.
 * 
 * GDBusProxyInterface objects are not meant to be interacted with directly.
 * Instead, each interface type should be implemented as a class inheriting
 * GDBusProxyInterface.
 */
#pragma once
#include <map>
#include "gio/gio.h"
#include "GVariantConverter.h"
#include "JuceHeader.h"

class GDBusProxyInterface
{
public:

    /**
     * Removes all signal handlers and dereferences the GDBusProxy object.
     */
    virtual ~GDBusProxyInterface();

    /**
     * Checks if the object connected successfully to a DBus interface.
     * 
     * @return  true iff the connection succeeded.
     */
    bool isValid() const
    {
        return proxy != nullptr;
    }
    
protected:
    /**
     * @param name        The name of the bus providing the DBus interface.  
     *                    This is usually something like "com.Group.ServiceName"
     * 
     * @param path        The DBus path to the interface object. This is usually
     *                    something like "/com/Group/ServiceName/SomeObjType/1"
     * 
     * @param interface   The interface type used by the object at the given
     *                    path.  This is usually something like
     *                    "com.Group.ServiceName.SomeObjType"
     */
    GDBusProxyInterface(const char* name, const char* path,
            const char* interface);
    
    /**
     * 
     * 
     * @param signalName
     * 
     * @param callback
     * 
     * @return 
     */
    gulong addSignalHandler(const char* signalName, 
            std::function<void(GVariant*)> callback);
    
    /**
     * 
     * @param handlerID
     * 
     * @return 
     */
    bool removeSignalHandler(gulong handlerID);

    /**
     * Checks if the interface has a property with a particular name
     * 
     * @propertyName  A string value to search for as a property name.
     * 
     * @return  true iff the interface is connected and has a property with the
     *          given name.
     */
    bool hasProperty(const char *  propertyName);

    /**
     * Attempts to read and return a property from the interface.
     * 
     * @tparam T            The property type to read.  This must be one of the 
     *                      property types supported by 
     *                      GVariantConverter::getValue
     * 
     * @param propertyName  The name of the property value to read.
     * 
     * @return  the interface property value, or the default value of type T
     *          if the property couldn't be read.
     * 
     * @see GVariantConverter.h
     */
    template<typename T> T getProperty(const char *  propertyName)
    {
        if(!isValid())
        {
            return T();
        }
        GVariant* property = g_dbus_proxy_get_cached_property(proxy,
                propertyName);
        if(property == nullptr)
        {
            return T();
        }
        T propertyVal = GVariantConverter::getValue<T>(property);
        g_variant_unref(property);
        return propertyVal;
    }

    /**
     * Attempts to set one of the interface properties.
     * 
     * @tparam T            The property type to set.  This must be one of the 
     *                      property types supported by 
     *                      GVariantConverter::getVariant
     * 
     * @param propertyName  The name of an interface property of type T.
     * 
     * @param newVal        The value to apply to the property selected by 
     *                      propertyName.
     * 
     * @see GVariantConverter.h
     */
    template<typename T> void setProperty(const char *  propertyName, T newVal)
    {
        GVariant* property = GVariantConverter::getVariant<T>(newVal);
        g_dbus_proxy_set_cached_property
                (proxy, propertyName, property);
    }

    /**
     * Calls one of the methods provided by this interface.
     * 
     * @param methodName  The ID of a method to call on this interface.
     * 
     * @param params      GVariant parameters to pass in with the method call,
     *                    or nullptr if the method takes no parameters.
     *                    If passing multiple parameters, they should be bound
     *                    together in a GVariant tuple object.  If the method
     *                    only takes a single parameter, the GDBusProxyInterface
     *                    will handle packaging it in a tuple if necessary.
     * 
     * @param error       The address of a null GError*, or nullptr.  If this
     *                    parameter is not null and an error occurs, the GError*
     *                    will be set to the address of a new GError object.
     *                    If this happens, the caller should free the error with
     *                    g_clear_error(GError**) when it's no longer needed.
     * 
     * @return  the return value of the method. Returns nullptr instead if the 
     *          method call failed, the method returned nothing, or the method 
     *          returned an empty container object. If the method returned a 
     *          single GVariant inside of a tuple, that variant will be 
     *          extracted from its container and returned alone. 
     * 
     *          Any non-null value returned by this method should eventually be
     *          freed with g_variant_unref.
     */
    GVariant* callMethod(const char * methodName, GVariant* params = nullptr,
            GError** error = nullptr);

    /**
     * Invalidate this object, removing its DBus connection. 
     */
    void invalidate()
    {
        proxy = nullptr;
    }
    
private:
    struct SignalData
    {
        std::function<void(GVariant*)> callback;
        String dbusSignalName;
        GDBusProxyInterface* signalSource;
    };
    
    /**
     * 
     * @param proxy
     * 
     * @param senderName
     * 
     * @param signalName
     * 
     * @param parameters
     * 
     * @param signalData
     */
    static void dBusSignalCallback(GDBusProxy* proxy,
            gchar* senderName,
            gchar* signalName,
            GVariant* parameters,
            SignalData* signalData);
    
    std::map<gulong, SignalData*> signalHandlers;
    
    GDBusProxy* proxy = nullptr;
};