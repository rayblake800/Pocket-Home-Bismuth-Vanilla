#define LIB_NM_THREAD_IMPLEMENTATION
#include "LibNM/ThreadHandler.h"
#include "LibNM/ThreadResource.h"
#include "LibNM/NMObjects/Client.h"
#include "LibNM/NMObjects/DeviceWifi.h"

/*
 * Creates the shared LibNM::ThreadResource if it doesn't already exist.
 */
LibNM::ThreadHandler::ThreadHandler() :
GLib::ThreadHandler(ThreadResource::resourceKey, 
        []()->GLib::ThreadResource* { return new ThreadResource(); } ) { }

/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
LibNM::Client LibNM::ThreadHandler::getClient()
{
    SharedResource::LockedPtr<ThreadResource> nmThread
        = getReadLockedResource<ThreadResource>();
    return nmThread->getClient();
}

/*
 * Gets the shared DeviceWifi object used to control the LibNM-managed Wifi 
 * device if called within the LibNM event loop.
 */
LibNM::DeviceWifi LibNM::ThreadHandler::getWifiDevice()
{
    SharedResource::LockedPtr<ThreadResource> nmThread
        = getReadLockedResource<ThreadResource>();
    return nmThread->getWifiDevice();
}
