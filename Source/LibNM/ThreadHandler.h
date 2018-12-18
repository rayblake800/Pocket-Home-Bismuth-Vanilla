#pragma once
/**
 * @file  LibNM/ThreadHandler.h
 *
 * @brief  Provides access to the GLib event loop thread used when accessing
 *         any LibNM::NMObject objects.
 */

#include "GLib/ThreadHandler.h"

namespace LibNM { class ThreadHandler; }
namespace LibNM { class ThreadResource; }
namespace LibNM { class Client; }
namespace LibNM { class DeviceWifi; }

class LibNM::ThreadHandler : public GLib::ThreadHandler<ThreadResource>
{
public:
    /**
     * @brief  Creates the shared LibNM::ThreadResource if it doesn't already 
     *         exist.
     */
    ThreadHandler();

    virtual ~ThreadHandler() { }
    
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
