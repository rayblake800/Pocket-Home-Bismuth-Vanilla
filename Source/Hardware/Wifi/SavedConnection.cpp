#include "SavedConnection.h"

#define BUS_NAME "org.freedesktop.NetworkManager"
#define INTERFACE "org.freedesktop.NetworkManager.Settings.Connection"


SavedConnection::SavedConnection(const char * path) :
GDBusProxyInterface(BUS_NAME, path, INTERFACE)
{
    GVariant* settings = callMethod("GetSettings");
    if(settings != nullptr)
    {
        StringArray settingNames;
        GVariantConverter::iterateArray(settings,[&settingNames]
                (GVariant* arrayVal)
        {
            settingNames.addArray(GVariantConverter::getKeys(arrayVal));
        });
        GVariant* emptyStr = GVariantConverter::getVariant<String>(String());
        GVariant* secrets = callMethod("GetSecrets",
                g_variant_new_tuple(&emptyStr,1));

        std::cout << "\nConnection " << path << " Settings:\n" 
                << GVariantConverter::toString(settings) << "\n";
        g_variant_unref(settings);
        settings = nullptr;
            
        for(const String& setting : settingNames)
        {
            GVariant* name = GVariantConverter::getVariant<String>(setting);
            GVariant* secrets = callMethod("GetSecrets", 
                    g_variant_new_tuple(&name,1));
            if(secrets != nullptr)
            {
                std::cout << "Secrets for setting " << setting << ":\n"
                        << GVariantConverter::toString(secrets) << "\n";
                g_variant_unref(secrets);
                secrets = nullptr;
            }
        }
    }
}
