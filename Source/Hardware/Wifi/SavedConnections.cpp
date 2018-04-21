#include "SavedConnections.h"


#define BUS_NAME "org.freedesktop.NetworkManager"
#define PATH "/org/freedesktop/NetworkManager/Settings"
#define INTERFACE "org.freedesktop.NetworkManager.Settings"

SavedConnections::SavedConnections() :
GDBusProxyInterface(BUS_NAME, PATH, INTERFACE) { }


Array<SavedConnection> SavedConnections::getConnections()
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
                connections.add(SavedConnection(paths[i]));
            }
            g_free(paths);
            paths = nullptr;
        }
        g_variant_unref(conArrayVar);
        conArrayVar = nullptr;
    }
    return connections;
}

