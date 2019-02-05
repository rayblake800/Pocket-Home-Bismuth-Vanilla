#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Signal_ClientHandler.h"
#include "Wifi_LibNM_Client.h"
#include "Wifi_Device_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_LibNM_ContextTest.h"
#include "SharedResource_Modular_Handler.h"

namespace NMSignal = Wifi::LibNM::Signal;

/**
 * @brief  A private handler class used to update the tracking module.
 */
class TrackingUpdater : 
    public SharedResource::Modular::Handler
    <Wifi::Resource, Wifi::Device::Module>
{
public:
    TrackingUpdater() { }

    virtual ~TrackingUpdater() { }

    /**
     * @brief  Updates the TrackingModule's saved wifi device state.
     *
     * @param exists   Indicates if a managed Wifi device is present.
     *
     * @param enabled  Indicates if the Wifi device is enabled.
     */
    void updateDeviceState(const bool exists, const bool enabled)
    {
        using namespace Wifi;
        SharedResource::Modular::LockedPtr<Resource, Device::Module> 
                deviceTracker = getWriteLockedResource();
        deviceTracker->updateDeviceState(exists, enabled);
    }
};

/*
 * Starts tracking the LibNM::ThreadResource's Client object.
 */
void NMSignal::ClientHandler::connect()
{
    const LibNM::Thread::Handler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler]()
    {
        LibNM::Client networkClient = nmThreadHandler.getClient();
        connectAllSignals(networkClient);
    });
}

/*
 * Stops tracking the LibNM::ThreadResource's Client object.
 */
void NMSignal::ClientHandler::disconnect()
{
    unsubscribeAll();
}

/*
 * Notifies the Wifi::DeviceTrackingModule when wireless networking is enabled or
 * disabled.
 */
void NMSignal::ClientHandler::wirelessStateChange(bool wifiEnabled)
{
    ASSERT_NM_CONTEXT;
    DBG("Wifi::LibNM::Signal::ClientHandler::" << __func__ 
            << ": Wireless state changed to "
            << (wifiEnabled ? "enabled" : "disabled"));
    TrackingUpdater trackingUpdater;
    const Thread::Handler nmThreadHandler;
    trackingUpdater.updateDeviceState(!nmThreadHandler.getWifiDevice().isNull(),
            wifiEnabled);
}

/*
 * Subscribes to all relevant signals from a single Client signal source.
 */
void NMSignal::ClientHandler::connectAllSignals(Client& source)
{
    ASSERT_NM_CONTEXT;
    if(!source.isNull())
    {
        createPropertyConnection(NM_CLIENT_WIRELESS_ENABLED, source);
    }
}

/*
 * Converts generic propertyChanged calls to class-specific wirelessStateChange 
 * calls.
 */
void NMSignal::ClientHandler::propertyChanged
(Client& source, juce::String property)
{ 
    ASSERT_NM_CONTEXT;
    if(!source.isNull() && property == NM_CLIENT_WIRELESS_ENABLED)
    {
        const bool enabled = source.wirelessEnabled();
        wirelessStateChange(enabled);
    }
}
