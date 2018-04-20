#include "SavedConnection.h"

#define BUS_NAME "org.freedesktop.NetworkManager"
#define INTERFACE "org.freedesktop.NetworkManager.Settings.Connection"


SavedConnection::SavedConnection(const char * path) :
GDBusProxyInterface(BUS_NAME, path, INTERFACE)
{
    GVariant* retval = callMethod("GetSettings");
    std::cout << "\nConnection " << path << ":" 
            << GVariantConverter::toString(retval) << "\n";
}