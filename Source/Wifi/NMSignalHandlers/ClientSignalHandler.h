#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi/NMSignalHandlers/ClientSignalHandler.h
 *
 * @brief  Notifies the Wifi device tracker whenever wireless networking is
 *         enabled or disabled.
 */

#include "SharedResource/Handler.h"
#include "LibNM/NMObjects/Client.h"

namespace Wifi { class ClientSignalHandler; }
namespace Wifi { class DeviceTracker; }

class Wifi::ClientSignalHandler : public LibNM::Client::Listener,
    public SharedResource::Handler<DeviceTracker>
{
public:
    ClientSignalHandler();

    virtual ~ClientSignalHandler() { }

private:
    /**
     * @brief  Notifies the Wifi::DeviceTracker when wireless networking is
     *         enabled or disabled.
     *
     * @param wifiEnabled  True if Wifi was just enabled, false if Wifi was just
     *                     disabled.
     */
    virtual void wirelessStateChange(bool wifiEnabled) final override;
};
