#pragma once
#include "LibNM/LibNM.h"
#include "GLib/ThreadResource.h"
#include "LibNM/NMObjects/Client.h"
#include "LibNM/NMObjects/DeviceWifi.h"

/**
 * @file  LibNM/NMThread.h
 *
 * @brief  Runs a GLib event loop with the global default context, to be used
 *         only for interacting with LibNM.
 */
class LibNM::NMThread : public GLib::ThreadResource
{
public:
    /* Unique SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes the NMThread with the default GLib context.
     */
    NMThread();

    virtual ~NMThread() { }

    /**
     * @brief
     *
     * @return 
     */
    Client getClient();

    /**
     * @brief 
     *
     * @return 
     */
    DeviceWifi getWifiDevice();

private:
    /* The main NetworkManager client object. */
    Client networkClient;

    /* Interacts with the LibNM-managed wifi device. */
    DeviceWifi wifiDevice;

};
