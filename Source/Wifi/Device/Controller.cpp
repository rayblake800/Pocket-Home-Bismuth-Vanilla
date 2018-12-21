#include "Wifi/Device/Controller.h"
#include "Wifi/Device/DeviceTracker.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/NMObjects/Client.h"
#include "LibNM/NMObjects/DeviceWifi.h"

/*
 * Enables or disables the Wifi device.
 */
void Wifi::Controller::setEnabled(const bool enableWifi)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler, enableWifi]()
    {
        SharedResource::LockedPtr<DeviceTracker> deviceTracker =
                getWriteLockedResource();
        if(deviceTracker->wifiDeviceExists())
        {
            if(enableWifi != deviceTracker->wifiDeviceEnabled())
            {
                deviceTracker->signalDeviceStateChanging();
            }
            LibNM::Client client = nmThreadHandler.getClient();
            client.setWirelessEnabled(enableWifi);
        }
        else
        {
            DBG("Wifi::Controller::setEnabled: Couldn't " 
                    << (enableWifi ? "enable" : "disable")
                    << " Wifi, no Wifi device found.");
        }
    });
}

/*
 * Makes the Wifi device rescan nearby access points.
 */
void Wifi::Controller::scanAccessPoints()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([]()
    {
        const LibNM::ThreadHandler nmThreadHandler;
        LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        wifiDevice.requestScan();
    });
}
