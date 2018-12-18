#include "LibNM/DBus/SavedConnection.h"
#include "LibNM/NMObjects/Settings.h"
#include "LibNM/ThreadHandler.h"
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

/*
 * Creates an empty object with no linked connection.
 */
LibNM::SavedConnection::SavedConnection() :
GLib::DBusProxy(nullptr, nullptr, nullptr) { } 

/*
 * Creates an object from an existing DBus Connection proxy.
 */
LibNM::SavedConnection::SavedConnection(const SavedConnection& toCopy) :
GLib::DBusProxy(nullptr, nullptr, nullptr),
path(toCopy.path),
settingNames(toCopy.settingNames)
{
    setGObject(toCopy);
    if(!toCopy.nmConnection.isNull())
    {
        ThreadHandler threadHandler;
        threadHandler.call([this, &toCopy]()
        {
            nmConnection = toCopy.nmConnection;
        });
    }
}

/*
 * Initializes a SavedConnection from a DBus connection path.
 */ 
LibNM::SavedConnection::SavedConnection(const char * path) :
GLib::DBusProxy(busName, path, interfaceName),
path(path) 
{ 
    if(!isNull())
    {
        createNMConnection();
    }
}

/*
 * Gets the connection's DBus path.
 */
const juce::String& LibNM::SavedConnection::getPath() const
{
    return path;
}

/*
 * Checks if this connection is a wifi connection.
 */
bool LibNM::SavedConnection::isWifiConnection() const
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
LibNM::Connection LibNM::SavedConnection::getNMConnection() const
{
    return nmConnection;
}
 
/*
 * Gets the last recorded time this saved connection was active.
 */
juce::Time LibNM::SavedConnection::lastConnectionTime() const
{ 
    juce::Time lastTime;
    if(!isNull())
    {
        GVariant* timestamp = getSettingProp(NM_SETTING_CONNECTION_SETTING_NAME,
                NM_SETTING_CONNECTION_TIMESTAMP);
        if(timestamp != nullptr)
        {
	        lastTime = juce::Time(1000 
                    * VariantConverter::getValue<juce::uint64> (timestamp));
            g_variant_unref(timestamp);
        }
    }
    return lastTime;
}

/*
 * Checks if the connection has a saved wireless security key.
 */
bool LibNM::SavedConnection::hasSavedKey() const
{
    if(isNull())
    {
        return false;
    }
    using juce::String;
    using namespace VariantConverter;
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
        DBG("SavedConnection::" << __func__ 
                << ": Reading secrets failed, error="
                << secretsError->message);
        g_clear_error(&secretsError);
    }
    return keyFound;
}

/*
 * Deletes this connection from the list of saved connections.
 */
void LibNM::SavedConnection::deleteConnection()
{
    if(!isNull())
    {
        callMethod(deleteConnectionMethod);
        clearGObject();
        path = "";
        nmConnection = LibNM::Connection(); 
        settingNames.clear();
    }
}

    
/*
 * Compares SavedConnections using their connection paths.
 */
bool LibNM::SavedConnection::operator==(const SavedConnection& rhs) const
{
    return path == rhs.path;
}

/*
 * Compares SavedConnections with NMConnections using their connection paths.
 */
bool LibNM::SavedConnection::operator==(NMConnection* rhs) const
{
    return path == juce::String(nm_connection_get_path(rhs));
}

/*
 * Creates a Connection object using this saved connection's data.
 */
void LibNM::SavedConnection::createNMConnection()
{
    if(isNull())
    {
        return;
    }
    ThreadHandler threadHandler;
    threadHandler.call([this]()
    {
        using juce::String;
        using namespace VariantConverter;
        if(!settingNames.isEmpty())
        {
            settingNames.clear();
            LibNM::Connection emptyConnection;
            nmConnection = emptyConnection; 
        }
        nmConnection.setPath(path.toRawUTF8());
        GVariant* settings = callMethod(getSettingsMethod);
        if (settings != nullptr)
        {
            NMSetting* setting = nullptr;
            std::function<void(GVariant*, GVariant*) > copyDict 
            = [this, &setting](GVariant* key, GVariant * val)
            {
                String keyStr = getValue<String>(key);
                if (keyStr.isNotEmpty())
                {
                    //GObject refuses to accept byte arrays packaged in GValues
                    if(getGType(val) == G_TYPE_BYTE_ARRAY)
                    {
                        GByteArray* byteArray = getValue<GByteArray*>(val);
                        g_object_set(G_OBJECT(setting),
                                keyStr.toRawUTF8(),
                                byteArray,
                                nullptr);
                    }
                    else
                    {
                        GValue propValue = getGValue(val);
                        g_object_set_property(G_OBJECT(setting), 
                                keyStr.toRawUTF8(), &propValue);
                    }
                }
            };
            iterateDict(settings, [this, &setting, &copyDict]
            (GVariant* key, GVariant * val)
            {
                String keyStr = getValue<String>(key);
                settingNames.add(keyStr);
                if (keyStr == NM_SETTING_CONNECTION_SETTING_NAME)
                {
                    setting = nm_setting_connection_new();
                }
                else if (keyStr == NM_SETTING_WIRELESS_SETTING_NAME)
                {
                    setting = nm_setting_wireless_new();
                }
                else if (keyStr == NM_SETTING_WIRELESS_SECURITY_SETTING_NAME)
                {
                    setting = nm_setting_wireless_security_new();
                }
                if (setting != nullptr)
                {
                    iterateDict(val, copyDict);
                    if (keyStr == NM_SETTING_WIRELESS_SECURITY_SETTING_NAME)
                    {
                        GError * secretsError = nullptr;
                        GVariant* secrets = callMethod(getSecretsMethod,
                                g_variant_new_string
                                (NM_SETTING_WIRELESS_SECURITY_SETTING_NAME),
                                &secretsError);
                        if (secrets != nullptr)
                        {
                            GVariant* securitySecrets = g_variant_lookup_value
                                    (secrets,
                                    NM_SETTING_WIRELESS_SECURITY_SETTING_NAME,
                                    nullptr);
                            if (securitySecrets != nullptr)
                            {
                                iterateDict(securitySecrets, copyDict);
                                g_variant_unref(securitySecrets);
                                securitySecrets = nullptr;
                            }
                            g_variant_unref(secrets);
                            secrets = nullptr;
                        }
                        if(secretsError != nullptr)
                        {
                            DBG("SavedConnection::" << __func__ 
                                    << ": Reading secrets failed, error="
                                    << secretsError->message);
                            g_clear_error(&secretsError);
                        }
                    }
                    nmConnection.addSettings
                        (Settings(setting, NM_TYPE_SETTING));
                    setting = nullptr;
                }
            });
        }
    });
}

/*
 * Returns one of this connection's settings objects.
 */
GVariant* LibNM::SavedConnection::getSetting(const char* name) const
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
GVariant* LibNM::SavedConnection::getSettingProp(const char* settingName,
        const char* propName) const
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
GVariant* LibNM::SavedConnection::getSettingProp(GVariant* settingsObject,
        const char* propName) const
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
bool LibNM::SavedConnection::hasSetting(const char* settingName) const
{
    using namespace juce;
    if(isNull())
    {
        return false;
    }
    return settingNames.contains(String(settingName));
}

/*
 * Checks if this connection has a specific settings property in a specific 
 * settings type.
 */
bool LibNM::SavedConnection::hasSettingProperty(const char* settingName,
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

