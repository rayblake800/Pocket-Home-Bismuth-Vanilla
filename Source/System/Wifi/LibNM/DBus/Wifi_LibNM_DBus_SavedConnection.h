#pragma once
/**
 * @file  Wifi_LibNM_DBus_SavedConnection.h
 *
 * @brief  Controls a saved network connection stored remotely by
 *         NetworkManager.
 */

#include "GLib_DBus_Proxy.h"
#include "Wifi_LibNM_Connection.h"
#include "JuceHeader.h"

namespace Wifi { namespace LibNM { namespace DBus {
        class SavedConnection; } } }

/**
 * @brief  A D-Bus proxy wrapper used to access a single saved connection held
 *         by NetworkManager.
 *
 *  All saved network connections visible to NetworkManager on the system
 * running this application may be loaded as SavedConnection objects.
 * SavedConnection objects are primarily meant to handle Wifi connections, and
 * support for other connection types is incomplete.
 *
 *  If the saved connection is a valid Wifi connection, SavedConnection can
 * create a Connection object that may be used with the LibNM::Client to
 * activate the connection if a compatible access point is visible.
 *
 *  SavedConnection may be used to delete its connection from NetworkManager.
 * This will affect other connection applications using NetworkManager, and it
 * cannot be undone.
 */
class Wifi::LibNM::DBus::SavedConnection : public GLib::DBus::Proxy
{
public:
    /**
     * @brief  Creates an empty object with no linked connection.
     */
    SavedConnection();

    /**
     * @brief  Initializes a SavedConnection from a DBus connection path.
     *
     * @param path  A valid DBus path to a NetworkManager saved connection.
     */
    SavedConnection(const char* path);

    virtual ~SavedConnection() { }

    /**
     * @brief  Gets the connection's DBus path.
     *
     * @return  The DBus path used to create this object, or the empty string
     *          if the connection is not valid.
     */
    const juce::String& getPath() const;

    /**
     * @brief  Checks if this connection is a wifi connection.
     *
     * @return  Whether the saved connection has wifi connection settings.
     */
    bool isWifiConnection() const;

    /**
     * @brief  Gets the Connection object generated from this connection's data.
     *
     *  Only Wifi connections are supported, other connection types will result
     * in incomplete Connection objects.
     *
     * @return  The Connection object for this connection, or nullptr if the
     *          connection is invalid.
     */
    Connection getNMConnection();

    /**
     * @brief  Gets the last recorded time this saved connection was active.
     *
     * @return  The last time the connection was active, or the Unix epoch if
     *          the connection has no saved connection time.
     */
    juce::Time lastConnectionTime() const;

    /**
     * @brief  Checks if the connection has a saved wireless security key.
     *
     * @return  Whether a security key value was found in this connection's
     *          settings.
     */
    bool hasSavedKey() const;

    /**
     * @brief  Deletes this connection from the list of saved connections.
     *
     * This object will be null after this method is called.
     */
    void deleteConnection();

    /**
     * @brief  Compares SavedConnections using their connection paths.
     *
     * @param rhs  A connection to compare with this object.
     *
     * @return     Whether this connection and rhs share a DBus path.
     */
    bool operator== (const SavedConnection& rhs) const;

    /**
     * @brief  Compares SavedConnections with NMConnections using their
     *         connection paths.
     *
     * @param rhs  A connection to compare with this object.
     *
     * @return     Whether this connection and rhs share a DBus path.
     */
    bool operator== (NMConnection* rhs) const;

private:
    /**
     * @brief  Returns one of this connection's settings objects.
     *
     * @param name  The name of the connection setting to find.
     *
     * @return      The setting with the matching name, if found. If this value
     *              is non-null, it will need to be freed with g_variant_unref.
     */
    GVariant* getSetting(const char* name) const;

    /**
     * @brief  Returns the value of a specific settings object property.
     *
     * @param settingName  The name of the connection setting to search.
     *
     * @param propName     The name of the property to search for in the
     *                     settings object.
     *
     * @return             The matching property, if found. If this value is
     *                     non-null, it will need to be freed with
     *                     g_variant_unref.
     */
    GVariant* getSettingProp(const char* settingName, const char* propName)
        const;

    /**
     * @brief  Returns the value of a specific settings object property.
     *
     * @param settingsObject  A settings object extracted from this connection.
     *
     * @param propName        The name of the property to search for in the
     *                        settings object.
     *
     * @return                The matching property, if found. If this value is
     *                        non-null, it will need to be freed with
     *                        g_variant_unref.
     */
    GVariant* getSettingProp(GVariant* settingsObject, const char* propName)
        const;

    /**
     * @brief  Checks if this connection has a particular setting type.
     *
     * @param settingName  The name of a connection settings object to find.
     *
     * @return             Whether a settings object named settingName exists.
     */
    bool hasSetting(const char* settingName) const;

    /**
     * @brief  Checks if this connection has a specific setting type, and if
     *         that settings object as a specific property.
     *
     *  Don't use this if you actually need any data from the setting, in that
     * case it's more effective to just get the setting object and check if
     * getSettingParam returns null.
     *
     * @param settingName  The name of a connection settings object to find.
     *
     * @param propName     The name of a settings property to check for.
     *
     * @return             Whether a settings object named settingName exists
     *                     and contains a property called propName.
     */
    bool hasSettingProperty(const char* settingName, const char* propName)
        const;

    // The SavedConnection's DBus path:
    juce::String path;
};
