#include "LibNM/OwnedObjects/Settings/ConnectionSettings.h"
#include "LibNM/ContextTest.h"
#include "GLib_ObjectPtr.h"

/*
 * Creates a ConnectionSettings object that shares data with another
 * ConnectionSettings object.
 */
LibNM::ConnectionSettings::ConnectionSettings
(const ConnectionSettings& toCopy) : Settings(toCopy) 
{
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a ConnectionSettings object to hold existing LibNM connection
 * settings.
 */
LibNM::ConnectionSettings::ConnectionSettings(NMSettingConnection* toAssign) :
Settings(NM_SETTING(toAssign), NM_TYPE_SETTING_CONNECTION) 
{
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a null ConnectionSettings object holding no data.
 */
LibNM::ConnectionSettings::ConnectionSettings() : 
Settings(NM_TYPE_SETTING_CONNECTION) 
{
    ASSERT_NM_CONTEXT;
}

/**
 * @brief  Casts a GLib::ObjectPtr holding the connection settings data to 
 *         the NMSettingsConnection* type.
 *
 * @param settingsPtr  A RAII pointer object storing the settings object's 
 *                     GObject* value.
 *
 * @return             The value stored in settingsPtr as a 
 *                     NMSettingsConnection*, or nullptr if settingsPtr does not 
 *                     hold a NMSettingsConnection*.
 */
static NMSettingConnection* toNMSettingConnection(GLib::ObjectPtr& settingPtr)
{
    return NM_SETTING_CONNECTION((GObject*) settingPtr);
}

/*
 * Gets the basic ID used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getID() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        return juce::String(nm_setting_connection_get_id
                (toNMSettingConnection(settingsPtr)));
    }
    return juce::String();
}

/*
 * Gets the UUID used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getUUID() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        return juce::String(nm_setting_connection_get_uuid(
                    toNMSettingConnection(settingsPtr)));
    }
    return juce::String();
}

/*
 * Gets the network interface used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getConnectionInterface() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        return juce::String(nm_setting_connection_get_interface_name(
                    toNMSettingConnection(settingsPtr)));
    }
    return juce::String();
}

/*
 * Gets the last timestamp saved for this object's connection.
 */
juce::Time LibNM::ConnectionSettings::getTimestamp() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        return juce::Time(1000 * nm_setting_connection_get_timestamp(
                    toNMSettingConnection(settingsPtr)));
    }
    return juce::Time();
}
