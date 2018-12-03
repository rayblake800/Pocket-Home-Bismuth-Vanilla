#include "LibNM/ThreadResource.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"

/* Unique SharedResource object instance key: */
const juce::Identifier LibNM::ThreadResource::resourceKey 
        = "LibNM::ThreadResource";

/*
 * Initializes the NMThread with the default GLib context, and creates the Wifi 
 * device object.
 */
LibNM::ThreadResource::ThreadResource() :
GLib::ThreadResource(resourceKey, GLib::SharedContextPtr(nullptr)) 
{ 
    call([this]()
    {
        using juce::String;
        using juce::Array;
        Config::MainFile config;
        String wifiInterface = config.getConfigValue<String>
            (Config::MainKeys::wifiInterfaceKey);
        if(wifiInterface.isNotEmpty())
        {
            wifiDevice = networkClient.getWifiDeviceByIface
                (wifiInterface.toRawUTF8());
        }
        if(wifiDevice.isNull() || !wifiDevice.isManaged())
        {
            Array<LibNM::DeviceWifi> devices = networkClient.getWifiDevices();
            for(const DeviceWifi& device : devices)
            {
                if(device.isManaged())
                {
                    wifiDevice = device;
                    break;
                }
            }
        }
        if(wifiDevice.isNull())
        {
            DBG("LibNM::ThreadResource::ThreadResource: "
                    << "Couldn't find managed wifi device.");
        }
    });
}
        
/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
LibNM::Client LibNM::ThreadResource::getClient()
{
    GLib::SharedContextPtr nmContext = getContext();
    if(!g_main_context_is_owner(*nmContext))
    {
        DBG("LibNM::ThreadResource::" << __func__ << 
                ": Tried to get Client outside of the LibNM event loop!");
        jassertfalse;
        return Client(nullptr);
    }
    return networkClient;
}

/*
 * Gets the shared DeviceWifi object used to control the LibNM-managed Wifi 
 * device if called within the LibNM event loop.
 */
LibNM::DeviceWifi LibNM::ThreadResource::getWifiDevice()
{
    GLib::SharedContextPtr nmContext = getContext();
    if(!g_main_context_is_owner(*nmContext))
    {
        DBG("LibNM::ThreadResource::" << __func__ << 
                ": Tried to get DeviceWifi outside of the LibNM event loop!");
        jassertfalse;
        return DeviceWifi();
    }
    return wifiDevice;
}

