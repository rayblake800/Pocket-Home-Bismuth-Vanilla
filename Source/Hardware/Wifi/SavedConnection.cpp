#include "SavedConnection.h"

#define BUS_NAME "org.freedesktop.NetworkManager"
#define INTERFACE "org.freedesktop.NetworkManager.Settings.Connection"

//Methods:
#define GET_SETTINGS "GetSettings"
#define GET_SECRETS "GetSecrets"

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

SavedConnection::SavedConnection(const char * path) :
GDBusProxyInterface(BUS_NAME, path, INTERFACE)
{
    //    GVariant* connectionSettings = callMethod(GET_SETTINGS);
    //    if(connectionSettings != nullptr)
    //    {
    //        std::cout << "Connection settings:\n" 
    //                << GVariantConverter::toString(connectionSettings) << "\n";
    //        g_variant_unref(connectionSettings);
    //        connectionSettings = nullptr;
    //    }
    createNMConnection();
}

bool SavedConnection::isWifiConnection()
{
    GVariant* type = getSettingProp(SETTING_CONN, SETTING_CONN_TYPE);
    if (type != nullptr)
    {
        String typeStr = GVariantConverter::toString(type);
        g_variant_unref(type);
        type = nullptr;
        return typeStr == SETTING_WIFI;
    }
    return false;
}

/**
 * Create a NMConnection object using this saved connection's data.
 * Only wifi connections are supported, others are not guaranteed to work.
 * 
 * @return the new NMConnection object.
 */
NMConnection* SavedConnection::createNMConnection()
{
    NMConnection* con = nm_connection_new();
    GVariant* settings = callMethod(GET_SETTINGS);
    if (settings != nullptr)
    {
        NMSetting* setting = nullptr;
        std::function<void(GVariant*, GVariant*) > copyDict = [this, &setting]
                (GVariant* key, GVariant * val)
        {
            String keyStr = GVariantConverter::toString(key);
            if (keyStr.isNotEmpty())
            {
                GValue propValue = GVariantConverter::getGValue(val);
                g_object_set_property(G_OBJECT(setting), keyStr.toRawUTF8(),
                        &propValue);
            }
        };
        GVariantConverter::iterateDict(settings, [this, con, &setting,
                &copyDict]
                (GVariant* key, GVariant * val)
        {
            String keyStr = GVariantConverter::getValue<String>(key);
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
                GVariantConverter::iterateDict(val, copyDict);
                if (keyStr == SETTING_WIFI_SECURITY)
                {
                    GVariant* secrets = callMethod(GET_SECRETS,
                            g_variant_new_string(SETTING_WIFI_SECURITY));
                    if (secrets != nullptr)
                    {
                        GVariant* securitySecrets = g_variant_lookup_value
                                (secrets, SETTING_WIFI_SECURITY, nullptr);
                        if (securitySecrets != nullptr)
                        {
                            GVariantConverter::iterateDict(securitySecrets,
                                    copyDict);
                            g_variant_unref(securitySecrets);
                            securitySecrets = nullptr;
                        }
                        g_variant_unref(secrets);
                        secrets = nullptr;
                    }
                }
                nm_connection_add_setting(con, setting);
                setting = nullptr;
            }
        });
    }
    if (con != nullptr)
    {
        nm_connection_dump(con);
    }
    return con;
}

/**
 * Returns one of this connection's settings objects.
 */
GVariant* SavedConnection::getSetting(const char* name)
{
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

/**
 * Returns the value of a specific property for a specific settings
 * object
 */
GVariant* SavedConnection::getSettingProp(const char* settingName,
        const char* propName)
{
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

/**
 * Returns the value of a specific property for a specific settings
 * object
 */
GVariant* SavedConnection::getSettingProp(GVariant* settingsObject,
        const char* propName)
{
    GVariant* property = nullptr;
    g_variant_lookup(settingsObject, propName, "@*", &property);
    return property;
}

/**
 * Checks if this connection has a particular setting type.
 * Don't use this if you actually need any data from the setting, in that
 * case it's more effective to just get the setting object and check if 
 * it is null.
 */
bool SavedConnection::hasSetting(const char* settingName)
{
    GVariant* setting = getSetting(settingName);
    if (setting != nullptr)
    {
        g_variant_unref(setting);
        setting = nullptr;
        return true;
    }
    return false;
}

/**
 * Checks if this connection has a specific settings property in a specific 
 * settings type. Don't use this if you actually need any data from the 
 * setting, in that case it's more effective to just get the setting object
 * and check if getSettingParam returns null.
 */
bool SavedConnection::hasSettingProperty(const char* settingName,
        const char* propName)
{
    GVariant* prop = getSettingProp(settingName, propName);
    if (prop != nullptr)
    {
        g_variant_unref(prop);
        prop = nullptr;
        return true;
    }
    return false;
}

