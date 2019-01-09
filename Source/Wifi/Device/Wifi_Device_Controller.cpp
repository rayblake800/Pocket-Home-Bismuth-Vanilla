#include "Wifi_Device_Controller.h"
#include "Wifi_Device_Tracker.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/OwnedObjects/Client.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"

Wifi::Device::Controller::Controller() { }

/*
 * Enables or disables the Wifi device.
 */
void Wifi::Device::Controller::setEnabled(const bool enableWifi)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler, enableWifi]()
    {
        SharedResource::LockedPtr<Tracker> deviceTracker =
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
void Wifi::Device::Controller::scanAccessPoints()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([]()
    {
        const LibNM::ThreadHandler nmThreadHandler;
        LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        wifiDevice.requestScan();
    });
}
