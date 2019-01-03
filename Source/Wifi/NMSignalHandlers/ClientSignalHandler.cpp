#define WIFI_IMPLEMENTATION
#include "Wifi/NMSignalHandlers/ClientSignalHandler.h"
#include "Wifi/Device/DeviceTracker.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/ContextTest.h"

Wifi::ClientSignalHandler::ClientSignalHandler() { }

/*
 * Starts tracking the LibNM::ThreadResource's Client object.
 */
void Wifi::ClientSignalHandler::connect()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler]()
    {
        LibNM::Client networkClient = nmThreadHandler.getClient();
        networkClient.addListener(*this);
    });
}

/*
 * Stops tracking the LibNM::ThreadResource's Client object.
 */
void Wifi::ClientSignalHandler::disconnect()
{
    unsubscribeAll();
}

/*
 * Notifies the Wifi::DeviceTracker when wireless networking is enabled or
 * disabled.
 */
void Wifi::ClientSignalHandler::wirelessStateChange(bool wifiEnabled)
{
    ASSERT_NM_CONTEXT;
    SharedResource::LockedPtr<DeviceTracker> deviceTracker
        = getWriteLockedResource();
    const LibNM::ThreadHandler nmThreadHandler;
    deviceTracker->updateDeviceState(!nmThreadHandler.getWifiDevice().isNull(),
            wifiEnabled);
}
