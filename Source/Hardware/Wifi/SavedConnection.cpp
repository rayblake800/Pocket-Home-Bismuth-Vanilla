#include "SavedConnection.h"

#define BUS_NAME "org.freedesktop.NetworkManager"
#define INTERFACE "org.freedesktop.NetworkManager.Settings.Connection"


SavedConnection::SavedConnection(const char * path) :
GDBusProxyInterface(BUS_NAME, path, INTERFACE)
{
    GVariant* settings = callMethod("GetSettings");
    GVariant* secrets = callMethod("GetSecrets");
    
    std::cout << "\nConnection " << path << " Settings:\n" 
            << GVariantConverter::toString(settings) 
            << "\nSecrets:\n"
            << GVariantConverter::toString(secrets) << "\n";
    if(settings != nullptr)
    {
        g_variant_unref(settings);
    }
    if(secrets != nullptr)
    {
        g_variant_unref(secrets);
    }
}
