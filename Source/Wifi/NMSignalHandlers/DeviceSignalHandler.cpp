#define WIFI_IMPLEMENTATION
#include "Wifi/NMSignalHandlers/DeviceSignalHandler.h"
#include "Wifi_APList_ListResource.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_APList_NMReader.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Connection_RecordReader.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/ContextTest.h"
#if JUCE_DEBUG
#include "Wifi_DebugOutput.h"
#endif

Wifi::DeviceSignalHandler::DeviceSignalHandler() { }

/*
 * Starts tracking the LibNM::ThreadResource's DeviceWifi object.
 */
void Wifi::DeviceSignalHandler::connect()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &nmThreadHandler]()
    {
        LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        wifiDevice.addListener(*this);
        juce::Array<LibNM::AccessPoint> visibleAPs 
                = wifiDevice.getAccessPoints();
        for(LibNM::AccessPoint& accessPoint : visibleAPs)
        {
            accessPoint.addListener(apSignalHandler);
        }
    });
}

/*
 * Stops tracking the LibNM::ThreadResource's DeviceWifi object.
 */
void Wifi::DeviceSignalHandler::disconnect()
{
    unsubscribeAll();
}

/*
 * This method will be called whenever the wifi device state changes.
 */
void Wifi::DeviceSignalHandler::stateChanged(NMDeviceState newState,
        NMDeviceState oldState, NMDeviceStateReason reason)
{
    ASSERT_NM_CONTEXT;
    DBG("Wifi::DeviceSignalHandler::stateChanged" << ":  changed to "
            << deviceStateString(newState));
    DBG("LibNMInterface::stateChanged" << ":  reason="
            << deviceStateReasonString(reason));

    const LibNM::ThreadHandler nmThreadHandler;
    AccessPoint lastActiveAP = nmThreadHandler.getWifiDevice()
         .getActiveAccessPoint();
    const Connection::RecordReader connectionRecord;
    Connection::RecordWriter recordWriter;
    using Connection::EventType;
    if(lastActiveAP.isNull())
    {
        lastActiveAP = connectionRecord.getActiveAP();
    }
    switch (newState)
    {
        case NM_DEVICE_STATE_ACTIVATED:
        {
            DBG("LibNMInterface::stateChanged: new connection activated, "
                    << "send connection signal");
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
            jassert(!lastActiveAP.isNull());
            if(!connectionRecord.isConnecting())
            {
                recordWriter.addConnectionEvent(lastActiveAP,
                        EventType::startedConnecting);
            }
        }
        case NM_DEVICE_STATE_DISCONNECTED:
        {
            Connection::EventType eventType;
            if(oldState == NM_DEVICE_STATE_DEACTIVATING)
            {
                eventType = EventType::disconnected;
            }
            else
            {
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
                recordWriter.addEventIfNotDuplicate(lastActiveAP,
                        EventType::connectionAuthFailed);
                return;
            }
            recordWriter.addEventIfNotDuplicate(lastActiveAP,
                    EventType::disconnected);
            break;
        }
        case NM_DEVICE_STATE_UNKNOWN:
        case NM_DEVICE_STATE_UNMANAGED:
        case NM_DEVICE_STATE_UNAVAILABLE:
        default:
        {
            DBG("LibNMInterface::stateChanged"
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
void Wifi::DeviceSignalHandler::accessPointAdded(LibNM::AccessPoint addedAP)
{
    ASSERT_NM_CONTEXT;
    addedAP.addListener(apSignalHandler);
    SharedResource::LockedPtr<APList::ListResource> apList 
            = getWriteLockedResource();
    apList->addAccessPoint(addedAP);
}

/*
 * Updates the access point list whenever a previously seen access point is 
 * lost.
 */
void Wifi::DeviceSignalHandler::accessPointRemoved()
{
    ASSERT_NM_CONTEXT;
    SharedResource::LockedPtr<APList::ListResource> apList 
            = getWriteLockedResource();
    apList->updateAllAccessPoints();
    // TODO: define and use apList->removeInvalidatedAccessPoints() instead
}

/*
 * Updates the connection record when the active network connection changes.
 */
void Wifi::DeviceSignalHandler::activeConnectionChanged
(LibNM::ActiveConnection activeConnection)
{
    ASSERT_NM_CONTEXT;
    using Connection::EventType;
    using Connection::RecordReader;
    using Connection::Event;

    const RecordReader connectionRecordReader;
    Event lastEvent = connectionRecordReader.getLastEvent();
    EventType lastEventType = lastEvent.getEventType();
    
    EventType updateType = EventType::invalid;        
    AccessPoint connectionAP = lastEvent.getEventAP();
    if(activeConnection.isNull())
    {
        updateType = EventType::disconnected; 
    }
    else
    {
        NMActiveConnectionState connectionState 
                = activeConnection.getConnectionState();

        if(connectionState == NM_ACTIVE_CONNECTION_STATE_ACTIVATING)
        {
            updateType = EventType::startedConnecting;
        }
        else if (connectionState == NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
        {
            updateType = EventType::connected;
        }
        const LibNM::ThreadHandler nmThreadHandler;
        const LibNM::DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
        const LibNM::AccessPoint nmAP = wifiDevice.getAccessPoint(
                activeConnection.getAccessPointPath());
        const APList::Reader apListReader;
        connectionAP = apListReader.getAccessPoint(nmAP.generateHash());
    }

    // Only add a new event if the event type and access point are valid.
    // Do not add an event if its type and access point are identical to the
    // last recorded event.
    if(updateType != EventType::invalid && !connectionAP.isNull())
    {
        Connection::RecordWriter connectionRecorder;
        connectionRecorder.addEventIfNotDuplicate(connectionAP, updateType);
    }
}
