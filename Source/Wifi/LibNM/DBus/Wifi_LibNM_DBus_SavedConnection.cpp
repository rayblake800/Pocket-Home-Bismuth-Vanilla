#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_DBus_SavedConnection.h"
#include "Wifi_LibNM_Settings_Object.h"
#include "Wifi_LibNM_APHash.h"
#include "Wifi_Resource.h"
#include <nm-setting-connection.h>
#include <nm-setting-wireless.h>
#include <nm-setting-wireless-security.h>

/* The NetworkManager's DBus path: */
static const constexpr char * busName = "org.freedesktop.NetworkManager";
/* SavedConnection's DBus interface name: */
static const constexpr char * interfaceName 
        = "org.freedesktop.NetworkManager.Settings.Connection";

/* DBus getSettings method key: */
static const constexpr char * getSettingsMethod = "GetSettings";
/* DBus getSecrets method key: */
static const constexpr char * getSecretsMethod = "GetSecrets";

/* DBus delete method key: */
static const constexpr char * deleteConnectionMethod = "Delete";
/* DBus update method key: */
static const constexpr char * updateMethod = "Update";

namespace NMDBus = Wifi::LibNM::DBus;

#ifdef JUCE_DEBUG
/* Print the full class name before debug messages: */
static const constexpr char* dbgPrefix = "Wifi::LibNM::DBus::SavedConnection::";
#endif

/*
 * Creates an empty object with no linked connection.
 */
NMDBus::SavedConnection::SavedConnection() :
GLib::DBus::Proxy(nullptr, nullptr, nullptr) { } 

/*
 * Creates an object from an existing DBus Connection proxy.
 */
NMDBus::SavedConnection::SavedConnection(const SavedConnection& toCopy) :
GLib::DBus::Proxy(toCopy), path(toCopy.path)
{
    setGObject(toCopy);
}

/*
 * Initializes a SavedConnection from a DBus connection path.
 */ 
NMDBus::SavedConnection::SavedConnection(const char * path) :
GLib::DBus::Proxy(busName, path, interfaceName), path(path) { }

/*
 * Gets the connection's DBus path.
 */
const juce::String& NMDBus::SavedConnection::getPath() const
{
    return path;
}

/*
 * Checks if this connection is a wifi connection.
 */
bool NMDBus::SavedConnection::isWifiConnection() const
{
    if(isNull())
    {
        return false;
    }
    return hasSetting(NM_SETTING_WIRELESS_SETTING_NAME);
}

/*
 * Gets the NMConnection object generated from this connection's data.
 */
Wifi::LibNM::Connection 
NMDBus::SavedConnection::getNMConnection()
{
    Connection nmConnection;
    if(isNull())
    {
        return nmConnection;
    }
    using juce::String;
    using namespace GLib::VariantConverter;
    nmConnection = nm_connection_new(); 
    nmConnection.setPath(path.toRawUTF8());
    GVariant* settings = callMethod(getSettingsMethod);
    if (settings == nullptr)
    {
        return nmConnection;
    }
    NMSetting* nmSetting = nullptr;
    
    // Iterates through all properties of a settings object:
    std::function<void(GVariant*, GVariant*) > copyDict 
            = [this, &nmSetting](GVariant* key, GVariant * val)
    {
        String keyStr = getValue<String>(key);
        if (keyStr.isNotEmpty())
        {
            /* Most data types packaged as GValue data can be directly added to
             * the GObject as new properties, but byte arrays need to be
             * extracted from the GValue as a GByteArray. */
            if(getGType(val) == G_TYPE_BYTE_ARRAY)
            {
                GByteArray* byteArray = getValue<GByteArray*>(val);
                g_object_set(G_OBJECT(nmSetting),
                        keyStr.toRawUTF8(),
                        byteArray,
                        nullptr);
                g_byte_array_unref(byteArray);
            }
            else
            {
                GValue propValue = getGValue(val);
                g_object_set_property(G_OBJECT(nmSetting), 
                        keyStr.toRawUTF8(), &propValue);
            }
        }
    };

    /* Iterates through all connection settings objects: */
    std::function<void(GVariant*, GVariant*) > copySetting 
            = [this, &nmSetting, &nmConnection, &copyDict]
            (GVariant* key, GVariant * val)
    {
        /* If the settings data corresponds to a supported NMSetting type,
         * create an appropriate NMSetting object, copy the settings data into
         * it, and add the NMSetting object to the NMConnection. */
        String keyStr = getValue<String>(key);
        if (keyStr == NM_SETTING_CONNECTION_SETTING_NAME)
        {
            nmSetting = nm_setting_connection_new();
        }
        else if (keyStr == NM_SETTING_WIRELESS_SETTING_NAME)
        {
            nmSetting = nm_setting_wireless_new();
        }
        else if (keyStr == NM_SETTING_WIRELESS_SECURITY_SETTING_NAME)
        {
            nmSetting = nm_setting_wireless_security_new();
        }
        
        if (nmSetting != nullptr)
        {
            iterateDict(val, copyDict);

            nmConnection.addSettings
                (Settings::Object(nmSetting, NM_TYPE_SETTING));
            nmSetting = nullptr;
        }
    };
    iterateDict(settings, copySetting);
    return nmConnection;
}
 
/*
 * Gets the last recorded time this saved connection was active.
 */
juce::Time NMDBus::SavedConnection::lastConnectionTime() const
{ 
    juce::Time lastTime;
    if(!isNull())
    {
        GVariant* timestamp = getSettingProp(NM_SETTING_CONNECTION_SETTING_NAME,
                NM_SETTING_CONNECTION_TIMESTAMP);
        if(timestamp != nullptr)
        {
	        lastTime = juce::Time(1000 
                    * GLib::VariantConverter::getValue<juce::uint64> 
                        (timestamp));
            g_variant_unref(timestamp);
        }
    }
    return lastTime;
}

/*
 * Checks if the connection has a saved wireless security key.
 */
bool NMDBus::SavedConnection::hasSavedKey() const
{
    if(isNull())
    {
        return false;
    }
    using juce::String;
    using namespace GLib::VariantConverter;
    GError * secretsError = nullptr;
    GVariant* secrets = callMethod(
            getSecretsMethod,
            g_variant_new_string(NM_SETTING_WIRELESS_SECURITY_SETTING_NAME),
            &secretsError);
    bool keyFound = false;
    if (secrets != nullptr)
    {
        GVariant* securitySecrets = g_variant_lookup_value
                (secrets, NM_SETTING_WIRELESS_SECURITY_SETTING_NAME, nullptr);
        if (securitySecrets != nullptr)
        {
            juce::Array<const char*> keyTypes = 
            {
                NM_SETTING_WIRELESS_SECURITY_PSK,
                NM_SETTING_WIRELESS_SECURITY_WEP_KEY0,
                NM_SETTING_WIRELESS_SECURITY_WEP_KEY1,
                NM_SETTING_WIRELESS_SECURITY_WEP_KEY2,
                NM_SETTING_WIRELESS_SECURITY_WEP_KEY3          
            };
            for(const char* keyType : keyTypes)
            {
                GVariant* keyVal = g_variant_lookup_value(securitySecrets,
                        keyType, nullptr);
                if(keyVal != nullptr)
                {
                    String keyString = getValue<String>(keyVal);
                    g_variant_unref(keyVal);
                    if(keyString.isNotEmpty())
                    {
                        keyFound = true;
                        break;
                    }             
                }
            }
            g_variant_unref(securitySecrets);
            securitySecrets = nullptr;
        }
        g_variant_unref(secrets);
        secrets = nullptr;
    }
    if(secretsError != nullptr)
    {
        DBG(dbgPrefix << __func__ 
                << ": Reading secrets failed, error="
                << secretsError->message);
        g_clear_error(&secretsError);
    }
    return keyFound;
}

/*
 * Deletes this connection from the list of saved connections.
 */
void NMDBus::SavedConnection::deleteConnection()
{
    if(!isNull())
    {
        callMethod(deleteConnectionMethod);
        clearGObject();
        path = "";
    }
}

/*
 * Compares SavedConnections using their connection paths.
 */
bool NMDBus::SavedConnection::operator==
        (const SavedConnection& rhs) const
{
    return path == rhs.path;
}

/*
 * Compares SavedConnections with NMConnections using their connection paths.
 */
bool NMDBus::SavedConnection::operator==(NMConnection* rhs) const
{
    return path == juce::String(nm_connection_get_path(rhs));
}

/*
 * Returns one of this connection's settings objects.
 */
GVariant* NMDBus::SavedConnection::getSetting(const char* name) const
{  
    if(isNull())
    {
        return nullptr;
    }
    GVariant* allSettings = callMethod(getSettingsMethod);
    GVariant* setting = nullptr;
    if (allSettings != nullptr)
    {
        g_variant_lookup(allSettings, name, "@*", &setting);
        g_variant_unref(allSettings);
        allSettings = nullptr;
    }
    return setting;
}

/*
 * Returns the value of a specific settings object property.
 */
GVariant* NMDBus::SavedConnection::getSettingProp
(const char* settingName, const char* propName) const
{  
    if(isNull())
    {
        return nullptr;
    }
    GVariant* property = nullptr;
    GVariant* setting = getSetting(settingName);
    if (setting != nullptr)
    {
        property = getSettingProp(setting, propName);
        g_variant_unref(setting);
        setting = nullptr;
    }
    return property;
}

/*
 * Returns the value of a specific settings object property.
 */
GVariant* NMDBus::SavedConnection::getSettingProp
(GVariant* settingsObject, const char* propName) const
{
    if(isNull())
    {
        return nullptr;
    }
    GVariant* property = nullptr;
    g_variant_lookup(settingsObject, propName, "@*", &property);
    return property;
}

/*
 * Checks if this connection has a particular setting type.
 */
bool NMDBus::SavedConnection::hasSetting(const char* settingName) const
{
    using namespace juce;
    bool settingFound = false;
    if(!isNull())
    {
        GVariant* setting = getSetting(settingName);
        if (setting != nullptr)
        {
            settingFound = true;
            g_variant_unref(setting);
        }
    }
    return settingFound;
}

/*
 * Checks if this connection has a specific settings property in a specific 
 * settings type.
 */
bool NMDBus::SavedConnection::hasSettingProperty(const char* settingName,
        const char* propName) const
{
    if(isNull())
    {
        return false;
    }
    GVariant* prop = getSettingProp(settingName, propName);
    if (prop != nullptr)
    {
        g_variant_unref(prop);
        prop = nullptr;
        return true;
    }
    return false;
}
