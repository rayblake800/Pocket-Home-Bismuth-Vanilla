#ifndef LIB_NM_THREAD_IMPLEMENTATION
  #error File included directly outside of LibNM Thread implementation.
#endif
#pragma once
/**
 * @file  LibNM/ThreadResource.h
 *
 * @brief  Runs the GLib event loop shared by all LibNM NMObject classes, and 
 *         creates and allows access to the NetworkManager client and wifi
 *         device objects within the event loop.
 */

#include "GLib_ThreadResource.h"
#include "LibNM/OwnedObjects/Client.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "GLib_Borrowed_ObjectLender.h"

namespace LibNM { class ThreadResource; }

/**
 *  All interaction with LibNM::Object subclasses should occur within the
 * LibNM::ThreadResource thread, using its call and callAsync methods. The 
 * Client and DeviceWifi objects provided by the thread should be requested as 
 * needed, and not saved for future use.
 */
class LibNM::ThreadResource : public GLib::ThreadResource
{
public:
    /* Unique SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes the ThreadResource with the default GLib context, and
     *         creates the Wifi device object.
     */
    ThreadResource();

    virtual ~ThreadResource() { }

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

    /**
     * @brief  Gets the set of Wifi device objects managed by the networkClient.
     *
     *  Only the ThreadResource and the Client's private ThreadHandler class 
     * should access this method.
     *
     * @return  The ObjectLender used to manage all NMDeviceWifi* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::DeviceWifi>& getWifiDeviceLender();

    /**
     * @brief  Gets the set of ActiveConnection objects managed by the 
     *         networkClient.
     *
     *  Only the ThreadResource and the Client's private ThreadHandler class 
     * should access this method.
     *
     * @return  The ObjectLender used to manage the networkClient's
     *          NMActiveConnection* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::ActiveConnection>& 
    getConnectionLender();

    /**
     * @brief  Gets the set of AccessPoint objects managed by the active Wifi
     *         device.
     *
     *  Only the ThreadResource and the DeviceWifi's private ThreadHandler
     * should access this method.
     *
     * @return  The ObjectLender used to manage all NMAccessPoint* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::AccessPoint>& getAccessPointLender();

    /**
     * @brief  Gets the set of ActiveConnection objects managed by the active 
     *         Wifi device.
     *
     *  Only the ThreadResource and the DeviceWifi's private ThreadHandler
     * should access this method.
     *
     * @return  The ObjectLender used to manage the Wifi device's 
     *          NMActiveConnection* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::ActiveConnection>& 
    getWifiConnectionLender();

private:
    /**
     * @brief  Initializes the thread's LibNM client object if necessary.
     */
    void initClient();

    /**
     * @brief  Initializes the thread's LibNM DeviceWifi object if necessary.
     */
    void initWifiDevice();

    /* The main NetworkManager client object. */
    Client networkClient;

    /* The managed Wifi device object. */
    DeviceWifi wifiDevice;

    /*#### Borrowed::Objects managed by the networkClient: ####*/

    /* Holds DeviceWifi objects for all valid wifi devices. */
    GLib::Borrowed::ObjectLender<DeviceWifi> wifiDeviceLender;

    /* Holds ActiveConnection objects for all valid NMActiveConnection* values
       taken from the Client. */
    GLib::Borrowed::ObjectLender<ActiveConnection> connectionLender;

    /*#### Borrowed::Objects managed by the wifiDevice: ####*/

    /* Holds AccessPoint objects for all valid NMAccessPoint* values. */
    GLib::Borrowed::ObjectLender<AccessPoint> accessPointLender;

    /* Holds ActiveConnection objects for all valid NMActiveConnection* values
       taken from the DeviceWifi. */
    GLib::Borrowed::ObjectLender<ActiveConnection> wifiConnectionLender;
};
