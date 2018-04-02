/**
 * @file GDBusProxyObject
 * 
 * @brief Provides an interface for easier access to a GDBusProxy
 */
#pragma once
#include "gio/gio.h"
#include "GVariantConverter.h"
#include "JuceHeader.h"

class GDBusProxyObject
{
public:

    virtual ~GDBusProxyObject() { }

protected:
    /**
     */
    GDBusProxyObject(const char* name, const char* path, const char* interface);

    /**
     */
    bool hasProperty(const String& propertyName);

    /**
     */
    template<typename T> T getProperty(String propertyName)
    {
        GVariant* property = g_dbus_proxy_get_cached_property(proxy,
                propertyName.toRawUTF8());
        if(property == nullptr)
        {
            return T();
        }
        T propertyVal = GVariantConverter::getValue<T>(property);
        g_variant_unref(property);
        return propertyVal;
    }

    /**
     */
    template<typename T> void setProperty(String propertyName, T newVal)
    {
        GVariant* property = GVariantConverter::getVariant<T>(newVal);
        g_dbus_proxy_set_cached_property
                (proxy, propertyName.toRawUTF8(), property);
    }

    /**
     */
    GVariant* callMethod(String methodName, GVariant* params = nullptr);

private:
    GDBusProxy* proxy = nullptr;
    ;
};