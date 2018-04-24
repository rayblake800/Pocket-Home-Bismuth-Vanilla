#include "SavedConnections.h"


#define BUS_NAME "org.freedesktop.NetworkManager"
#define PATH "/org/freedesktop/NetworkManager/Settings"
#define INTERFACE "org.freedesktop.NetworkManager.Settings"

SavedConnections::SavedConnections() :
GDBusProxyInterface(BUS_NAME, PATH, INTERFACE) { }

/*
 * Reads all connection paths from NetworkManager, and returns all the wifi
 * connections as SavedConnection objects.
 */
Array<SavedConnection> SavedConnections::getWifiConnections()
{
    using namespace GVariantConverter;
    Array<SavedConnection> connections;
    GVariant* conArrayVar = callMethod("ListConnections");
    if(conArrayVar != nullptr)
    {
        StringArray paths = getValue<StringArray>(conArrayVar);
        for(const String& path : paths)
        {
            SavedConnection con(path.toRawUTF8());
            if(con.isWifiConnection())
            {
                connections.add(con);
            }
            
        }
        g_variant_unref(conArrayVar);
        conArrayVar = nullptr;
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
bool connectionExists(const String& connectionPath)
{
    using namespace GVariantConverter;
    Array<SavedConnection> connections;
    GVariant* conArrayVar = callMethod("ListConnections");
    if(conArrayVar != nullptr)
    {
        StringArray paths = getValue<StringArray>(conArrayVar);
        g_variant_unref(conArrayVar);
        conArrayVar = nullptr;
        return paths.contains(connectionPath);
    }
    return false;
}

