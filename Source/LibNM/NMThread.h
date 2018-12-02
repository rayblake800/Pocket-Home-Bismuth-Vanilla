#pragma once
#include "LibNM/LibNM.h"
#include "GLib/ThreadResource.h"
#include "LibNM/NMObjects/Client.h"
#include "LibNM/NMObjects/DeviceWifi.h"

/**
 * @file  LibNM/NMThread.h
 *
 * @brief  Runs the GLib event loop shared by all LibNM NMObject classes, and 
 *         creates and allows access to the NetworkManager client and wifi
 *         device objects within the event loop.
 *
 *  All interaction with LibNM::Object subclasses should occur within the
 * NMThread, using its call and callAsync methods. The Client and DeviceWifi
 * objects provided by the NMThread should be requested as needed, and not
 * saved for future use.
 */
class LibNM::NMThread : public GLib::ThreadResource
{
public:
    /* Unique SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes the NMThread with the default GLib context, and
     *         creates the Wifi device object.
     */
    NMThread();

    virtual ~NMThread() { }

    /**
     * @brief  Gets the shared NetworkManager client object if called within
     *         the LibNM event loop.
     *
     * @return  The client object if called within the event loop, or a null
     *          Client if called outside of the event loop.
     */
    Client getClient();

    /**
     * @brief  Gets the shared DeviceWifi object used to control the
     *         LibNM-managed Wifi device if called within the LibNM event loop.
     *
     * @return  The wifi device object if called within the event loop, or a
     *          null DeviceWifi if called outside of the event loop, or if no
     *          managed wifi device was found.
     */
    DeviceWifi getWifiDevice();

private:
    /* The main NetworkManager client object. */
    Client networkClient;

    /* Interacts with the LibNM-managed wifi device. */
    DeviceWifi wifiDevice;
};
