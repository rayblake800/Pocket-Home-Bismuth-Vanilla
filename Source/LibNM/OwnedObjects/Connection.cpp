#include "LibNM/OwnedObjects/Connection.h"
#include "LibNM/OwnedObjects/Settings/ConnectionSettings.h"
#include "LibNM/OwnedObjects/Settings/WifiSettings.h"
#include "LibNM/OwnedObjects/Settings/WifiSecuritySettings.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/Data/SSID.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/*
 * Creates a Connection sharing a GObject with an existing Connection.
 */
LibNM::Connection::Connection(const Connection& toCopy) :
GLib::Owned::Object(toCopy, NM_TYPE_CONNECTION) 
{
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a Connection to contain a NMConnection object.
 */
LibNM::Connection::Connection(NMConnection* toAssign) :
GLib::Owned::Object(G_OBJECT(toAssign), NM_TYPE_CONNECTION) 
{ 
    ASSERT_NM_CONTEXT;
}
    
/*
 * Creates a null Connection.
 */
LibNM::Connection::Connection() : GLib::Owned::Object(NM_TYPE_CONNECTION) { }

/**
 * @brief  Casts a smart pointer holding a Connection's object data to the LibNM
 *         connection type.
 *
 * @param connectionPtr  A smart pointer holding an NMConnection* value.
 *
 * @return               The object data pointer cast to the NMConnection* type,
 *                       or nullptr if connectionPtr did not hold a valid
 *                       NMConnection*
 */
static NMConnection* toNMConnection(GLib::ObjectPtr& connectionPtr)
{
    return NM_CONNECTION((GObject*) connectionPtr);
}

/*
 * Checks if this connection object and another could be describing the same 
 * network connection.
 */
bool LibNM::Connection::connectionMatches(const Connection& rhs) const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr thisData(*this);
    GLib::ObjectPtr rhsData(rhs);
    if(thisData == rhsData)
    {
        return true;
    }
    else if(thisData == nullptr || rhsData == nullptr)
    {
        return false;
    }
    return nm_connection_compare(
            toNMConnection(thisData),
            toNMConnection(rhsData),
            NM_SETTING_COMPARE_FLAG_FUZZY);
}

/*
 * Checks if this connection could potentially be activated with an access 
 * point.
 */
bool LibNM::Connection::isCompatibleAccessPoint
(const AccessPoint& accessPoint) const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr connectionPtr(*this);
    GLib::ObjectPtr accessPointPtr(accessPoint);
    if(connectionPtr == nullptr || accessPointPtr == nullptr)
    {
        return false;
    }
    return nm_access_point_connection_valid(
            NM_ACCESS_POINT((GObject*) accessPointPtr), 
            toNMConnection(connectionPtr));
}

/*
 * Adds a new set of settings to this connection, creating new connection data 
 * if this object is null.
 */
void LibNM::Connection::addSettings(Settings addedSettings)
{
    ASSERT_NM_CONTEXT;
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    GLib::ObjectPtr connectionPtr(*this);
    jassert(connectionPtr != nullptr);

    // Don't use smart pointers for NMSettings, the NMConnection is going to
    // claim the reference that the smart pointer would clear.
    NMSetting* settingsObject = NM_SETTING(addedSettings.getGObject());
    if(settingsObject != nullptr)
    {
        nm_connection_add_setting(toNMConnection(connectionPtr), 
                settingsObject);
    }
}

/*
 * Removes one of the connection settings from this connection.
 */
void LibNM::Connection::removeSettings(GType settingType)
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        nm_connection_remove_setting(toNMConnection(connectionPtr), 
                settingType);
    }
}
    
/*
 * Adds new wireless connection settings to this connection.
 */
void LibNM::Connection::addWifiSettings(SSID ssid, bool isHidden)
{
    ASSERT_NM_CONTEXT;
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
    ASSERT_NM_CONTEXT;
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
    ASSERT_NM_CONTEXT;
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
    ASSERT_NM_CONTEXT;
    if(!isNull())
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
    ASSERT_NM_CONTEXT;
    if(!isNull())
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
    ASSERT_NM_CONTEXT;
    if(!isNull())
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
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        return nm_connection_verify(toNMConnection(connectionPtr), error);
    }
    return false;
}

/*
 * Sets the connection path stored by this object.
 */
void LibNM::Connection::setPath(const char* path)
{
    ASSERT_NM_CONTEXT;
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        nm_connection_set_path(toNMConnection(connectionPtr), path);
    }
}

/*
 * Gets the connection path stored by this object.
 */
const char* LibNM::Connection::getPath() const
{
    ASSERT_NM_CONTEXT;
    const char* path = "";
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        path = nm_connection_get_path(toNMConnection(connectionPtr));
        if(path != nullptr)
        {
            return path;
        }
    }
    return "";
}

/*
 * Gets a unique ID string for this connection.
 */
const char* LibNM::Connection::getUUID() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        const char* uuid 
                = nm_connection_get_uuid(toNMConnection(connectionPtr));
        if(uuid != nullptr)
        {
            return uuid;
        }
    }
    return "";
}

/*
 * Gets the connection's NetworkManager ID string.
 */
const char* LibNM::Connection::getID() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        const char* id = nm_connection_get_id(toNMConnection(connectionPtr));
        if(id != nullptr)
        {
            return id;
        }
    }
    return "";
}


#ifdef JUCE_DEBUG
/*
 * Prints all stored connection data to stdout.
 */
void LibNM::Connection::printDebugOutput() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        nm_connection_dump(toNMConnection(connectionPtr));
    }
}
#endif

/*
 * Gets one of this connection's setting objects.
 */
NMSetting* LibNM::Connection::getSetting(GType settingType) const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr connectionPtr(*this);
    if(connectionPtr != nullptr)
    {
        return nm_connection_get_setting(toNMConnection(connectionPtr),
                settingType);
    }
    return nullptr;
}
