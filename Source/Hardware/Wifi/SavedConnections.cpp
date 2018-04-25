#include "SavedConnections.h"


#define BUS_NAME "org.freedesktop.NetworkManager"
#define PATH "/org/freedesktop/NetworkManager/Settings"
#define INTERFACE "org.freedesktop.NetworkManager.Settings"

//methods
#define LIST_CONNECTIONS "ListConnections"

SavedConnections::SavedConnections() :
GDBusProxyInterface(BUS_NAME, PATH, INTERFACE) 
{ 
    StringArray paths = getConnectionPaths();
    for(const String& path : paths)
    {
        connectionList.add(SavedConnection(path.toRawUTF8()));
    }
}

/*
 * Reads all connection paths from NetworkManager, and returns all the wifi
 * connections as SavedConnection objects.
 */
Array<SavedConnection> SavedConnections::getWifiConnections()
{
    updateSavedConnections();
    Array<SavedConnection> connections;
    for(const SavedConnection& con : connectionList)
    {
        if(con.isWifiConnection())
        {
            connections.add(con);
        }
    }
    return connections;
}

    
/**
 * Checks saved connection paths to see if one exists at the given path.
 * 
 * @param connectionPath  A DBus path to check for a connection.
 * 
 * @return  true iff connectionPath is a valid path to a saved connection. 
 */
bool SavedConnections::connectionExists(const String& connectionPath)
{
    using namespace GVariantConverter;
    StringArray paths = getConnectionPaths();
    return paths.contains(connectionPath);
}
    
/**
 * Get the list of all available connection paths
 * 
 * @return the list of paths, freshly updated over the DBus interface.
 */
inline StringArray SavedConnections::getConnectionPaths()
{
    using namespace GVariantConverter;
    GVariant* conArrayVar = callMethod(LIST_CONNECTIONS);
    if(conArrayVar != nullptr)
    {
        StringArray paths = getValue<StringArray>(conArrayVar);
        g_variant_unref(conArrayVar);
        conArrayVar = nullptr;
        return paths;
    }
    return StringArray();
}

/**
 * Check the list of saved connections against an updated connection path
 * list, adding any new connections and removing any deleted connections.
 */
void SavedConnections::updateSavedConnections()
{
    StringArray paths = getConnectionPaths();
    Array<SavedConnection> toRemove;
    for(const SavedConnection& saved : connectionList)
    {
        if(!paths.contains(saved.getPath()))
        {
            toRemove.add(saved);
        }
        else
        {
            paths.removeString(saved.getPath());
        }
    }
    for(const SavedConnection& removing : toRemove)
    {
        connectionList.removeAllInstancesOf(removing);
    }
    for(const String& path : paths)
    {
        connectionList.add(SavedConnection(path.toRawUTF8()));
    }
}

