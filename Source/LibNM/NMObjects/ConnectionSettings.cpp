#include "LibNM/NMObjects/ConnectionSettings.h"
#include "GLib/SmartPointers/ObjectPtr.h"

typedef GLib::ObjectPtr<NMSettingConnection*> NMSettingConnectionPtr;

/*
 * Creates a ConnectionSettings object that shares data with another
 * ConnectionSettings object.
 */
LibNM::ConnectionSettings::ConnectionSettings
(const ConnectionSettings& toCopy) : Settings(toCopy) 
{
    ASSERT_CORRECT_CONTEXT;
}

/*
 * Creates a ConnectionSettings object to hold existing LibNM connection
 * settings.
 */
LibNM::ConnectionSettings::ConnectionSettings(NMSettingConnection* toAssign) :
Settings(NM_SETTING(toAssign), NM_TYPE_SETTING_CONNECTION) 
{
    ASSERT_CORRECT_CONTEXT;
}

/*
 * Creates a null ConnectionSettings object holding no data.
 */
LibNM::ConnectionSettings::ConnectionSettings() : 
Settings(NM_TYPE_SETTING_CONNECTION) 
{
    ASSERT_CORRECT_CONTEXT;
}

/*
 * Gets the basic ID used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getID() const
{
    ASSERT_CORRECT_CONTEXT;
    NMSettingConnectionPtr settings = NM_SETTING_CONNECTION(getGObject());
    if(settings != nullptr)
    {
        return juce::String(nm_setting_connection_get_id(settings));
    }
    return juce::String();
}

/*
 * Gets the UUID used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getUUID() const
{
    ASSERT_CORRECT_CONTEXT;
    NMSettingConnectionPtr settings = NM_SETTING_CONNECTION(getGObject());
    if(settings != nullptr)
    {
        return juce::String(nm_setting_connection_get_uuid(settings));
    }
    return juce::String();
}

/*
 * Gets the network interface used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getConnectionInterface() const
{
    ASSERT_CORRECT_CONTEXT;
    NMSettingConnectionPtr settings = NM_SETTING_CONNECTION(getGObject());
    if(settings != nullptr)
    {
        return juce::String(nm_setting_connection_get_interface_name(settings));
    }
    return juce::String();
}

/*
 * Gets the last timestamp saved for this object's connection.
 */
juce::Time LibNM::ConnectionSettings::getTimestamp() const
{
    ASSERT_CORRECT_CONTEXT;
    NMSettingConnectionPtr settings = NM_SETTING_CONNECTION(getGObject());
    if(settings != nullptr)
    {
        return juce::Time(1000 * nm_setting_connection_get_timestamp(settings));
    }
    return juce::Time();
}
