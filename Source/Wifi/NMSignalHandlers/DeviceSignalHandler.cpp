#define WIFI_IMPLEMENTATION
#include "Wifi/NMSignalHandlers/DeviceSignalHandler.h"
#include "Wifi/AccessPointList/APList.h"
#include "Wifi/AccessPointList/APListReader.h"
#include "Wifi/AccessPointList/NMAPListReader.h"
#include "Wifi/Connection/Event.h"
#include "Wifi/Connection/RecordReader.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/NMObjects/DeviceWifi.h"
#include "LibNM/NMObjects/ActiveConnection.h"
#include "LibNM/ThreadHandler.h"

/*
 * Sets up Wifi access point signal strength tracking.
 */
Wifi::DeviceSignalHandler::DeviceSignalHandler()
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
 * Updates the access point list whenever a new access point is detected.
 */
void Wifi::DeviceSignalHandler::accessPointAdded(LibNM::AccessPoint addedAP)
{
    ASSERT_CORRECT_CONTEXT;
    addedAP.addListener(apSignalHandler);
    apListWriter.addAccessPoint(addedAP);
}

/*
 * Updates the access point list whenever a previously seen access point is 
 * lost.
 */
void Wifi::DeviceSignalHandler::accessPointRemoved
(LibNM::AccessPoint removedAP)
{
    ASSERT_CORRECT_CONTEXT;
    apListWriter.removeAccessPoint(removedAP);
}

/*
 * Updates the connection record when the active network connection changes.
 */
void Wifi::DeviceSignalHandler::activeConnectionChanged
(LibNM::ActiveConnection activeConnection)
{
    ASSERT_CORRECT_CONTEXT;
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
        const APListReader apListReader;
        connectionAP = apListReader.getAccessPoint(nmAP.generateHash());
    }

    // Only add a new event if the event type and access point are valid.
    // Do not add an event if its type and access point are identical to the
    // last recorded event.
    if(updateType != EventType::invalid && !connectionAP.isNull()
            && (updateType != lastEventType 
                || connectionAP != lastEvent.getEventAP()))
    {
        connectionRecorder.addConnectionEvent(Event(connectionAP, updateType));
    }
}
