#include <nm-setting-connection.h>
#include <nm-setting-wireless.h>
#include <nm-setting-wireless-security.h>
#include "SavedConnection.h"

/*
 * Create an empty object with no linked connection.
 */
SavedConnection::SavedConnection() :
GLib::DBusProxy(nullptr, nullptr, nullptr) { } 

/*
 * Creates an object from an existing DBus Connection proxy.
 */
SavedConnection::SavedConnection(const SavedConnection& toCopy) :
GLib::DBusProxy(nullptr, nullptr, nullptr),
path(toCopy.path),
settingNames(toCopy.settingNames),
nmConnection(toCopy.nmConnection)
{
    setGObject(toCopy);
}

/*
 * Initializes a SavedConnection from a DBus connection path.
 */ 
SavedConnection::SavedConnection(const char * path) :
GLib::DBusProxy(busName, path, interfaceName),
path(path) 
{ 
    if(!isNull())
    {
        createNMConnection();
    }
}

/**
 * Gets the connection's DBus path.
 */
const juce::String& SavedConnection::getPath() const
{
    return path;
}


/*
 * Checks if this connection is a wifi connection.
 */
bool SavedConnection::isWifiConnection() const
{
    if(isNull())
    {
        return false;
    }
    return hasSetting(NM_SETTING_WIRELESS_SETTING_NAME);
}

/*
 * Gets the NMConnection object generated from this connection's data.
 * Only wifi connections are supported, others are not guaranteed to work.
 */
NMPPConnection SavedConnection::getNMConnection() const
{
    return nmConnection;
}
 
/*
 * Gets the last recorded time this saved connection was active.
 */
juce::Time SavedConnection::lastConnectionTime() const
{ 
    juce::Time lastTime;
    if(!isNull())
    {
        GVariant* timestamp = getSettingProp(NM_SETTING_CONNECTION_SETTING_NAME,
                NM_SETTING_CONNECTION_TIMESTAMP);
        if(timestamp != nullptr)
        {
	        lastTime = juce::Time(1000 
                    * GVariantConverter::getValue<juce::uint64> (timestamp));
            g_variant_unref(timestamp);
        }
    }
    return lastTime;
}



/*
 * Checks if the connection has a saved wireless security key.
 */
bool SavedConnection::hasSavedKey() const
{
    if(isNull())
    {
        return false;
    }
    using juce::String;
    using namespace GVariantConverter;
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

/**
 * Deletes this connection from the list of saved connections.  This object
 * will be invalid after this method is called.
 */
void SavedConnection::deleteConnection()
{
    if(!isNull())
    {
        callMethod(deleteConnectionMethod);
        clearGObject();
        path = "";
        NMPPConnection emptyConnection;
        nmConnection = emptyConnection; 
        settingNames.clear();
    }
}

    
/**
 * Compare SavedConnections using the connection path.
 */
bool SavedConnection::operator==(const SavedConnection& rhs) const
{
    return path == rhs.path;
}

/**
 * Compare SavedConnections with NMConnections using the connection path.
 */
bool SavedConnection::operator==(NMConnection* rhs) const
{
    return path == juce::String(nm_connection_get_path(rhs));
}

/*
 * Create a NMConnection object using this saved connection's data.
 * Only wifi connections are supported, others are not guaranteed to work.
 */
void SavedConnection::createNMConnection()
{
    if(isNull())
    {
        return;
    }
    using juce::String;
    using namespace GVariantConverter;
    if(!settingNames.isEmpty())
    {
        settingNames.clear();
        NMPPConnection emptyConnection;
        nmConnection = emptyConnection; 
    }
    nmConnection.setPath(path.toRawUTF8());
    GVariant* settings = callMethod(getSettingsMethod);
    if (settings != nullptr)
    {
        NMSetting* setting = nullptr;
        std::function<void(GVariant*, GVariant*) > copyDict = [this, &setting]
                (GVariant* key, GVariant * val)
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
                    g_object_set_property(G_OBJECT(setting), keyStr.toRawUTF8(),
                            &propValue);
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
                nmConnection.addSetting(setting);
                setting = nullptr;
            }
        });
    }
}

/*
 * Returns one of this connection's settings objects.
 */
GVariant* SavedConnection::getSetting(const char* name) const
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
 * Returns the value of a specific property for a specific settings
 * object
 */
GVariant* SavedConnection::getSettingProp(const char* settingName,
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
 * Returns the value of a specific property for a specific settings
 * object
 */
GVariant* SavedConnection::getSettingProp(GVariant* settingsObject,
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
bool SavedConnection::hasSetting(const char* settingName) const
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
 * settings type. Don't use this if you actually need any data from the 
 * setting, in that case it's more effective to just get the setting object
 * and check if getSettingParam returns null.
 */
bool SavedConnection::hasSettingProperty(const char* settingName,
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

