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

// Connects to NetworkManager over DBus to initialize the saved connection list.
NMDBus::SavedConnectionLoader::SavedConnectionLoader() :
GLib::DBus::Proxy(busName, path, interface)
{
    connectionPaths = loadConnectionPaths();
    for (const juce::String& path : connectionPaths)
    {
        connectionList.add(SavedConnection(path.toRawUTF8()));
    }
    DBG("SavedConnectionLoader::" << __func__  << ": Found "
            << connectionList.size() << " connections.");
}


// Reads all connection paths from NetworkManager, and returns all the wifi
// connections as SavedConnection objects.
juce::Array<NMDBus::SavedConnection>
NMDBus::SavedConnectionLoader::getWifiConnections() const
{
    juce::Array<SavedConnection> connections;
    for (const SavedConnection& con : connectionList)
    {
        if (con.isWifiConnection())
        {
            connections.add(con);
        }
    }
    return connections;
}


// Checks saved connection paths to see if one exists at the given path.
bool NMDBus::SavedConnectionLoader::connectionExists
(const juce::String& connectionPath) const
{
    return connectionPaths.contains(connectionPath);
}


// Finds a saved connection from its DBus path.
NMDBus::SavedConnection NMDBus::SavedConnectionLoader::getConnection
(const juce::String& connectionPath)
{
    if (!connectionExists(connectionPath))
    {
        updateSavedConnections();
    }
    if (connectionExists(connectionPath))
    {
        for (const SavedConnection& connection : connectionList)
        {
            if (connection.getPath() == connectionPath)
            {
                return connection;
            }
        }
    }
    SavedConnection emptyConnection;
    return emptyConnection;
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


// Checks the list of saved connections against an updated connection path
// list, adding any new connections and removing any deleted connections.
void NMDBus::SavedConnectionLoader::updateSavedConnections()
{
    connectionPaths = loadConnectionPaths();
    for (int i = 0; i < connectionList.size(); i++)
    {
        const SavedConnection& saved = connectionList[i];
        const juce::String savedPath = saved.getPath();
        if (savedPath.isEmpty() || saved.isNull() 
                || ! connectionPaths.contains(savedPath))
        {
            connectionList.remove(i);
            i--;
        }
        else
        {
            connectionPaths.removeString(savedPath);
        }
    }
    for (const juce::String& path : connectionPaths)
    {
        SavedConnection newSaved(path.toRawUTF8());
        if (! newSaved.isNull())
        {
            connectionList.add(newSaved);
        }
    }
}
