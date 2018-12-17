#include "LibNM/NMObjects/ConnectionSettings.h"

/*
 * Creates a ConnectionSettings object that shares data with another
 * ConnectionSettings object.
 */
LibNM::ConnectionSettings::ConnectionSettings(const ConnectionSettings& toCopy)
{
}

/*
 * Creates a ConnectionSettings object to hold existing LibNM connection
 * settings.
 */
LibNM::ConnectionSettings::ConnectionSettings(NMSettingConnection* toAssign)
{
}

/*
 * Creates a null ConnectionSettings object holding no data.
 */
LibNM::ConnectionSettings::ConnectionSettings()
{
}

/*
 * Gets the basic ID used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getID() const
{
}

/*
 * Gets the UUID used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getUUID() const
{
}

/*
 * Gets the network interface used by this object's connection.
 */
juce::String LibNM::ConnectionSettings::getConnectionInterface() const
{
}

/*
 * Gets the last timestamp saved for this object's connection.
 */
juce::Time LibNM::ConnectionSettings::getTimestamp() const
{
}