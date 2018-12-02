#include "LibNM/NMThreadHandler.h"
#include "LibNM/NMThread.h"
    
/*
 * Creates the shared NMThread resource if it doesn't already exist.
 */
LibNM::NMThreadHandler::NMThreadHandler() :
GLib::ThreadHandler<NMThread>() { }

/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
LibNM::Client LibNM::NMThreadHandler::getClient()
{
    SharedResource::LockedPtr<NMThread> nmThread
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
