#include "WifiEventHandler.h"

/**
 * Loads client and device objects, and starts the signal thread.
 */
WifiEventHandler::WifiEventHandler()
{
    gLibCall([this]()
    {
        nmClient = nm_client_new();
        if (nmClient == nullptr || !NM_IS_CLIENT(nmClient))
        {
            DBG("WifiEventHandler::" << __func__
                    << ": failed to connect to nmclient over dbus");
                    nmClient = nullptr;
            return;
        }
        nmDevice = nm_client_get_device_by_iface(nmClient, "wlan0");
        if (nmDevice == nullptr || !NM_IS_DEVICE(nmDevice))
        {
            DBG("WifiEventHandler::" << __func__ <<
                    ":  failed to connect to nmDevice wlan0 over DBus");
                    nmClient = nullptr;
                    nmDevice = nullptr;
            return;
        }
        nmWifiDevice = NM_DEVICE_WIFI(nmDevice);
        if (nmWifiDevice == nullptr || !NM_IS_DEVICE_WIFI(nmWifiDevice))
        {
            DBG("WifiEventHandler::" << __func__ <<
                    ": network manager cannot access wlan0 as a wifi device!");
                    nmClient = nullptr;
                    nmDevice = nullptr;
                    nmWifiDevice = nullptr;
            return;
        }
    });
}

/**
 * Shuts down the signal thread, removing all signal handlers.
 */
WifiEventHandler::~WifiEventHandler()
{
    disconnectAllSignals();
}

/**
 * Checks if the network manager and the wifi device are available
 */
bool WifiEventHandler::isWifiAvailable()
{
    return nmClient != nullptr
            && nmDevice != nullptr
            && nmWifiDevice != nullptr;
}

/**
 * Connects a signal handler to the network manager client.
 */
gulong WifiEventHandler::nmClientSignalConnect(
        const char* signal,
        GCallback signalHandler,
        gpointer callbackData)
{
    if (nmClient == nullptr)
    {
        return 0;
    }
    gulong handlerId = g_signal_connect
            (nmClient, signal, signalHandler, callbackData);
    clientSignalHandlers.add(handlerId);
    return handlerId;
}

/**
 * Connects a signal handler to the generic wlan0 device.
 */
gulong WifiEventHandler::nmDeviceSignalConnect(
        const char* signal,
        GCallback signalHandler,
        gpointer callbackData)
{
    if (nmDevice == nullptr)
    {
        return 0;
    }
    gulong handlerId = g_signal_connect
            (nmDevice, signal, signalHandler, callbackData);
    deviceSignalHandlers.add(handlerId);
    return handlerId;
}

/**
 * Connects a signal handler to the wifi device wlan0.
 */
gulong WifiEventHandler::nmWifiDeviceSignalConnect(
        const char* signal,
        GCallback signalHandler,
        gpointer callbackData)
{
    if (nmWifiDevice == nullptr)
    {
        return 0;
    }
    gulong handlerId = g_signal_connect
            (nmWifiDevice, signal, signalHandler, callbackData);
    wifiDeviceSignalHandlers.add(handlerId);
    return handlerId;
}

/**
 * Disconnects a signal handler from the network manager client.
 */
void WifiEventHandler::nmClientSignalDisconnect(gulong toDisconnect)
{
    if (nmClient != nullptr)
    {
        g_signal_handler_disconnect(nmClient, toDisconnect);
        clientSignalHandlers.removeAllInstancesOf(toDisconnect);
    }
}

/**
 * Disconnects a signal handler from the generic wlan0 device
 */
void WifiEventHandler::nmDeviceSignalDisconnect(gulong toDisconnect)
{
    if (nmDevice != nullptr)
    {
        g_signal_handler_disconnect(nmDevice, toDisconnect);
        deviceSignalHandlers.removeAllInstancesOf(toDisconnect);
    }
}

/**
 * Disconnects a signal handler from the wifi device wlan0
 */
void WifiEventHandler::nmWifiSignalDisconnect(gulong toDisconnect)
{
    if (nmWifiDevice != nullptr)
    {
        g_signal_handler_disconnect(nmWifiDevice, toDisconnect);
        wifiDeviceSignalHandlers.removeAllInstancesOf(toDisconnect);
    }
}

/**
 * Removes all signal handlers from the client, device, and wifi device.
 */
void WifiEventHandler::disconnectAllSignals()
{
    if (nmWifiDevice != nullptr && NM_IS_DEVICE_WIFI(nmWifiDevice))
    {
        for (gulong& signalHandlerId : wifiDeviceSignalHandlers)
        {
            g_signal_handler_disconnect(nmWifiDevice, signalHandlerId);
        }
    }
    wifiDeviceSignalHandlers.clear();

    if (nmDevice != nullptr && NM_IS_DEVICE(nmDevice))
    {
        for (gulong& signalHandlerId : deviceSignalHandlers)
        {
            g_signal_handler_disconnect(nmDevice, signalHandlerId);
        }
    }
    deviceSignalHandlers.clear();

    if (nmClient != nullptr && NM_IS_CLIENT(nmClient))
    {
        for (gulong& signalHandlerId : clientSignalHandlers)
        {
            g_signal_handler_disconnect(nmClient, signalHandlerId);
        }
        clientSignalHandlers.clear();
    }
    clientSignalHandlers.clear();
}