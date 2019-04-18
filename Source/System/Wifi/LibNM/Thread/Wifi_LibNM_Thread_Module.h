#pragma once
/**
 * @file  Wifi_LibNM_Thread_Module.h
 *
 * @brief  Runs the GLib event loop shared by all LibNM NMObject classes, and
 *         creates and allows access to the NetworkManager client and wifi
 *         device objects within the event loop.
 */

#include "GLib_SharedThread.h"
#include "Wifi_Module.h"
#include "Wifi_LibNM_Client.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "GLib_Borrowed_ObjectLender.h"

namespace Wifi
{
    namespace LibNM { namespace Thread { class Module; } }
    class Resource;
}

/**
 * @brief  The Wifi::Module responsible for creating and managing the LibNM
 *         event loop thread, and for initializing the LibNM client and wifi
 *         device objects.
 *
 *  All interaction with LibNM::Object subclasses should occur within the
 * LibNM::ThreadResource thread, using its call and callAsync functions. The
 * Client and DeviceWifi objects provided by the thread should be requested as
 * needed, and not saved for future use.
 */
class Wifi::LibNM::Thread::Module : public GLib::SharedThread,
        public Wifi::Module
{
public:
    /**
     * @brief  Initializes the ThreadResource with the default GLib context.
     *
     * @param parentResource  The main Wifi::Resource object.
     */
    Module(Resource& parentResource);

    virtual ~Module() { }

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
     * @brief  Gets the set of AccessPoint objects managed by the active Wifi
     *         device.
     *
     *  Only the ThreadResource and the DeviceWifi's private ThreadHandler
     * should access this function.
     *
     * @return  The ObjectLender used to manage all NMAccessPoint* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::AccessPoint>& getAccessPointLender();

    /**
     * @brief  Gets the set of ActiveConnection objects managed by the active
     *         Wifi device.
     *
     *  Only the ThreadResource and the DeviceWifi's private ThreadHandler
     * should access this function.
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

    /**
     * @brief  Gets the DBusThread resource's SharedResource instance key.
     *
     * @return  The resourceKey value.
     */
    virtual const juce::Identifier& getThreadResourceKey() const override;

    /**
     * @brief  Gets the number of references connected to this Thread.
     *
     * @return   The DBusThread's reference count.
     */
    virtual int getThreadReferenceCount() const override;

    // The main NetworkManager client object.
    Client networkClient;

    // The managed Wifi device object.
    DeviceWifi wifiDevice;

    //#### Borrowed::Objects managed by the networkClient: ####

    // Holds DeviceWifi objects for all valid wifi devices.
    GLib::Borrowed::ObjectLender<DeviceWifi> wifiDeviceLender;

    // Holds ActiveConnection objects for all valid NMActiveConnection* values
    // taken from the Client.
    GLib::Borrowed::ObjectLender<ActiveConnection> connectionLender;

    //#### Borrowed::Objects managed by the wifiDevice: ####

    // Holds AccessPoint objects for all valid NMAccessPoint* values.
    GLib::Borrowed::ObjectLender<AccessPoint> accessPointLender;

    // Holds ActiveConnection objects for all valid NMActiveConnection* values
    // taken from the DeviceWifi.
    GLib::Borrowed::ObjectLender<ActiveConnection> wifiConnectionLender;

    // Tracks if no Wifi device exists, to make sure the thread doesn't waste
    // time trying to find it every single time getWifiDevice() is called.
    bool missingWifiDevice = false;
};
