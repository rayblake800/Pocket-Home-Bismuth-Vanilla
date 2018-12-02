#pragma once
#include "LibNM/LibNM.h"
#include "GLib/ThreadHandler.h"

class LibNM::NMThreadHandler : public GLib::ThreadHandler<NMThread>
{
public:
    /**
     * @brief  Creates the shared NMThread resource if it doesn't already exist.
     */
    NMThreadHandler();

    virtual ~NMThreadHandler() { }
    
    /**
     * @brief  Gets the shared NetworkManager client object if called within
     *         the LibNM event loop.
     *
     * @return  The client object if called within the event loop, or a null
     *          Client if called outside of the event loop.
     */
    Client getClient();

    /**
     * @brief  Gets the shared DeviceWifi object used to control the
     *         LibNM-managed Wifi device if called within the LibNM event loop.
     *
     * @return  The wifi device object if called within the event loop, or a
     *          null DeviceWifi if called outside of the event loop, or if no
     *          managed wifi device was found.
     */
    DeviceWifi getWifiDevice();
};
