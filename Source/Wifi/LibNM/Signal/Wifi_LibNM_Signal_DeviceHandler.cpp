#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Signal_DeviceHandler.h"
#include "Wifi_APList_Module.h"
#include "Wifi_APList_Writer.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_APList_NMReader.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_Connection_Record_Writer.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_Connection.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_ContextTest.h"
#include "GLib_Signal_CallbackData.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug messages: */
static const constexpr char* dbgPrefix 
        = "Wifi::LibNM::Signal::DeviceHandler::";
#include "Wifi_DebugOutput.h"
#endif

/* Object property keys: */
static const constexpr char * activeConnectionProperty = "active-connection";

/* Object signal keys: */
static const constexpr char * stateChangeSignal = "state-changed";
static const constexpr char * addedAPSignal     = "access-point-added";
static const constexpr char * removedAPSignal   = "access-point-removed";

namespace NMSignal = Wifi::LibNM::Signal;

NMSignal::DeviceHandler::DeviceHandler() { }

/*
 * Starts tracking the LibNM::ThreadResource's DeviceWifi object.
 */
void NMSignal::DeviceHandler::connect()
{
    const Thread::Handler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler]()
    {
        LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        connectAllSignals(wifiDevice);
        juce::Array<LibNM::AccessPoint> visibleAPs 
                = wifiDevice.getAccessPoints();
        for(LibNM::AccessPoint& accessPoint : visibleAPs)
        {
            apHandler.connectAllSignals(accessPoint);
        }
    });
}

/*
 * Stops tracking the LibNM::ThreadResource's DeviceWifi object.
 */
void NMSignal::DeviceHandler::disconnect()
{
    unsubscribeAll();
    apHandler.disconnect();
}

/*
 * Subscribes to all relevant signals from a single GObject signal source.
 */
void NMSignal::DeviceHandler::connectAllSignals(DeviceWifi& source) 
{
    ASSERT_NM_CONTEXT;
    if(!source.isNull())
    {
        createConnection(stateChangeSignal, G_CALLBACK(stateChangeCallback),
                new GLib::Signal::CallbackData<DeviceWifi>(source, this));
        createConnection(addedAPSignal, G_CALLBACK(apAddedCallback),
                new GLib::Signal::CallbackData<DeviceWifi>(source, this));
        createConnection(removedAPSignal, G_CALLBACK(apRemovedCallback),
                new GLib::Signal::CallbackData<DeviceWifi>(source, this));
        createPropertyConnection(activeConnectionProperty, source);
    }
}

/*
 * This method will be called whenever the wifi device state changes.
 */
void NMSignal::DeviceHandler::stateChanged(NMDeviceState newState,
        NMDeviceState oldState, NMDeviceStateReason reason)
{
    ASSERT_NM_CONTEXT;
    DBG(dbgPrefix << __func__ << ":  changed to " << deviceStateString(newState) 
            << ", reason=" << deviceStateReasonString(reason));

    const Thread::Handler nmThreadHandler;
    Wifi::AccessPoint lastActiveAP = nmThreadHandler.getWifiDevice()
         .getActiveAccessPoint();
    namespace ConnectionRecord = Wifi::Connection::Record;
    const ConnectionRecord::Reader connectionRecord;
    ConnectionRecord::Writer recordWriter;
    using Wifi::Connection::EventType;
    if(lastActiveAP.isNull())
    {
        lastActiveAP = connectionRecord.getActiveAP();
    }
    switch (newState)
    {
        case NM_DEVICE_STATE_ACTIVATED:
        {
            DBG(dbgPrefix << __func__
                    << ": new connection activated, send connection signal");
            jassert(!lastActiveAP.isNull());
            if(!connectionRecord.isConnected())
            {
                recordWriter.addConnectionEvent(lastActiveAP, 
                        EventType::connected);
            }
            break;
        }
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
        case NM_DEVICE_STATE_NEED_AUTH:
        {
            DBG(dbgPrefix << __func__
                    << ": new connection activating, send connecting signal");
            jassert(!lastActiveAP.isNull());
            if(!connectionRecord.isConnecting())
            {
                recordWriter.addConnectionEvent(lastActiveAP,
                        EventType::startedConnecting);
            }
            break;
        }
        case NM_DEVICE_STATE_DISCONNECTED:
        {
            Wifi::Connection::EventType eventType;
            if(oldState == NM_DEVICE_STATE_DEACTIVATING)
            {
                DBG(dbgPrefix << __func__
                        << ": connection disconnected, send disconnect signal");
                eventType = EventType::disconnected;
            }
            else
            {
                DBG(dbgPrefix << __func__
                        << ": connection failed, send failure signal");
                eventType = EventType::connectionFailed;
            }
            recordWriter.addEventIfNotDuplicate(lastActiveAP, eventType);
            break;
        }
        case NM_DEVICE_STATE_DEACTIVATING:
        case NM_DEVICE_STATE_FAILED:
        {
            if(reason == NM_DEVICE_STATE_REASON_NO_SECRETS)
            {
                DBG(dbgPrefix << __func__
                        << ": missing secrets, sending auth failure signal");
                recordWriter.addEventIfNotDuplicate(lastActiveAP,
                        EventType::connectionAuthFailed);
                return;
            }
            DBG(dbgPrefix << __func__
                    << ": connection disconnected, send disconnect signal");
            recordWriter.addEventIfNotDuplicate(lastActiveAP,
                    EventType::disconnected);
            break;
        }
        case NM_DEVICE_STATE_UNKNOWN:
        case NM_DEVICE_STATE_UNMANAGED:
        case NM_DEVICE_STATE_UNAVAILABLE:
        default:
        {
            DBG(dbgPrefix << __func__
                    << ": wlan0 device entered unmanaged state: "
                    << deviceStateString(newState));
            if(!lastActiveAP.isNull())
            {
                recordWriter.addEventIfNotDuplicate(lastActiveAP,
                        EventType::disconnected);
            }
        }
    }
}

/*
 * Updates the access point list whenever a new access point is detected.
 */
void NMSignal::DeviceHandler::accessPointAdded
(LibNM::AccessPoint addedAP)
{
    ASSERT_NM_CONTEXT;
    apHandler.connectAllSignals(addedAP);

    // The apAddedCallback should be filtering out APs with null SSIDs
    jassert(addedAP.getSSID() != nullptr);

    APList::Writer listWriter;
    DBG(dbgPrefix << __func__ << ": Added Wifi AP " << addedAP.getSSIDText());
    listWriter.addAccessPoint(addedAP);
}

/*
 * Updates the access point list whenever a previously seen access point is 
 * lost.
 */
void NMSignal::DeviceHandler::accessPointRemoved()
{
    ASSERT_NM_CONTEXT;
        DBG(dbgPrefix << __func__ << ": Removed Wifi AP.");
    APList::Writer listWriter;
    listWriter.removeInvalidatedAccessPoints();
}

/*
 * Updates the connection record when the active network connection changes.
 */
void NMSignal::DeviceHandler::activeConnectionChanged
(LibNM::ActiveConnection activeConnection)
{
    ASSERT_NM_CONTEXT;
    using Wifi::Connection::EventType;
    using Wifi::Connection::Record::Reader;
    using Wifi::Connection::Event;

    DBG(dbgPrefix << __func__ << ": active connection changed to " 
            << (activeConnection.isNull() ? 
                "NULL" : activeConnection.getUUID()));

    const Reader connectionRecordReader;
    Event lastEvent = connectionRecordReader.getLatestEvent();
    EventType lastEventType = lastEvent.getEventType();
    
    EventType updateType = EventType::invalid;        
    Wifi::AccessPoint connectionAP = lastEvent.getEventAP();
    if(activeConnection.isNull())
    {
        updateType = EventType::disconnected; 
        static_cast<APInterface::SavedConnection*>(&connectionAP)
                ->setLastConnectionTime
                (juce::Time::getCurrentTime().toMilliseconds());
    }
    else
    {
        const Thread::Handler nmThreadHandler;
        const LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        const LibNM::AccessPoint nmAP = wifiDevice.getAccessPoint(
                activeConnection.getAccessPointPath());
        const APList::Reader apListReader;
        connectionAP = apListReader.getAccessPoint(nmAP.generateHash());

        NMActiveConnectionState connectionState 
                = activeConnection.getConnectionState();

        if(connectionState == NM_ACTIVE_CONNECTION_STATE_ACTIVATING)
        {
            updateType = EventType::startedConnecting;
        }
        else if (connectionState == NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
        {
            updateType = EventType::connected;
            APInterface::SavedConnection* apUpdateInterface
                    = static_cast<APInterface::SavedConnection*>(&connectionAP);
            apUpdateInterface->setLastConnectionTime
                    (juce::Time::getCurrentTime().toMilliseconds());
            apUpdateInterface->setHasSavedConnection(true);
        }
    }

    // Only add a new event if the event type and access point are valid.
    // Do not add an event if its type and access point are identical to the
    // last recorded event.
    if(updateType != EventType::invalid && !connectionAP.isNull())
    {
        Wifi::Connection::Record::Writer connectionRecorder;
        connectionRecorder.addEventIfNotDuplicate(connectionAP, updateType);
    }
}
 
/*
 * Converts generic property change notifications into activeConnectionChanged 
 * calls.
 */
void NMSignal::DeviceHandler::propertyChanged(DeviceWifi& source,
        juce::String property)
{
    ASSERT_NM_CONTEXT;
    jassert(property == activeConnectionProperty);
    if(!source.isNull())
    {
        activeConnectionChanged(source.getActiveConnection());
    }
}

/*
 * The GCallback method called directly by LibNM code when sending state-changed 
 * signals.
 */
void NMSignal::DeviceHandler::stateChangeCallback(
        NMDevice* device,
        NMDeviceState newState,
        NMDeviceState oldState,
        NMDeviceStateReason reason,
        GLib::Signal::CallbackData<DeviceWifi>* data) 
{ 
    ASSERT_NM_CONTEXT;
    if(data != nullptr)
    {
        DeviceHandler* deviceHandler = dynamic_cast<DeviceHandler*>
                (data->getSignalHandler());
        if(deviceHandler != nullptr)
        {
            deviceHandler->stateChanged(newState, oldState, reason);
        }
    }
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-added signals.
 */
void NMSignal::DeviceHandler::apAddedCallback(
        NMDeviceWifi* device,
        NMAccessPoint* ap,
        GLib::Signal::CallbackData<DeviceWifi>* data) 
{ 
    ASSERT_NM_CONTEXT;
    if(device != nullptr && ap != nullptr && data != nullptr)
    {
        // Ignore access points with null SSIDs.
        // TODO: Find out why access points without SSIDs are showing up at all
        //       Are they hidden? Have the SSIDs just not loaded yet?
        if(nm_access_point_get_ssid(ap) == nullptr)
        {
            DBG(dbgPrefix << __func__ << ": Ignoring AP with null SSID.");
            return;
        }

        DeviceWifi wifiDevice = data->getSignalSource();
        DeviceHandler* deviceHandler = dynamic_cast<DeviceHandler*>
                (data->getSignalHandler());
        if(!wifiDevice.isNull() && deviceHandler != nullptr)
        {
            deviceHandler->accessPointAdded(wifiDevice.getAPLender()
                    ->borrowObject(G_OBJECT(ap)));
        }
    }
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-removed signals.
 */
void NMSignal::DeviceHandler::apRemovedCallback(
        NMDeviceWifi* device, 
        NMAccessPoint* ap,
        GLib::Signal::CallbackData<DeviceWifi>* data) 
{ 
    ASSERT_NM_CONTEXT;
    if(data != nullptr)
    {
        DeviceWifi wifiDevice = data->getSignalSource();
        DeviceHandler* deviceHandler = dynamic_cast<DeviceHandler*>
                (data->getSignalHandler());
        if(!wifiDevice.isNull() && deviceHandler != nullptr)
        {
            wifiDevice.getAPLender()->invalidateObject(G_OBJECT(ap));
            deviceHandler->accessPointRemoved();
        }
    }
}
