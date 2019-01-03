#define LIB_NM_THREAD_IMPLEMENTATION
#include "LibNM/ThreadResource.h"
#include "LibNM/OwnedObjects/Client.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/ContextTest.h"
#include "Config/MainFile.h"

/* Unique SharedResource object instance key: */
const juce::Identifier LibNM::ThreadResource::resourceKey 
        = "LibNM::ThreadResource";

/* Tracks if no Wifi device exists, to make sure the thread doesn't waste time
   trying to find it every single time getWifiDevice() is called. */
static bool missingWifiDevice = false;

/*
 * Initializes the NMThread with the default GLib context, and creates the Wifi 
 * device object.
 */
LibNM::ThreadResource::ThreadResource() : GLib::ThreadResource(resourceKey,
        GLib::SharedContextPtr(g_main_context_default())) 
{ 
    startThread();
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
    initClient();
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
    initWifiDevice();
    return wifiDevice;
}
    


/*
 * Gets the set of Wifi device objects managed by the networkClient.
 */
GLib::Borrowed::ObjectLender<LibNM::DeviceWifi>& 
LibNM::ThreadResource::getWifiDeviceLender()
{
    return wifiDeviceLender;
}

/*
 * Gets the set of ActiveConnection objects managed by the networkClient.
 */
GLib::Borrowed::ObjectLender<LibNM::ActiveConnection>& 
LibNM::ThreadResource::getConnectionLender()
{
    return connectionLender;
}

/*
 * Gets the set of AccessPoint objects managed by the active Wifi device.
 */
GLib::Borrowed::ObjectLender<LibNM::AccessPoint>& 
LibNM::ThreadResource::getAccessPointLender()
{
    return accessPointLender;
}

/*
 * Gets the set of ActiveConnection objects managed by the active Wifi device.
 */
GLib::Borrowed::ObjectLender<LibNM::ActiveConnection>& 
LibNM::ThreadResource::getWifiConnectionLender()
{
    return wifiConnectionLender;
}

/*
 * Initializes the thread's LibNM client object if necessary.
 */
void LibNM::ThreadResource::initClient()
{
    ASSERT_NM_CONTEXT;
    if(networkClient.isNull())
    {
        networkClient.initClient();
    }
}

/*
 * Initializes the thread's LibNM DeviceWifi object if necessary.
 */
void LibNM::ThreadResource::initWifiDevice()
{
    ASSERT_NM_CONTEXT;
    if(missingWifiDevice || !wifiDevice.isNull())
    {
        return;
    }

    initClient();
    using juce::String;
    using juce::Array;
    Config::MainFile config;
    String wifiInterface = config.getWifiInterface();
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
        missingWifiDevice = true;
    }
    else
    {
        DBG("LibNM::ThreadResource::ThreadResource: "
                << "Initialized Wifi device with interface "
                << wifiDevice.getInterface() << ", path="
                << wifiDevice.getPath());
        wifiDevice.requestScan();
    }
}
