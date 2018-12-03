#pragma once
#include "Wifi/Wifi.h"
#include "LibNM/NMObjects/DeviceWifi.h"

/**
 * @file
 *
 * @brief
 */
class Wifi::ConnectionManager
{
public:
    /**
     * @brief 
     */
    ConnectionManager();

    virtual ~ConnectionManager();

private:
    /**
     * @brief 
     */
    class ConnectionListener : public LibNM::DeviceWifi::Listener
    {
    public:
        /**
         * @brief 
         *
         * @param manager
         */
        ConnectionListener(ConnectionManager& manager);

        virtual ~ConnectionListener() { }

    private:
        /**
         * @brief 
         *
         * @param newState
         * @param oldState
         * @param reason
         */
        virtual void stateChanged(
                NMDeviceState newState,
                NMDeviceState oldState,
                NMDeviceStateReason reason) override;

        /**
         * @brief 
         *
         * @param activeConnection
         *
         * @return 
         */
        virtual void activeConnectionChanged
        (LibNM::ActiveConnection activeConnection) override;

    };

};
