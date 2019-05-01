#include "Wifi_LibNM_DBus_SavedConnectionLoader.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_APHash.h"

// The NetworkManager's DBus path:
const constexpr char* busName = "org.freedesktop.NetworkManager";
// SavedConnectionLoader's DBus interface name:
const constexpr char* interface = "org.freedesktop.NetworkManager.Settings";
// SavedConnectionLoader's DBus interface path:
const constexpr char* path = "/org/freedesktop/NetworkManager/Settings";

// DBus listConnections function key:
const constexpr char* listConnectionFunction = "ListConnections";

namespace NMDBus = Wifi::LibNM::DBus;

// Connects to NetworkManager over DBus.
NMDBus::SavedConnectionLoader::SavedConnectionLoader() :
GLib::DBus::Proxy(busName, path, interface) { }


// Reads all connection paths from NetworkManager, and returns all the wifi
// connections as SavedConnection objects.
juce::Array<NMDBus::SavedConnection>
NMDBus::SavedConnectionLoader::getWifiConnections() const
{
    juce::Array<SavedConnection> connections;
    juce::StringArray connectionPaths(std::move(loadConnectionPaths()));
    for (const juce::String& path : connectionPaths)
    {
        SavedConnection connection(path.toRawUTF8());
        if (connection.isWifiConnection())
        {
            connections.add(connection);
        }
    }
    return connections;
}


// Checks saved connection paths to see if one exists at the given path.
bool NMDBus::SavedConnectionLoader::connectionExists
(const juce::String& connectionPath) const
{
    SavedConnection testConnection(connectionPath.toRawUTF8());
    return ! testConnection.isNull();
}


// Finds a saved connection from its DBus path.
NMDBus::SavedConnection NMDBus::SavedConnectionLoader::getConnection
(const juce::String& connectionPath)
{
    return SavedConnection(connectionPath.toRawUTF8());
}


// Finds all saved connections that are compatible with a given wifi access
// point.
juce::Array<NMDBus::SavedConnection>
NMDBus::SavedConnectionLoader::findConnectionsForAP
(const LibNM::AccessPoint& accessPoint)
{
    using juce::Array;
    Array<SavedConnection> compatible;
    if (!isNull() && !accessPoint.isNull())
    {
        Array<SavedConnection> wifiCons = getWifiConnections();
        for (SavedConnection& con : wifiCons)
        {
            if (con.getNMConnection().isCompatibleAccessPoint(accessPoint))
            {
                compatible.add(con);
            }
        }
    }
    return compatible;
}


// Reads the list of all available connection paths.
inline juce::StringArray
NMDBus::SavedConnectionLoader::loadConnectionPaths() const
{
    using namespace GLib::VariantConverter;
    using juce::StringArray;
    GVariant* pathVar = callFunction(listConnectionFunction);
    if (pathVar != nullptr)
    {
        StringArray paths = getValue<StringArray>(pathVar);
        g_variant_unref(pathVar);
        pathVar = nullptr;
        return paths;
    }
    return StringArray();
}


// Checks if a saved connection exists that is compatible with an access point.
bool NMDBus::SavedConnectionLoader::matchingConnectionExists
(const AccessPoint& accessPoint) const
{
    if (!accessPoint.isNull())
    {
        juce::Array<SavedConnection> wifiConnections = getWifiConnections();
        for (SavedConnection& con : wifiConnections)
        {
            if (con.getNMConnection().isCompatibleAccessPoint(accessPoint))
            {
                return true;
            }
        }
    }
    return false;
}
