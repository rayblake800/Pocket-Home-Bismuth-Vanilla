#pragma once
#include "Wifi/Wifi.h"
#include "LibNM/NMObjects/Client.h"
#include "JuceHeader.h"

/**
 * @file  Wifi/Control/ConnectionHandler.h
 *
 * @brief  Connects to and disconnects from wireless networks, and clears saved
 *         connections.
 */
class Wifi::ConnectionManager
{
public:
    ConnectionManager() { }

    virtual ~ConnectionManager() { }

    void connectToAccessPoint(const AccessPoint toConnect,
            juce::String securityKey = juce::String()) const;

    void disconnect() const;

    void forgetConnection(const AccessPoint toForget) const;

private:
    class ConnectionHandler : public LibNM::Client::ConnectionHandler
    {
    public:
        ConnectionHandler() { }

        virtual ~ConnectionHandler() { }

    private:
        virtual void openingConnection(LibNM::ActiveConnection connection,
                bool isNew) override;

        virtual void openingConnectionFailed(LibNM::ActiveConnection connection,
                GError* error, bool isNew) override;
    };
    ConnectionHandler connectionHandler;
};
