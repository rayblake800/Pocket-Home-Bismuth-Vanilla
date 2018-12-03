#include "Wifi/Control/Controller.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/NMObjects/Client.h"
#include "LibNM/NMObjects/DeviceWifi.h"

/*
 * Enables or disables the Wifi device.
 */
void Wifi::Controller::setEnabled(const bool enableWifi)
{
    LibNM::ThreadHandler wifiThread;
    wifiThread.call([this, &wifiThread, enableWifi]()
    {
        LibNM::Client client = wifiThread.getClient();
        client.setWirelessEnabled(enableWifi);
    });
}

/*
 * Makes the Wifi device rescan nearby access points.
 */
void Wifi::Controller::scanAccessPoints()
{
    LibNM::ThreadHandler wifiThread;
    wifiThread.call([&wifiThread]()
    {
        LibNM::DeviceWifi wifiDevice = wifiThread.getWifiDevice();
        wifiDevice.requestScan();
    });
}
