#include "SavedConnections.h"


#define BUS_NAME "org.freedesktop.NetworkManager"
#define PATH "/org/freedesktop/NetworkManager/Settings"
#define INTERFACE "org.freedesktop.NetworkManager.Settings"

//methods
#define LIST_CONNECTIONS "ListConnections"

SavedConnections::SavedConnections() :
GPPDBusProxy(BUS_NAME, PATH, INTERFACE) 
{ 
    StringArray paths = getConnectionPaths();
    for(const String& path : paths)
    {
        connectionList.add(SavedConnection(path.toRawUTF8()));
    }
    DBG("SavedConnections::" << __func__  << ": Found "
            << connectionList.size() << " connections.");
}

/*
 * Reads all connection paths from NetworkManager, and returns all the wifi
 * connections as SavedConnection objects.
 */
Array<SavedConnection> SavedConnections::getWifiConnections() const
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
    DBG("SavedConnections::" << __func__  << ": Found "
            << connections.size() << " wifi connections.");
    return connections;
}

    
/*
 * Checks saved connection paths to see if one exists at the given path.
 */
bool SavedConnections::connectionExists(const String& connectionPath) const
{
    using namespace GVariantConverter;
    StringArray paths = getConnectionPaths();
    return paths.contains(connectionPath);
}

  
/*
 * Finds all saved connections that are compatible with a given wifi
 * access point.
 */
Array<SavedConnection> SavedConnections::findConnectionsForAP
(const NMPPAccessPoint& accessPoint) const
{
    Array<SavedConnection> compatible;
    if(!isNull && !accessPoint.isNull())
    {
        Array<SavedConnection> wifiCons = getWifiConnections();
        for(const SavedConnection& con : wifiCons)
        {
            if(accessPoint.isValidConnection(con.getNMConnection()))
            {
                compatible.add(con);
            }
        }
    }
    return compatible;
}
    
/*
 * Get the list of all available connection paths
 */
inline StringArray SavedConnections::getConnectionPaths() const
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

/*
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

