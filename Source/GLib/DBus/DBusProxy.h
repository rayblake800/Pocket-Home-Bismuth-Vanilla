/**
 * @file  GLib/DBus/DBusProxy.h
 * 
 * @brief  Provides an interface for easier access to a GDBusProxy.
 * 
 * This object connects to a DBus interface on construction. Once connected,
 * it can get and set properties on that interface, and call interface methods.
 * 
 * DBusProxy objects are not meant to be interacted with directly. Instead, each 
 * interface type should be implemented as a class inheriting DBusProxy.
 */
#pragma once
#include "gio/gio.h"
#include "JuceHeader.h"
#include "GVariantConverter.h"
#include "GLib/Object.h"
#include "GLib/ThreadHandler.h"
#include "GLib/SignalHandler.h"

class GLib::DBusProxy : public GLib::Object,
    public GLib::ThreadHandler<DBusThread>
{
protected:
    /**
     * @brief  Opens a new DBusProxy.
     * 
     * @param name        The name of the bus providing the DBus interface.  
     *                    This is usually something like "com.Group.ServiceName"
     * 
     * @param path        The DBus path to the interface object. This is usually
     *                    something like "/com/Group/ServiceName/SomeObjType/1"
     * 
     * @param interface   The interface type used by the object at the given
     *                    path. This is usually something like
     *                    "com.Group.ServiceName.SomeObjType"
     */
    DBusProxy(const char* name, const char* path,
            const char* interface);
    
    /**
     * @brief  Creates an object holding an existing GDBusProxy.
     * 
     * @param proxy  An initialized proxy object.  
     */
    DBusProxy(GDBusProxy * proxy);
    
    /**
     * @brief  Creates a DBusProxy as a copy of another DBusProxy
     * 
     * @param proxy  The DBusProxy object to copy.
     */
    DBusProxy(const DBusProxy& proxy);
    
public:
    virtual ~DBusProxy() { }
    
    /**
     * @brief  Generic base class for objects that receive DBus signals.
     */
    class DBusSignalHandler : public GLib::SignalHandler
    {
    public:
        friend class DBusProxy;

        DBusSignalHandler() { }
        
        virtual ~DBusSignalHandler() { }
        
        /**
         * @brief  Subscribes to all DBus signals and property changes emitted 
         *         by this signal source.
         * 
         * @param source  A GDBusProxy's underlying GObject.  This function will
         *                do nothing if this source is nullptr or not a
         *                GDBusProxy.
         */
        virtual void connectAllSignals(GObject* source);
        
    private:
        /**
         * @brief  This will be called whenever the DBus object emits a signal.
         *        
         * DBusSignalHandler subclasses should override this to handle the 
         * specific signals they expect to receive.
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
        virtual void dBusSignalReceived(
                DBusProxy& source,
                juce::String senderName,
                juce::String signalName,
                GVariant* parameters); 
        
        /**
         * @brief  This will be called whenever a property of the DBus object 
         *         changes. 
         *
         * DBusSignalHandler subclasses should override this to handle the
         * specific property changes they need to receive.
         * 
         * @param source        The DBus proxy object.
         * 
         * @param propertyName  The name of the updated property.
         * 
         * @param newValue      A GVariant holding the updated property value.
         */
        virtual void dBusPropertyChanged(DBusProxy& source,
                juce::String propertyName, GVariant* newValue);
        
        /**
         * @brief  This will be called whenever a property of the DBus object 
         *         becomes invalid.
         *
         * DBusSignalHandler subclasses should override this to handle the
         * specific property changes they need to receive.
         * 
         * @param source        The DBus proxy object.
         * 
         * @param propertyName  The name of the invalidated property.
         */
        virtual void dBusPropertyInvalidated(DBusProxy& source,
                juce::String propertyName);
    };

protected:
    /**
     * @brief  Checks if the DBus interface has a property with a particular 
     *         name.
     * 
     * @propertyName  A string value to search for as a property name.
     * 
     * @return  Whether the interface is connected and has a property with the
     *          given name.
     */
    bool hasProperty(const char *  propertyName) const;

    /**
     * @brief  Attempts to read and return a property from the DBus interface.
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
    template<typename T> T getProperty(const char *  propertyName) const
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
     * @brief  Attempts to set one of the DBus interface properties.
     * 
     * @tparam T            The property type to set. This must be one of the 
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
     * @brief  Calls one of the methods provided by this DBus interface.
     *
     * Any non-null value returned by this method should eventually be freed 
     * with g_variant_unref.
     * 
     * @param methodName  The ID of a method to call on this interface.
     * 
     * @param params      GVariant parameters to pass in with the method call,
     *                    or nullptr if the method takes no parameters.
     *                    If passing multiple parameters, they should be bound
     *                    together in a GVariant tuple object.  If the method
     *                    only takes a single parameter, the DBusProxy
     *                    will handle packaging it in a tuple if necessary.
     * 
     * @param error       The address of a null GError*, or nullptr.  If this
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
     * 
     */
    GVariant* callMethod(const char * methodName, GVariant* params = nullptr,
            GError** error = nullptr) const;
    
    /**
     * @brief  Registers a signal handler to receive DBus signals and property 
     *         updates.
     * 
     * @param signalHandler  A signal handler that will receive all signals
     *                       and property updates emitted by the DBus object.
     */
    void connectSignalHandler(DBusSignalHandler& signalHandler);

    /**
     * @brief  A callback function for handling all DBus signals.
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
     * @brief  A callback function for handling DBus property change signals.
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
