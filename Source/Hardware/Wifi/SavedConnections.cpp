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
    Array<SavedConnection> connections;
    GVariant* conArrayVar = callMethod("ListConnections");
    if(conArrayVar != nullptr)
    {
        gsize length = 0;
        const gchar** paths = g_variant_get_objv(conArrayVar, &length);
        if(paths != nullptr)
        {
            for(int i = 0; i < length; i++)
            {
                SavedConnection con(paths[i]);
                if(con.isWifiConnection())
                {
                    connections.add(SavedConnection(paths[i]));
                }
            }
            g_free(paths);
            paths = nullptr;
        }
        g_variant_unref(conArrayVar);
        conArrayVar = nullptr;
    }
    return connections;
}

