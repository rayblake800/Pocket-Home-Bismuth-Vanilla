#include "LibNM/NMThread.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"

/* Unique SharedResource object instance key: */
const juce::Identifier LibNM::NMThread::resourceKey = "LibNM::NMThread";

/*
 * Initializes the NMThread with the default GLib context, and creates the Wifi 
 * device object.
 */
LibNM::NMThread::NMThread() :
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
            DBG("LibNM::NMThread::NMThread: "
                    << "Couldn't find managed wifi device.");
        }
    });
}
        
/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
LibNM::Client LibNM::NMThread::getClient()
{
    GLib::SharedContextPtr nmContext = getContext();
    if(!g_main_context_is_owner(*nmContext))
    {
        DBG("LibNM::NMThread::" << __func__ << 
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
LibNM::DeviceWifi LibNM::NMThread::getWifiDevice()
{
    GLib::SharedContextPtr nmContext = getContext();
    if(!g_main_context_is_owner(*nmContext))
    {
        DBG("LibNM::NMThread::" << __func__ << 
                ": Tried to get DeviceWifi outside of the LibNM event loop!");
        jassertfalse;
        return DeviceWifi();
    }
    return wifiDevice;
}

