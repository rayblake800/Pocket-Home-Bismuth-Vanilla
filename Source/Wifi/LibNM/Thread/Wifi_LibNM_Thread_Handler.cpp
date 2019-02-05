#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_Resource.h"

namespace NMThread = Wifi::LibNM::Thread;

/*
 * Creates the shared Wifi::Resource if it doesn't already exist.
 */
NMThread::Handler::Handler() :
GLib::ThreadHandler <SharedResource::Modular::Handler<Resource, Module>>
    (Resource::resourceKey) { } 

/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
Wifi::LibNM::Client NMThread::Handler::getClient() const
{
    SharedResource::Modular::LockedPtr<Resource, Module> nmThread
            = getWriteLockedResource();
    return nmThread->getClient();
}

/*
 * Gets the shared DeviceWifi object used to control the LibNM-managed Wifi 
 * device if called within the LibNM event loop.
 */
Wifi::LibNM::DeviceWifi NMThread::Handler::getWifiDevice() const
{
    SharedResource::Modular::LockedPtr<Resource, Module> nmThread
            = getWriteLockedResource();
    return nmThread->getWifiDevice();
}
    
void NMThread::Handler::call(std::function<void()> toCall) const
{
    GLib::ThreadHandler<SharedResource::Modular::Handler<Resource, Module>>
            ::call(toCall);
}
