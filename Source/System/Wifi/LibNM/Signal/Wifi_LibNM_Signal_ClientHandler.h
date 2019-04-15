#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_LibNM_Signal_ClientHandler.h
 *
 * @brief  Provides a basis for classes that should be updated when wireless
 *         networking is enabled or disabled.
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

/**
 * @brief  Receives a signal from the LibNM::Client object whenever wireless
 *         networking is enabled or disabled.
 */
class Wifi::LibNM::Signal::ClientHandler: public GLib::Signal::Handler<Client>
{
public:
    ClientHandler() { }

    virtual ~ClientHandler() { }

protected:
    /**
     * @brief  Subscribes to all relevant signals from a single Client signal
     *         source.
     *
     * @param source  A Client object this signal handler should track.
     */
    virtual void connectAllSignals(const Client source) override;

private:
    /**
     * @brief  Notifies the ClientHandler when wireless networking is enabled
     *         or disabled.
     *
     *  This method does nothing by default, ClientHandler subclasses should
     * override it to define how to handle wireless state changes.
     *
     * @param wifiEnabled  True if Wifi was just enabled, false if Wifi was
     *                     just disabled.
     */
    virtual void wirelessStateChange(bool wifiEnabled);

    /**
     * @brief  Converts generic propertyChanged calls to class-specific
     *         wirelessStateChange calls.
     *
     * @param source    The updated Client object.
     *
     * @param property  This should always be the "wireless-enabled" property.
     */
    virtual void propertyChanged(const Client source, juce::String property)
            override;
};
