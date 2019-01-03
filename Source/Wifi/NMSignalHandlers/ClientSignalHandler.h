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

#include "SharedResource_Handler.h"
#include "LibNM/OwnedObjects/Client.h"

namespace Wifi { class ClientSignalHandler; }
namespace Wifi { class DeviceTracker; }

class Wifi::ClientSignalHandler : public LibNM::Client::Listener,
    public SharedResource::Handler<DeviceTracker>
{
public:
    ClientSignalHandler();

    virtual ~ClientSignalHandler() { }

    /**
     * @brief  Starts tracking the LibNM::ThreadResource's Client object.
     *
     * wirelessStateChange() notifications will not be received until the signal
     * handler is connected.
     */
    void connect();

    /**
     * @brief  Stops tracking the LibNM::ThreadResource's Client object.
     * 
     * wirelessStateChange() notifications will not be received after the signal
     * handler is disconnected until it is connected again.
     */
    void disconnect();

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
