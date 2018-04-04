/**
 * @file WifiEventHandler
 * 
 * @brief WifiEventHandler runs a GLib event loop used for interacting with
 *         the wifi network manager.
 */
#pragma once
#include "GLibEventHandler.h"
#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>

class WifiEventHandler : public GLibEventHandler
{
public:
    /**
     * Loads client and device objects, and starts the signal thread.
     */
    WifiEventHandler();

    /**
     * Shuts down the signal thread, removing all signal handlers.
     */
    virtual ~WifiEventHandler();

    /**
     * Checks if the network manager and the wifi device are available
     * 
     * @return true iff nmClient, nmDevice, and nmWifiDevice initialized
     *          successfully.
     */
    bool isWifiAvailable();

protected:
    /**
     * Gets the network manager client object.
     */
    NMClient* getNMClient()
    {
        return nmClient;
    }

    /**
     * Get the generic network device object for device wlan0
     */
    NMDevice* getNMDevice()
    {
        return nmDevice;
    }

    /**
     * Get the wifi device object for device wlan0
     */
    NMDeviceWifi* getNMDeviceWifi()
    {
        return nmWifiDevice;
    }

    /**
     * Connects a signal handler to the network manager client.
     * 
     * @param signal         The string value of some signal the nmClient
     *                        could send.
     * 
     * @param signalHandler  A callback function to run when the signal is
     *                        sent.  This should take the NMClient* as its first
     *                        parameter and the callbackData as its second
     *                        parameter.
     * 
     * @param callbackData   User data to pass to the signalHandler function.
     * 
     * @return  an ID for this signal handler, which can be used to disconnect
     *           the signal handler.  This ID should not be used after the 
     *           signal handler is disconnected.
     */
    gulong nmClientSignalConnect(
            const char* signal,
            GCallback signalHandler,
            gpointer callbackData);

    /**
     * Connects a signal handler to the generic wlan0 device.
     * 
     * @param signal         The string value of some signal the nmDevice
     *                        could send.
     * 
     * @param signalHandler  A callback function to run when the signal is
     *                        sent.  This should take the NMDevice* as its first
     *                        parameter and the callbackData as its second
     *                        parameter.
     * 
     * @param callbackData   User data to pass to the signalHandler function.
     * 
     * @return  an ID for this signal handler, which can be used to disconnect
     *           the signal handler.  This ID should not be used after the 
     *           signal handler is disconnected.
     */
    gulong nmDeviceSignalConnect(
            const char* signal,
            GCallback signalHandler,
            gpointer callbackData);    
 

    /**
     * Connects a signal handler to the wifi device wlan0.
     * 
     * @param signal         The string value of some signal the NMDeviceWifi
     *                        could send.
     * 
     * @param signalHandler  A callback function to run when the signal is
     *                        sent.  This should take the NMDeviceWifi* as its 
     *                        first parameter and the callbackData as its second
     *                        parameter.
     * 
     * @param callbackData   User data to pass to the signalHandler function.
     * 
     * @return  an ID for this signal handler, which can be used to disconnect
     *           the signal handler.  This ID should not be used after the 
     *           signal handler is disconnected.
     */
    gulong nmWifiDeviceSignalConnect(
            const char* signal,
            GCallback signalHandler,
            gpointer callbackData);  
    
    /**
     * Disconnects a signal handler from the network manager client.
     * 
     * @toDisconnect   A signal handler ID previously returned by 
     *                   nmClientSignalConnect
     */
    void nmClientSignalDisconnect(gulong toDisconnect);
    
    /**
     * Disconnects a signal handler from the generic wlan0 device
     * 
     * @toDisconnect   A signal handler ID previously returned by 
     *                   nmDeviceSignalConnect
     */
    void nmDeviceSignalDisconnect(gulong toDisconnect);    
    /**
     * Disconnects a signal handler from the wifi device wlan0
     * 
     * @toDisconnect   A signal handler ID previously returned by 
     *                   nmWifiDeviceSignalConnect
     */
    void nmWifiSignalDisconnect(gulong toDisconnect);
    
    /**
     * Removes all signal handlers from the client, device, and wifi device.
     */
    void disconnectAllSignals();
    
private:
    
    NMClient* nmClient = nullptr;
    NMDevice* nmDevice = nullptr;
    NMDeviceWifi* nmWifiDevice = nullptr;
    
    Array<gulong,CriticalSection> clientSignalHandlers;
    Array<gulong,CriticalSection> deviceSignalHandlers;
    Array<gulong,CriticalSection> wifiDeviceSignalHandlers;
};