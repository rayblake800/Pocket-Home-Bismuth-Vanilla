#include "SavedConnection.h"

#define BUS_NAME "org.freedesktop.NetworkManager"
#define INTERFACE "org.freedesktop.NetworkManager.Settings.Connection"

//Methods:
#define GET_SETTINGS "GetSettings"
#define GET_SECRETS "GetSecrets"
#define DELETE_CONN "Delete"
#define UPDATE_CONN "Update"

//Settings types and parameters:
#define SETTING_CONN "connection"
#define SETTING_CONN_AUTOCONNECT "autoconnect"
#define SETTING_CONN_ID "id"
#define SETTING_CONN_UUID "uuid"
#define SETTING_CONN_TYPE "type"
#define SETTING_CONN_TIMESTAMP "timestamp"

#define SETTING_WIFI "802-11-wireless"
#define SETTING_WIFI_SSID "ssid"
#define SETTING_WIFI_MODE "mode"

#define SETTING_WIFI_SECURITY "802-11-wireless-security"
#define SETTING_WIFI_SECURITY_KEY_MGMT "key-mgmt"

#define SETTING_BLUETOOTH "bluetooth"
#define SETTING_BLUETOOTH_ADDR "bdaddr"
#define SETTING_BLUETOOTH_TYPE "type"

/*
 * Create an empty object with no linked connection.
 */
SavedConnection::SavedConnection() :
GPPDBusProxy(nullptr, nullptr, nullptr) { } 

/**
 * Create an object from an existing DBus Connection proxy.
 */
SavedConnection::SavedConnection(const SavedConnection& toCopy) :
GPPDBusProxy(nullptr, nullptr, nullptr),
path(toCopy.path),
settingNames(toCopy.settingNames),
nmConnection(toCopy.nmConnection)
{
    setGObject(toCopy);
}

/**
 * Initialize a SavedConnection from a DBus connection path.
 */ 
SavedConnection::SavedConnection(const char * path) :
GPPDBusProxy(BUS_NAME, path, INTERFACE),
path(path)
{ 
    if(!isVoid())
    {
        createNMConnection();
    }
}

/**
 * Gets the connection's DBus path.
 */
const String& SavedConnection::getPath() const
{
    return path;
}


/*
 * Checks if this connection is a wifi connection.
 */
bool SavedConnection::isWifiConnection() const
{
    if(isVoid())
    {
        return false;
    }
    return hasSetting(SETTING_WIFI);
}
  
/**
 * Replace the connection's existing wifi security settings.
 */
void SavedConnection::updateWifiSecurity(GVariant* newSettings)
{
    using namespace GVariantConverter;
    if(isVoid())
    {
        return;
    }
    GVariantBuilder* builder = g_variant_builder_new
            (G_VARIANT_TYPE("a{sa{sv}}"));
    GVariant* oldSettings = callMethod(GET_SETTINGS);
    #ifdef JUCE_DEBUG
    jassert(builder != nullptr && oldSettings != nullptr 
            && newSettings != nullptr);
    DBG("SavedConnection::" << __func__ << ": Updating connection security.");
    std::cout << " New settings:\n" << toString(newSettings) << "\n";
    #endif
    bool newSettingsAdded = false;
    iterateDict(oldSettings,[this, builder, newSettings, &newSettingsAdded]
            (GVariant* key, GVariant* val)
    {
        String keyStr = getValue<String>(key);
        if(keyStr.isEmpty())
        {
            return;
        }
              
        StringArray keysToReplace;
        if(!newSettingsAdded && keyStr == SETTING_WIFI_SECURITY)
        {
            newSettingsAdded = true;
            keysToReplace = getKeys(newSettings);
        }
    	GVariantBuilder* settingBldr = g_variant_builder_new
	        (G_VARIANT_TYPE("a{sv}"));
	iterateDict(val,[this, settingBldr, newSettings, &keysToReplace]
                (GVariant* key, GVariant* val)
        {
	    String keyStr = getValue<String>(key);
            GVariant* addedValue = val;
            if(keysToReplace.contains(keyStr))
            {
                addedValue = g_variant_lookup_value(newSettings,
                        keyStr.toRawUTF8(), nullptr);
            }
            if(addedValue != nullptr)
            {
                g_variant_builder_add(settingBldr, "{sv}",
                        keyStr.toRawUTF8(), addedValue);
            }
	});
	for(const String& key : keysToReplace)
	{
	    GVariant* newVal = g_variant_lookup_value(newSettings,
	            key.toRawUTF8(), nullptr);
	    if(newVal != nullptr)
	    {
	        g_variant_builder_add(settingBldr, "{sv}",
		        key.toRawUTF8(), newVal);
	    }
	}
        g_variant_builder_add(builder, "{sa{sv}}",
                keyStr.toRawUTF8(), settingBldr);
    });
    if(!newSettingsAdded)
    {
        GVariantBuilder* settingBldr = g_variant_builder_new
	        (G_VARIANT_TYPE("a{sv}"));
	iterateDict(newSettings,[this, settingBldr]
                (GVariant* key, GVariant* val)
        {
            String keyStr = getValue<String>(key);
            GVariant* addedValue = val;
            if(val != nullptr)
            {
                g_variant_builder_add(settingBldr, "{sv}",
                        keyStr.toRawUTF8(), val);
            }
	});
	g_variant_builder_add(builder, "{sa{sv}}",
                SETTING_WIFI_SECURITY, settingBldr);
    }
    GVariant* updatedSettings = g_variant_builder_end(builder);
    builder = nullptr;
    g_variant_unref(newSettings);
    newSettings = nullptr;
    jassert(updatedSettings != nullptr);
    callMethod(UPDATE_CONN, updatedSettings);
    createNMConnection();
}

    
/**
 * Removes any saved WEP key or WPA PSK attached to this connection.
 */
void SavedConnection::removeSecurityKey()
{
    if(isVoid())
    {
        return;
    }
    using namespace GVariantConverter;
    GVariantDict* newDict = g_variant_dict_new(nullptr);
    GVariant* oldSettings = getSetting(SETTING_WIFI_SECURITY);
    if(oldSettings != nullptr)
    {
        iterateDict(oldSettings,[this, newDict]
                (GVariant* key, GVariant* val)
        {
            String keyStr = getValue<String>(key);
            if(keyStr == NM_SETTING_WIRELESS_SECURITY_WEP_KEY0
               || keyStr == NM_SETTING_WIRELESS_SECURITY_WEP_KEY1
               || keyStr == NM_SETTING_WIRELESS_SECURITY_WEP_KEY2
               || keyStr == NM_SETTING_WIRELESS_SECURITY_WEP_KEY3
               || keyStr == NM_SETTING_WIRELESS_SECURITY_PSK)
            {
                return;
            }
            g_variant_ref(val);
            g_variant_dict_insert_value(newDict, keyStr.toRawUTF8(), val);
        });
        updateWifiSecurity(g_variant_dict_end(newDict));
        g_variant_unref(oldSettings);
        oldSettings = nullptr;
    }
}

/*
 * Gets the NMConnection object generated from this connection's data.
 * Only wifi connections are supported, others are not guaranteed to work.
 */
NMPPConnection SavedConnection::getNMConnection()
{
    return nmConnection;
}

/**
 * Deletes this connection from the list of saved connections.  This object
 * will be invalid after this method is called.
 */
void SavedConnection::deleteConnection()
{
    if(!isVoid())
    {
        callMethod(DELETE_CONN);
        removeData();
        path = "";
        nmConnection = NMPPConnection();
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
    return path == String(nm_connection_get_path(rhs));
}

/*
 * Create a NMConnection object using this saved connection's data.
 * Only wifi connections are supported, others are not guaranteed to work.
 */
void SavedConnection::createNMConnection()
{
    if(isVoid())
    {
        return;
    }
    using namespace GVariantConverter;
    if(!settingNames.isEmpty())
    {
        settingNames.clear();
        nmConnection = NMPPConnection();
    }
    nmConnection.setPath(path.toRawUTF8());
    GVariant* settings = callMethod(GET_SETTINGS);
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
            if (keyStr == SETTING_CONN)
            {
                setting = nm_setting_connection_new();
            }
            else if (keyStr == SETTING_WIFI)
            {
                setting = nm_setting_wireless_new();
            }
            else if (keyStr == SETTING_WIFI_SECURITY)
            {
                setting = nm_setting_wireless_security_new();
            }
            if (setting != nullptr)
            {
                iterateDict(val, copyDict);
                if (keyStr == SETTING_WIFI_SECURITY)
                {
                    GError * secretsError = nullptr;
                    GVariant* secrets = callMethod(GET_SECRETS,
                            g_variant_new_string(SETTING_WIFI_SECURITY),
                            &secretsError);
                    if (secrets != nullptr)
                    {
                        GVariant* securitySecrets = g_variant_lookup_value
                                (secrets, SETTING_WIFI_SECURITY, nullptr);
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
                                << ": Reading secrets failed, error code="
                                << secretsError->code);
                        g_clear_error(&secretsError);
                        secretsError = nullptr;
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
GVariant* SavedConnection::getSetting(const char* name)
{  
    if(isVoid())
    {
        return nullptr;
    }
    GVariant* allSettings = callMethod(GET_SETTINGS);
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
        const char* propName)
{  
    if(isVoid())
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
        const char* propName)
{
    if(isVoid())
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
    if(isVoid())
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
        const char* propName)
{
    if(isVoid())
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

