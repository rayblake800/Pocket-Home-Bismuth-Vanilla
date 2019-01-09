#define WIFI_IMPLEMENTATION
#include "Wifi_NMSignals_ClientHandler.h"
#include "Wifi_Device_Tracker.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/ContextTest.h"

Wifi::NMSignals::ClientHandler::ClientHandler() { }

/*
 * Starts tracking the LibNM::ThreadResource's Client object.
 */
void Wifi::NMSignals::ClientHandler::connect()
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
void Wifi::NMSignals::ClientHandler::disconnect()
{
    unsubscribeAll();
}

/*
 * Notifies the Wifi::DeviceTracker when wireless networking is enabled or
 * disabled.
 */
void Wifi::NMSignals::ClientHandler::wirelessStateChange(bool wifiEnabled)
{
    ASSERT_NM_CONTEXT;
    SharedResource::LockedPtr<Device::Tracker> deviceTracker
            = getWriteLockedResource();
    const LibNM::ThreadHandler nmThreadHandler;
    deviceTracker->updateDeviceState(!nmThreadHandler.getWifiDevice().isNull(),
            wifiEnabled);
}
