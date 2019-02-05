#define WIFI_IMPLEMENTATION
#include "Wifi_Device_Controller.h"
#include "Wifi_Device_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_Client.h"
#include "Wifi_LibNM_DeviceWifi.h"

Wifi::Device::Controller::Controller() { }

/*
 * Enables or disables the Wifi device.
 */
void Wifi::Device::Controller::setEnabled(const bool enableWifi)
{
    const LibNM::Thread::Handler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler, enableWifi]()
    {
        SharedResource::Modular::LockedPtr<Resource, Module> deviceModule 
                = getWriteLockedResource();
        if(deviceModule->wifiDeviceExists())
        {
            if(enableWifi != deviceModule->wifiDeviceEnabled())
            {
                deviceModule->signalDeviceStateChanging();
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
    const LibNM::Thread::Handler nmThreadHandler;
    nmThreadHandler.call([&nmThreadHandler]()
    {
        LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        wifiDevice.requestScan();
    });
}
