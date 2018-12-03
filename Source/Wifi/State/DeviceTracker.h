#pragma once
#include "Wifi/Wifi.h"
#include "SharedResource/Resource.h"
#include "LibNM/NMObjects/Client.h"

/**
 * @file  Wifi/State/DeviceTracker.h
 *
 * @brief  Checks if the wifi device is enabled or disabled, and  
 */
class Wifi::DeviceTracker : public SharedResource::Resource
{
public:
    /**
     * @brief 
     */
    DeviceTracker();

    virtual ~DeviceTracker() { }

    /**
     * @brief 
     *
     * @return 
     */
    bool wifiDeviceEnabled() const;

private:
    /**
     * @brief 
     *
     * @param isEnabled
     */
    void updateDeviceState(bool isEnabled);

    class ClientListener : public LibNM::Client::Listener
    {
    public:
        /**
         * @brief 
         *
         * @param owner
         */
        ClientListener(DeviceTracker& owner);

    private:
        /**
         * @brief 
         *
         * @param wifiEnabled
         */
        virtual void wirelessStateChange(bool wifiEnabled) override;
    };
    ClientListener clientListener;

    /* */
    bool deviceEnabled;
};
