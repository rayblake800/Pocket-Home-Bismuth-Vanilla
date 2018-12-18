#include "LibNM/NMObjects/Connection.h"
#include "LibNM/NMObjects/ConnectionSettings.h"
#include "LibNM/NMObjects/WifiSettings.h"
#include "LibNM/NMObjects/WifiSecuritySettings.h"
#include "LibNM/Data/SSID.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMConnection*> NMConnectionPtr;
typedef GLib::ObjectPtr<NMSetting*> NMSettingPtr;
typedef GLib::ObjectPtr<GObject*> GObjectPtr;

/*
 * Creates a Connection sharing a GObject with an existing Connection.
 */
LibNM::Connection::Connection(const Connection& toCopy) :
LibNM::Object(toCopy, NM_TYPE_CONNECTION) 
{
    ASSERT_CORRECT_CONTEXT;
}

/*
 * Creates a Connection to contain a NMConnection object.
 */
LibNM::Connection::Connection(NMConnection* toAssign) :
LibNM::Object(NM_OBJECT(toAssign), NM_TYPE_CONNECTION) 
{ 
    ASSERT_CORRECT_CONTEXT;
}
    
/*
 * Creates a null Connection.
 */
LibNM::Connection::Connection() : LibNM::Object(NM_TYPE_CONNECTION) { }

/*
 * Checks if this connection object and another could be describing the same 
 * network connection.
 */
bool LibNM::Connection::connectionMatches(const Connection& rhs) const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr self(NM_CONNECTION(getGObject()));
    NMConnectionPtr toCompare(NM_CONNECTION(rhs.getGObject()));
    if(self == toCompare)
    {
        return true;
    }
    else if(self == nullptr || toCompare == nullptr)
    {
        return false;
    }
    return nm_connection_compare(self, toCompare,
            NM_SETTING_COMPARE_FLAG_FUZZY);
}

/*
 * Adds a new set of settings to this connection, creating new connection data 
 * if this object is null.
 */
void LibNM::Connection::addSettings(Settings addedSettings)
{
    ASSERT_CORRECT_CONTEXT;
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    jassert(connection != nullptr);
    NMSettingPtr settingsObject = NM_SETTING(getOtherGObject(addedSettings));
    if(settingsObject != nullptr)
    {
        nm_connection_add_setting(connection, settingsObject);
    }
}

/*
 * Removes one of the connection settings from this connection.
 */
void LibNM::Connection::removeSettings(GType settingType)
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        nm_connection_remove_setting(connection, settingType);
    }
}
    
/*
 * Adds new wireless connection settings to this connection.
 */
void LibNM::Connection::addWifiSettings(SSID ssid, bool isHidden)
{
    ASSERT_CORRECT_CONTEXT;
    if(ssid.getByteArray() != nullptr)
    {   
        WifiSettings newSettings;
        newSettings.setSSID(ssid);
        newSettings.setHidden(isHidden);
        addSettings(newSettings);
    }
}

/*
 * Attempts to add WPA security settings to this connection.
 */
bool LibNM::Connection::addWPASettings(const juce::String& psk)
{
    ASSERT_CORRECT_CONTEXT;
    WifiSecuritySettings newSettings;
    bool wpaSet = newSettings.addWPASettings(psk);
    if(wpaSet)
    {
        addSettings(newSettings);
    }
    return wpaSet;
}

/*
 * Attempts to add WEP security settings to this connection.
 */
bool LibNM::Connection::addWEPSettings(const juce::String& psk)
{
    ASSERT_CORRECT_CONTEXT;
    WifiSecuritySettings newSettings;
    bool wepSet = newSettings.addWEPSettings(psk);
    if(wepSet)
    {
        addSettings(newSettings);
    }
    return wepSet;
}

/*
 * Gets this connection's basic settings.
 */
LibNM::ConnectionSettings LibNM::Connection::getConnectionSettings() const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        NMSetting* connectionSettings = getSetting(NM_TYPE_SETTING_CONNECTION);
        if(connectionSettings != nullptr)
        {
            return ConnectionSettings
                (NM_SETTING_CONNECTION(connectionSettings));
        }
    }
    return ConnectionSettings(nullptr);
}

/*
 * Gets this connection's wireless network settings.
 */
LibNM::WifiSettings LibNM::Connection::getWirelessSettings() const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        NMSetting* wifiSettings = getSetting(NM_TYPE_SETTING_WIRELESS);
        if(wifiSettings != nullptr)
        {
            return WifiSettings
                (NM_SETTING_WIRELESS(wifiSettings));
        }
    }
    return WifiSettings(nullptr);
}

/*
 * Gets this connection's wireless network security settings.
 */
LibNM::WifiSecuritySettings LibNM::Connection::getSecuritySettings() const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        NMSetting* securitySettings 
            = getSetting(NM_TYPE_SETTING_WIRELESS_SECURITY);
        if(securitySettings != nullptr)
        {
            return WifiSecuritySettings
                (NM_SETTING_WIRELESS_SECURITY(securitySettings));
        }
    }
    return WifiSecuritySettings(nullptr);
}

/*
 * Checks the validity of this connection.
 */
bool LibNM::Connection::verify(GError** error) const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        return nm_connection_verify(connection, error);
    }
    return false;
}

/*
 * Sets the connection path stored by this object.
 */
void LibNM::Connection::setPath(const char* path)
{
    ASSERT_CORRECT_CONTEXT;
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        nm_connection_set_path(connection, path);
    }
}

/*
 * Gets the connection path stored by this object.
 */
const char* LibNM::Connection::getPath() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* path = "";
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        path = nm_connection_get_path(connection);
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}

/*
 * Gets a unique ID string for this connection.
 */
const char* LibNM::Connection::getUUID() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* uuid = "";
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        uuid = nm_connection_get_uuid(connection);
        if(uuid == nullptr)
        {
            uuid = "";
        }
    }
    return uuid;
}

/*
 * Gets the connection's NetworkManager ID string.
 */
const char* LibNM::Connection::getID() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* conId = "";
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        conId = nm_connection_get_id(connection);
        if(conId == nullptr)
        {
            conId = "";
        }
    }
    return conId;
}


#ifdef JUCE_DEBUG
/*
 * Prints all stored connection data to stdout.
 */
void LibNM::Connection::printDebugOutput() const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        nm_connection_dump(connection);
    }
}
#endif


/*
 * Gets one of this connection's setting objects.
 */
NMSetting* LibNM::Connection::getSetting(GType settingType) const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        return nm_connection_get_setting(connection, settingType);
    }
    return nullptr;
}
