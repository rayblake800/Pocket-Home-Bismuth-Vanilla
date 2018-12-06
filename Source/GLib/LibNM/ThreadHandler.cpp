#define LIB_NM_THREAD_IMPLEMENTATION
#include "LibNM/ThreadHandler.h"
#include "LibNM/ThreadResource.h"
    
/*
 * Creates the shared LibNM::ThreadResource if it doesn't already exist.
 */
LibNM::ThreadHandler::ThreadHandler() :
GLib::ThreadHandler<ThreadResource>() { }

/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
LibNM::Client LibNM::ThreadHandler::getClient()
{
    SharedResource::LockedPtr<ThreadResource> nmThread
        = getReadLockedResource();
    return nmThread->getClient();
}

/**
 * @brief  Gets the shared DeviceWifi object used to control the
 *         LibNM-managed Wifi device if called within the LibNM event loop.
 *
 * @return  The wifi device object if called within the event loop, or a
 *          null DeviceWifi if called outside of the event loop, or if no
 *          managed wifi device was found.
 */
DeviceWifi getWifiDevice();
