/**
 * @file GPPDBusProxy
 * 
 * @brief Provides an interface for easier access to a GDBusProxy.
 * 
 * This object connects to a DBus interface on construction.  Once connected,
 * it can get and set properties on that interface, and call interface methods.
 * 
 * GPPDBusProxy objects are not meant to be interacted with directly.
 * Instead, each interface type should be implemented as a class inheriting
 * GPPDBusProxy.
 */
#pragma once
#include "gio/gio.h"
#include "GVariantConverter.h"
#include "GPPObject.h"
#include "JuceHeader.h"

class GPPDBusProxy : public GPPObject
{
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
    GPPDBusProxy(const char* name, const char* path,
            const char* interface);
public:
    virtual ~GPPDBusProxy() { }
    
    /**
     * Generic base class for objects that receive DBus signals.
     */
    class DBusSignalHandler : public GPPObject::SignalHandler
    {
    public:
        friend class GPPDBusProxy;
        DBusSignalHandler() { }
        
        virtual ~DBusSignalHandler() { }
        
    private:
        /**
         * Called whenever the DBus object emits a signal.  DBusSignalHandler
         * subclasses should override this to handle the specific signals
         * they expect to receive.
         * 
         * @param source      The DBus proxy object.
         * 
         * @param senderName  The name of the object sending the signal.
         * 
         * @param signalName  The signal name.
         * 
         * @param parameters  A GVariant tuple containing all parameters sent
         *                    with the signal.
         */
        virtual void dBusSignalReceived(GPPDBusProxy* source,
                String senderName, String signalName, GVariant* parameters); 
        
        /**
         * Called whenever a property of the DBus object changes. 
         * DBusSignalHandler subclasses should override this to handle the
         * specific property changes they need to receive.
         * 
         * @param source        The DBus proxy object.
         * 
         * @param propertyName  The name of the updated property.
         * 
         * @param newValue      A GVariant holding the updated property value.
         */
        virtual void dBusPropertyChanged(GPPDBusProxy* source,
                String propertyName, GVariant* newValue);
        
        /**
         * Called whenever a property of the DBus object becomes invalid. 
         * DBusSignalHandler subclasses should override this to handle the
         * specific property changes they need to receive.
         * 
         * @param source        The DBus proxy object.
         * 
         * @param propertyName  The name of the invalidated property.
         */
        virtual void dBusPropertyInvalidated(GPPDBusProxy* source,
                String propertyName);
    };

protected:
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
        GDBusProxy * proxy = G_DBUS_PROXY(getGObject());
        if(proxy != nullptr)
        {
            GVariant* property = GVariantConverter::getVariant<T>(newVal);
            g_dbus_proxy_set_cached_property
                    (proxy, propertyName, property);
            g_object_unref(G_OBJECT(proxy));
        }
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
     *                    only takes a single parameter, the GPPDBusProxy
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
     * Register a signal handler to receive DBus signals.
     * 
     * @param signalHandler  A signal handler that will receive all signals
     *                       emitted by the DBus object.
     */
    void addDBusSignalHandler(DBusSignalHandler* signalHandler);
    
    /**
     * Register a signal handler to receive DBus property updates.
     * 
     * @param signalHandler  A signal handler that will be notified whenever
     *                       DBus object properties change.
     */
    void addDBusPropChangeHandler(DBusSignalHandler* signalHandler);
    
private:
    /**
     * Get GDbusProxy's GLib class type.
     * 
     * @return G_TYPE_DBUS_PROXY
     */
    virtual GType getType() const override;

    /**
     * Callback function for handling all DBus signals.
     * 
     * @param proxy       The DBus signal source.
     * 
     * @param senderName  The name of the DBus interface that sent the signal.
     * 
     * @param signalName  The name of the received signal.
     * 
     * @param parameters  A GVariant tuple containing any parameters associated
     *                    with the signal.
     * 
     * @param handler     A DBusSignalHandler subscribed to signals from this
     *                    object.
     */
    static void dBusSignalCallback(GDBusProxy* proxy,
            gchar* senderName,
            gchar* signalName,
            GVariant* parameters,
            DBusSignalHandler* handler);
    
    /**
     * Callback function for handling DBus property change signals.
     * 
     * @param proxy                   The DBus signal source.
     * 
     * @param changedProperties       A GVariant containing all updated 
     *                                properties.
     * 
     * @param invalidatedProperties   A null-terminated array of all invalidated
     *                                object properties.
     * 
     * @param handler                 A DBusSignalHandler subscribed to property
     *                                changes from this object.
     */
    static void dBusPropertiesChanged(GDBusProxy* proxy,
            GVariant* changedProperties,
            GStrv invalidatedProperties,
            DBusSignalHandler* handler);
};