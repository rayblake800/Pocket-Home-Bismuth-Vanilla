#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_LibNM_Signal_ClientHandler.h
 *
 * @brief  Notifies the Wifi device tracker whenever wireless networking is
 *         enabled or disabled.
 */

#include "GLib_Signal_Handler.h"

namespace Wifi 
{ 
    namespace LibNM 
    { 
        namespace Signal { class ClientHandler; }
        class Client;
    } 
}

class Wifi::LibNM::Signal::ClientHandler: public GLib::Signal::Handler<Client>
{
public:
    ClientHandler() { }

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
    * @brief  Subscribes to all relevant signals from a single Client signal 
    *         source.
    * 
    * @param source  A Client object this signal handler should track.
    */
    virtual void connectAllSignals(Client& source) override; 

    /**
     * @brief  Notifies the Device::TrackingModule when wireless networking is
     *         enabled or disabled.
     *
     * @param wifiEnabled  True if Wifi was just enabled, false if Wifi was just
     *                     disabled.
     */
    void wirelessStateChange(bool wifiEnabled);
        
    /**
     * @brief  Converts generic propertyChanged calls to class-specific 
     *         wirelessStateChange calls.
     * 
     * @param source    The updated Client object.
     * 
     * @param property  This should always be the "wireless-enabled" property.
     */
    virtual void propertyChanged(Client& source, juce::String property) 
            override;  
};
