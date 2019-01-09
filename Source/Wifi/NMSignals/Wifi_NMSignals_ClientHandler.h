#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_NMSignals_ClientHandler.h
 *
 * @brief  Notifies the Wifi device tracker whenever wireless networking is
 *         enabled or disabled.
 */

#include "SharedResource_Handler.h"
#include "LibNM/OwnedObjects/Client.h"

namespace Wifi { namespace NMSignals { class ClientHandler; } }
namespace Wifi { namespace Device { class Tracker; } }

class Wifi::NMSignals::ClientHandler : public LibNM::Client::Listener,
    public SharedResource::Handler<Device::Tracker>
{
public:
    ClientHandler();

    virtual ~ClientHandler() { }

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
