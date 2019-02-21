#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Signal_ClientModule.h
 *
 * @brief  Notifies the Wifi device module whenever wireless networking is
 *         enabled or disabled.
 */

#include "Wifi_LibNM_Signal_ClientHandler.h"
#include "Wifi_Module.h"

namespace Wifi 
{ 
    class Resource;
    namespace Signal { class ClientModule; }
}

class Wifi::Signal::ClientModule: public Wifi::LibNM::Signal::ClientHandler,
    public Wifi::Module
{
public:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    ClientModule(Resource& parentResource);

    virtual ~ClientModule() { }

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
     * @brief  Notifies the Device::Module when wireless networking is enabled 
     *         or disabled.
     *
     * @param wifiEnabled  True if Wifi was just enabled, false if Wifi was just
     *                     disabled.
     */
    virtual void wirelessStateChange(bool wifiEnabled) override;
};
