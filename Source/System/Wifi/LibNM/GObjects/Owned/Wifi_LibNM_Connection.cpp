#include "Wifi_LibNM_Connection.h"
#include "Wifi_LibNM_Settings_Connection.h"
#include "Wifi_LibNM_Settings_Wireless.h"
#include "Wifi_LibNM_Settings_WirelessSecurity.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_APHash.h"
#include "Wifi_LibNM_SSID.h"
#include "Wifi_LibNM_APMode.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Wifi_LibNM_ContextTest.h"
#include "GLib_ObjectPtr.h"

// Creates a Connection to contain a NMConnection object.
Wifi::LibNM::Connection::Connection(NMConnection* toAssign) :
GLib::Owned::Object(G_OBJECT(toAssign), NM_TYPE_CONNECTION) { }


// Creates a null Connection.
Wifi::LibNM::Connection::Connection() :
        GLib::Owned::Object(NM_TYPE_CONNECTION) { }

/**
 * @brief  Casts a smart pointer holding a Connection's object data to the
 *         LibNM connection type.
 *
 * @param connectionPtr  A smart pointer holding an NMConnection* value.
 *
 * @return               The object data pointer cast to the NMConnection*
 *                       type, or nullptr if connectionPtr did not hold a valid
 *                       NMConnection*.
 */
static NMConnection* toNMConnection(GLib::ObjectPtr& connectionPtr)
{
    return NM_CONNECTION((GObject*) connectionPtr);
}


// Checks if this connection object and another could be describing the same
// network connection.
bool Wifi::LibNM::Connection::connectionMatches(const Connection& rhs) const
{
    GLib::ObjectPtr thisData(*this);
    GLib::ObjectPtr rhsData(rhs);
    if (thisData == rhsData)
    {
        return true;
    }
    else if (thisData == nullptr || rhsData == nullptr)
    {
        return false;
    }
    return nm_connection_compare(
            toNMConnection(thisData),
            toNMConnection(rhsData),
            NM_SETTING_COMPARE_FLAG_FUZZY);
}


// Checks if this connection could potentially be activated with an access
// point.
bool Wifi::LibNM::Connection::isCompatibleAccessPoint
(const AccessPoint& accessPoint) const
{
    GLib::ObjectPtr connectionPtr(*this);
    GLib::ObjectPtr accessPointPtr(accessPoint);
    if (connectionPtr == nullptr || accessPointPtr == nullptr)
    {
        return false;
    }
    return nm_access_point_connection_valid(
            NM_ACCESS_POINT((GObject*) accessPointPtr),
            toNMConnection(connectionPtr));
}


// Adds a new set of settings to this connection, creating new connection data
// if this object is null.
void Wifi::LibNM::Connection::addSettings(Settings::Object addedSettings)
{
    if (isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    GLib::ObjectPtr connectionPtr(*this);
    jassert(connectionPtr != nullptr);

    // Don't use smart pointers for NMSettings, the NMConnection is going to
    // claim the reference that the smart pointer would clear.
    NMSetting* settingsObject = NM_SETTING(addedSettings.getGObject());
    if (settingsObject != nullptr)
    {
        nm_connection_add_setting(toNMConnection(connectionPtr),
                settingsObject);
    }
}


// Removes one of the connection settings from this connection.
void Wifi::LibNM::Connection::removeSettings(GType settingType)
{
    GLib::ObjectPtr connectionPtr(*this);
    if (connectionPtr != nullptr)
    {
        nm_connection_remove_setting(toNMConnection(connectionPtr),
                settingType);
    }
}


// Adds new wireless connection settings to this connection.
void Wifi::LibNM::Connection::addWifiSettings(SSID ssid, bool isHidden)
{
    if (ssid.getByteArray() != nullptr)
    {
        Settings::Wireless newSettings;
        newSettings.setSSID(ssid);
        newSettings.setHidden(isHidden);
        addSettings(newSettings);
    }
}


// Attempts to add WPA security settings to this connection.
bool Wifi::LibNM::Connection::addWPASettings(const juce::String& psk)
{
    Settings::WirelessSecurity newSettings;
    bool wpaSet = newSettings.addWPASettings(psk);
    if (wpaSet)
    {
        addSettings(newSettings);
    }
    return wpaSet;
}


// Attempts to add WEP security settings to this connection.
bool Wifi::LibNM::Connection::addWEPSettings(const juce::String& psk)
{
    Settings::WirelessSecurity newSettings;
    bool wepSet = newSettings.addWEPSettings(psk);
    if (wepSet)
    {
        addSettings(newSettings);
    }
    return wepSet;
}


// Checks the validity of this connection.
bool Wifi::LibNM::Connection::verify(GError** error) const
{
    GLib::ObjectPtr connectionPtr(*this);
    if (connectionPtr != nullptr)
    {
        return nm_connection_verify(toNMConnection(connectionPtr), error);
    }
    return false;
}


// Sets the connection path stored by this object.
void Wifi::LibNM::Connection::setPath(const char* path)
{
    if (isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    GLib::ObjectPtr connectionPtr(*this);
    if (connectionPtr != nullptr)
    {
        nm_connection_set_path(toNMConnection(connectionPtr), path);
    }
}


// Gets the connection path stored by this object.
const char* Wifi::LibNM::Connection::getPath() const
{
    const char* path = "";
    GLib::ObjectPtr connectionPtr(*this);
    if (connectionPtr != nullptr)
    {
        path = nm_connection_get_path(toNMConnection(connectionPtr));
        if (path != nullptr)
        {
            return path;
        }
    }
    return "";
}


// Gets a unique ID string for this connection.
const char* Wifi::LibNM::Connection::getUUID() const
{
    GLib::ObjectPtr connectionPtr(*this);
    if (connectionPtr != nullptr)
    {
        const char* uuid
                = nm_connection_get_uuid(toNMConnection(connectionPtr));
        if (uuid != nullptr)
        {
            return uuid;
        }
    }
    return "";
}


// Gets the connection's NetworkManager ID string.
const char* Wifi::LibNM::Connection::getID() const
{
    GLib::ObjectPtr connectionPtr(*this);
    if (connectionPtr != nullptr)
    {
        const char* id = nm_connection_get_id(toNMConnection(connectionPtr));
        if (id != nullptr)
        {
            return id;
        }
    }
    return "";
}

#ifdef JUCE_DEBUG
// Prints all stored connection data to stdout.
void Wifi::LibNM::Connection::printDebugOutput() const
{
    GLib::ObjectPtr connectionPtr(*this);
    if (connectionPtr != nullptr)
    {
        nm_connection_dump(toNMConnection(connectionPtr));
    }
}
#endif

