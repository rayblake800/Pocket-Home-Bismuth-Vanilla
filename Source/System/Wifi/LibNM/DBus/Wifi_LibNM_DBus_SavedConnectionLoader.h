#pragma once
/**
 * @file  Wifi_LibNM_DBus_SavedConnectionLoader.h
 *
 * @brief  Connects to NetworkManager to read saved network connections.
 */

#include "GLib_DBus_Proxy.h"
#include "Wifi_LibNM_DBus_SavedConnection.h"

namespace Wifi { namespace LibNM
{
    namespace DBus { class SavedConnectionLoader; }
    class AccessPoint;
    class APHash;
} }

/**
 * @brief  A D-Bus proxy object that loads NetworkManager's saved connection
 *         data.
 *
 *  SavedConnectionLoader reads all saved network connections from
 * NetworkManager over DBus, creating SavedConnection objects for each Wifi
 * connection it finds. It then caches the SavedConnections, updating and
 * sharing them on request.
 *
 *  SavedConnectionLoader may be used to request all saved Wifi connections, or
 * a single saved connection specified by DBus path. It can also check the
 * validity of connection DBus paths, and find all saved connections compatible
 * with a LibNM::AccessPoint object.
 */
class Wifi::LibNM::DBus::SavedConnectionLoader : public GLib::DBus::Proxy
{
public:
    /**
     * @brief  Connects to NetworkManager over DBus to initialize the saved
     *         connection list.
     */
    SavedConnectionLoader();

    virtual ~SavedConnectionLoader() { }

    /**
     * @brief  Reads all connection paths from NetworkManager, and returns all
     *         the wifi connections as SavedConnection objects.
     *
     * @return  All saved wifi connections.
     */
    juce::Array<SavedConnection> getWifiConnections() const;

    /**
     * @brief  Checks saved connection paths to see if one exists at the given
     *         path.
     *
     * @param connectionPath  A DBus path to check for a connection.
     *
     * @return                Whether connectionPath is a valid path to a saved
     *                        connection.
     */
    bool connectionExists(const juce::String& connectionPath) const;

    /**
     * @brief  Finds a saved connection from its DBus path.
     *
     *  If no matching connection is already loaded, the saved connection list
     * will be updated in case the requested connection was recently added.
     *
     * @param connectionPath  The DBus path of a saved network connection.
     *
     * @return                The connection at the given path, or a null
     *                        object if no connection was found.
     */
    SavedConnection getConnection(const juce::String& connectionPath);

    /**
     * @brief  Finds all saved connections that are compatible with a given
     *         Wifi access point.
     *
     * @param accessPoint  A wifi access point to check against the list of
     *                     saved connections.
     *
     * @return             The list of all saved connections that could be
     *                     activated with the given access point.
     */
    juce::Array<SavedConnection> findConnectionsForAP
    (const AccessPoint& accessPoint);

    /**
     * @brief  Checks if a saved connection exists that is compatible with a
     *         particular access point.
     *
     * @param accessPoint  A wifi access point to check against the list of
     *                     saved connections.
     *
     * @return             Whether a compatible SavedConnection object exists.
     */
    bool matchingConnectionExists(const AccessPoint& accessPoint) const;

    /**
     * @brief  Checks the list of saved connections against an updated
     *         connection path list, adding any new connections and removing
     *         any deleted connections.
     */
    void updateSavedConnections();

private:
    /**
     * @brief  Reads the list of all available connection paths.
     *
     * @return  The list of paths, freshly updated over the DBus interface.
     */
    inline juce::StringArray loadConnectionPaths() const;

    // All loaded saved connections
    juce::Array<SavedConnection> connectionList;

    // Paths to all saved connections, in the same order as connectionList
    juce::StringArray connectionPaths;
};
