/**
 * @file GDBusProxyInterface
 * 
 * @brief Provides an interface for easier access to a GDBusProxy
 */
#pragma once
#include "gio/gio.h"
#include "GVariantConverter.h"
#include "JuceHeader.h"

class GDBusProxyInterface
{
public:

    virtual ~GDBusProxyInterface() { }

protected:
    /**
     */
    GDBusProxyInterface(const char* name, const char* path, const char* interface);

    /**
     */
    bool hasProperty(const char *  propertyName);

    /**
     */
    template<typename T> T getProperty(const char *  propertyName)
    {
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
     */
    template<typename T> void setProperty(const char *  propertyName, T newVal)
    {
        GVariant* property = GVariantConverter::getVariant<T>(newVal);
        g_dbus_proxy_set_cached_property
                (proxy, propertyName, property);
    }

    /**
     */
    GVariant* callMethod(const char * methodName, GVariant* params = nullptr);

private:
    GDBusProxy* proxy = nullptr;
};