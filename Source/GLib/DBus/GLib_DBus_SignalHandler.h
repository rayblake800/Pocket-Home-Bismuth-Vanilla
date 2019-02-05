#pragma once
/**
 * @file  GLib_DBus_SignalHandler.h
 *
 * @brief  Handles GLib signals from DBus proxy objects.
 */

#include "GLib_Signal_Handler.h"

namespace GLib { namespace DBus { 
        template <class ProxyType> class SignalHandler; } }

/**
 * @brief  Generic base class for objects that receive DBus signals.
 *
 * @tparam  ProxyType  The concrete subclass of DBusProxy the SignalHandler
 *                     will access.
 */
template <class ProxyType>
class GLib::DBus::SignalHandler : public GLib::Signal::Handler<ProxyType>
{
public:
    SignalHandler() { }
    
    virtual ~SignalHandler() { }
    
    /**
     * @brief  Subscribes to all DBus signals and property changes emitted 
     *         by this signal source.
     * 
     * @param source  A DBusProxy signal source.  This function will do 
     *                nothing if this source is null.
     */
    virtual void connectAllSignals(ProxyType& source) override
    {
        if(!source.isNull())
        {
            createConnection("g-signal", G_CALLBACK(dBusSignalCallback),
                    new Signal::CallbackData<ProxyType>(source, this));
            createConnection("g-properties-changed", 
                    G_CALLBACK(dBusSignalCallback),
                    new Signal::CallbackData<ProxyType>(source, this));
        }
    }
    
private:
    /**
     * @brief  This will be called whenever the DBus object emits a signal.
     *        
     *  SignalHandler subclasses should override this to handle the specific 
     * signals they expect to receive.
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
            ProxyType& source,
            juce::String senderName,
            juce::String signalName,
            GVariant* parameters)
    {
        DBG("GLib::DBusProxy::SignalHandler::" << __func__ 
                << ": Received unhandled signal " 
                << signalName << " from " << senderName);
    }
    
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
    virtual void dBusPropertyChanged(ProxyType& source,
            juce::String propertyName, GVariant* newValue)
    {   
        DBG("GLib::DBusProxy::SignalHandler::" << __func__ <<
                ": Received unhandled change to property " 
                << propertyName);
    }
    
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
    virtual void dBusPropertyInvalidated(ProxyType& source,
            juce::String propertyName)
    {   
        DBG("GLib::DBusProxy::SignalHandler::" << __func__ <<
                ": Received unhandled invalidation message for property " 
                << propertyName);
    }

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
     * @param data        The callback data needed to pass the signal to its
     *                    signal handler.
     */
    static void dBusSignalCallback(ProxyType* proxy,
            gchar* senderName,
            gchar* signalName,
            GVariant* parameters,
            Signal::CallbackData<ProxyType>* data);
    
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
     * @param data                    The callback data needed to pass the 
     *                                signal to its signal handler.
     */
    static void dBusPropertiesChanged(ProxyType* proxy,
            GVariant* changedProperties,
            GStrv invalidatedProperties,
            Signal::CallbackData<ProxyType>* data);

    typedef std::function<void(ProxyType&, SignalHandler*)> SignalAction;
    
    /**
     * @brief  Checks the validity of a signal callback's signal source and
     *         handler, and then performs some action if the source and handler
     *         are valid.
     *
     * @param data          A signal callback data object.
     * 
     * @param signalAction  The action to perform with the signal source and
     *                      handler if the data object was valid.
     */
    static void handleCallback(Signal::CallbackData<ProxyType>* data,
            SignalAction signalAction);
};


/*
 * Checks the validity of a signal callback's signal source and handler, and 
 * then performs some action if the source and handler are valid.
 */
template <class ProxyType>
void GLib::DBus::SignalHandler<ProxyType>::handleCallback
(Signal::CallbackData<ProxyType>* data, SignalAction signalAction)
{
    if(data == nullptr)
    {
        return;
    }
    ProxyType sourceProxy = data->getSignalSource();
    if(!sourceProxy.isNull())
    {
        SignalHandler* handler = dynamic_cast<SignalHandler*>
                (data->getSignalHandler());
        if(handler != nullptr)
        {
            signalAction(sourceProxy, handler);
        }
    }
}

/*
 * A callback function for handling all DBus signals.
 */
template <class ProxyType>
void GLib::DBus::SignalHandler<ProxyType>::dBusSignalCallback(
        GDBusProxy* proxy,
        gchar* senderName,
        gchar* signalName,
        GVariant* parameters,
        Signal::CallbackData<ProxyType>* data)
{
    SignalAction callbackAction = [senderName, signalName, parameters]
        (ProxyType& sourceProxy, SignalHandler* handler)
    {
        handler->dBusSignalReceived(sourceProxy,
                juce::String(senderName),
                juce::String(signalName),
                parameters);
    };
    handleCallback(data, callbackAction);
}

/*
 * A callback function for handling DBus property change signals.
 */
template <class ProxyType>
void GLib::DBus::SignalHandler<class ProxyType>::dBusPropertiesChanged(
        GDBusProxy* proxy,
        GVariant* changedProperties,
        GStrv invalidatedProperties,
        Signal::CallbackData<ProxyType>* data)
{
    SignalAction propertySignalAction 
        = [changedProperties, &invalidatedProperties]
        (ProxyType& sourceProxy, SignalHandler* handler)
    {
        using namespace VariantConverter;
        using juce::String;
        iterateDict(changedProperties,[&sourceProxy, handler]
                (GVariant* key, GVariant* property)
        {
           String propName = getValue<String>(key);
           handler->dBusPropertyChanged(sourceProxy, propName, property);
        });
        for(int i = 0; invalidatedProperties[i] != nullptr; i++)
        {
            String invalidProp(invalidatedProperties[i]);
            handler->dBusPropertyInvalidated(sourceProxy, invalidProp);
        }
    };
    g_variant_unref(changedProperties);
    handleCallback(data, propertySignalAction);
}
