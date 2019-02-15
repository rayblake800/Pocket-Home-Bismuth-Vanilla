#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_LibNM_Client.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_Resource.h"
#include "Config_MainFile.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Wifi::LibNM::Thread::Module::";
#endif

namespace NMThread = Wifi::LibNM::Thread;

/*
 * Initializes the NMThread with the default GLib context.
 */
NMThread::Module::Module(Resource& wifiResource) : GLib::SharedThread
        (Resource::resourceKey.toString(),g_main_context_default()),
SharedResource::Modular::Module<Resource>(wifiResource),
networkClient() { }
        
/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
Wifi::LibNM::Client NMThread::Module::getClient()
{
    GLib::SharedContextPtr nmContext = getContext();
    if(!g_main_context_is_owner(*nmContext))
    {
        DBG(dbgPrefix << __func__ << 
                ": Tried to get Client outside of the LibNM event loop!");
        jassertfalse;
        return Client();
    }
    initClient();
    return networkClient;
}

/*
 * Gets the shared DeviceWifi object used to control the LibNM-managed Wifi 
 * device if called within the LibNM event loop.
 */
Wifi::LibNM::DeviceWifi NMThread::Module::getWifiDevice()
{
    GLib::SharedContextPtr nmContext = getContext();
    if(!g_main_context_is_owner(*nmContext))
    {
        DBG(dbgPrefix << __func__ << 
                ": Tried to get DeviceWifi outside of the LibNM event loop!");
        jassertfalse;
        return DeviceWifi();
    }
    initWifiDevice();
    return wifiDevice;
}
    
/*
 * Gets the set of AccessPoint objects managed by the active Wifi device.
 */
GLib::Borrowed::ObjectLender<Wifi::LibNM::AccessPoint>& 
NMThread::Module::getAccessPointLender()
{
    return accessPointLender;
}

/*
 * Gets the set of ActiveConnection objects managed by the active Wifi device.
 */
GLib::Borrowed::ObjectLender<Wifi::LibNM::ActiveConnection>& 
NMThread::Module::getWifiConnectionLender()
{
    return wifiConnectionLender;
}

/*
 * Initializes the thread's LibNM client object if necessary.
 */
void NMThread::Module::initClient()
{
    ASSERT_NM_CONTEXT;
    if(networkClient.isNull())
    {
        DBG(dbgPrefix << __func__ << ": Creating new NMClient*");
        networkClient = Client(nm_client_new(), &wifiDeviceLender,
                &wifiConnectionLender);
    }
}

/*
 * Initializes the thread's LibNM DeviceWifi object if necessary.
 */
void NMThread::Module::initWifiDevice()
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
        DBG(dbgPrefix << __func__ << ": Finding valid NMDeviceWifi*");
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
        DBG(dbgPrefix << __func__ << ": Couldn't find managed wifi device.");
        missingWifiDevice = true;
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Initialized Wifi device with interface "
                << wifiDevice.getInterface() << ", path="
                << wifiDevice.getPath());
        wifiDevice.setLenders(&wifiConnectionLender, &accessPointLender);
        wifiDevice.requestScan();
    }
}

/*
 * Gets the DBusThread resource's SharedResource instance key.
 */
const juce::Identifier& NMThread::Module::getThreadResourceKey() const
{
    return Resource::resourceKey;
}

/*
 * Gets the number of references connected to this Thread.
 */
int NMThread::Module::getThreadReferenceCount() const
{
    return getConstResource().getReferenceCount();
}
