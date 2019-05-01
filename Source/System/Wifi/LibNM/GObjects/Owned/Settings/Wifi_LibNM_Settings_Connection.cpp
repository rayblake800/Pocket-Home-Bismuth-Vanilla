#include "Wifi_LibNM_Settings_Connection.h"
#include "GLib_ObjectPtr.h"

namespace WifiNMSettings = Wifi::LibNM::Settings;

// Creates a Connection object to hold existing LibNM connection settings.
WifiNMSettings::Connection::Connection(NMSettingConnection* toAssign) :
Object(NM_SETTING(toAssign), NM_TYPE_SETTING_CONNECTION) { }


// Creates a null Connection object holding no data.
WifiNMSettings::Connection::Connection() :
Object(NM_TYPE_SETTING_CONNECTION) { }

/**
 * @brief  Casts a GLib::ObjectPtr holding the connection settings data to the
 *         NMSettingsConnection* type.
 *
 * @param settingsPtr  A RAII pointer object storing the settings object's
 *                     GObject* value.
 *
 * @return             The value stored in settingsPtr as a
 *                     NMSettingsConnection*, or nullptr if settingsPtr does
 *                     not hold a NMSettingsConnection*.
 */
static NMSettingConnection* toNMSettingConnection(GLib::ObjectPtr& settingPtr)
{
    return NM_SETTING_CONNECTION((GObject *) settingPtr);
}


// Gets the basic ID used by this object's connection.
juce::String WifiNMSettings::Connection::getID() const
{
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        return juce::String(nm_setting_connection_get_id
                (toNMSettingConnection(settingsPtr)));
    }
    return juce::String();
}


// Gets the UUID used by this object's connection.
juce::String WifiNMSettings::Connection::getUUID() const
{
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        return juce::String(nm_setting_connection_get_uuid(
                    toNMSettingConnection(settingsPtr)));
    }
    return juce::String();
}


// Gets the network interface used by this object's connection.
juce::String WifiNMSettings::Connection::getConnectionInterface() const
{
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        return juce::String(nm_setting_connection_get_interface_name(
                    toNMSettingConnection(settingsPtr)));
    }
    return juce::String();
}


// Gets the last timestamp saved for this object's connection.
juce::Time WifiNMSettings::Connection::getTimestamp() const
{
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        return juce::Time(1000 * nm_setting_connection_get_timestamp(
                    toNMSettingConnection(settingsPtr)));
    }
    return juce::Time();
}
