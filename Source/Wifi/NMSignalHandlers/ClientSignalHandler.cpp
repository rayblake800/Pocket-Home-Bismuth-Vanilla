#define WIFI_IMPLEMENTATION
#include "Wifi/NMSignalHandlers/ClientSignalHandler.h"
#include "Wifi/Device/DeviceTracker.h"
#include "LibNM/ThreadHandler.h"

Wifi::ClientSignalHandler::ClientSignalHandler() 
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler]()
    {
        LibNM::Client networkClient = nmThreadHandler.getClient();
        networkClient.addListener(*this);
    });
}

/*
 * Notifies the Wifi::DeviceTracker when wireless networking is enabled or
 * disabled.
 */
void Wifi::ClientSignalHandler::wirelessStateChange(bool wifiEnabled)
{
    ASSERT_CORRECT_CONTEXT;
    SharedResource::LockedPtr<DeviceTracker> deviceTracker
        = getWriteLockedResource();
    const LibNM::ThreadHandler nmThreadHandler;
    deviceTracker->updateDeviceState(!nmThreadHandler.getWifiDevice().isNull(),
            wifiEnabled);
}
