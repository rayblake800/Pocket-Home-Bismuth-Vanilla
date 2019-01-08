#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_RecordResource.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Connection_UpdateInterface.h"
#include "Wifi_AP_AccessPoint.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_APList_NMReader.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "LibNM/OwnedObjects/Client.h"
#include "LibNM/ThreadHandler.h"

namespace WifiConnect = Wifi::Connection;

/* SharedResource object instance key: */
const juce::Identifier WifiConnect::RecordResource::resourceKey 
        = "Wifi_Connection_RecordResource";

/* Stores all connection events: */
static juce::Array<WifiConnect::Event> connectionEvents;

/* Sorts the event list from newest to oldest. */
class
{
public:
    static int compareElements(WifiConnect::Event first, 
            WifiConnect::Event second)
    {
        juce::int64 timeDifference = first.getEventTime().currentTimeMillis()
            - second.getEventTime().currentTimeMillis();
        if(timeDifference == 0)
        {
            return 0;
        }
        return ((timeDifference < 0) ? -1 : 1);
    }
} eventSorter;

/*
 * Reads NetworkManager data to build the initial set of connection records.
 */
WifiConnect::RecordResource::RecordResource() : 
SharedResource::Resource(resourceKey)
{
    savedConnections.updateSavedConnections();
    updateRecords();
}


/*
 * Checks if the system has an active, established Wifi network connection.
 */
bool WifiConnect::RecordResource::isConnected() const
{
    return connectionEvents.getFirst().getEventType() == EventType::connected;
}

/*
 * Checks if NetworkManager is currently opening a network connection.
 */
bool WifiConnect::RecordResource::isConnecting() const
{
    EventType latestEventType = connectionEvents.getFirst().getEventType();
    return latestEventType == EventType::connectionRequested
        || latestEventType == EventType::startedConnecting;
}

/*
 * Gets the access point being used by the active or activating connection.
 */
Wifi::AccessPoint WifiConnect::RecordResource::getActiveAP() const
{
    Event latestEvent = connectionEvents.getFirst();
    if(latestEvent.getEventType() == EventType::disconnected
            || latestEvent.getEventType() == EventType::invalid)
    {
        return AccessPoint();
    }
    return latestEvent.getEventAP();
}

/*
 * Gets all saved connections compatible with a particular AccessPoint object.
 */
juce::Array<LibNM::SavedConnection> 
WifiConnect::RecordResource::getMatchingConnections
(const Wifi::AccessPoint toMatch) const
{
    APList::NMReader nmList;
    LibNM::AccessPoint nmAccessPoint 
            = nmList.getStrongestNMAccessPoint(toMatch);
    if(nmAccessPoint.isNull())
    {
        return juce::Array<LibNM::SavedConnection>();
    }
    return savedConnections.findConnectionsForAP(nmAccessPoint);
}

/*
 * Checks if NetworkManager has a saved connection that is compatible with an 
 * access point.
 */
bool WifiConnect::RecordResource::hasSavedConnection
(const AccessPoint toCheck) const
{
    bool matchFound = false;
    LibNM::ThreadHandler nmThread;
    nmThread.call([this, &toCheck, &matchFound]()
    {
        matchFound = !getMatchingConnections(toCheck).isEmpty();
    });
    return matchFound;
}

/*
 * Finds the last time the system was fully connected to a particular wifi 
 * access point's connection.
 */
juce::Time WifiConnect::RecordResource::lastConnectionTime
(const AccessPoint connectionAP) const
{
    juce::Time connectionTime;
    LibNM::ThreadHandler nmThread;
    nmThread.call([this, &connectionAP, &connectionTime]()
    {
        juce::Array<LibNM::SavedConnection> matchingConnections
                = getMatchingConnections(connectionAP);
        for(const LibNM::SavedConnection& connection : matchingConnections)
        {
            juce::Time timestamp = connection.lastConnectionTime();
            if(timestamp.toMilliseconds() > connectionTime.toMilliseconds())
            {
                connectionTime = timestamp;
            }
        }
    });
    return connectionTime;
}

/*
 * Adds a new event to the list of saved events.
 */
void WifiConnect::RecordResource::addConnectionEvent(const Event newEvent)
{
    if(!newEvent.isNull())
    {
        connectionEvents.addSorted(eventSorter, newEvent);
        foreachHandler<UpdateInterface>([this, &newEvent]
                (UpdateInterface* listener)
        {
            switch(newEvent.getEventType())
            {
                case EventType::connectionRequested:
                case EventType::startedConnecting:
                    listener->startedConnecting(newEvent.getEventAP());
                    break;
                case EventType::connectionAuthFailed:
                    listener->connectionAuthFailed(newEvent.getEventAP());
                    break;
                case EventType::connected:
                    listener->connected(newEvent.getEventAP());
                    break;
                case EventType::connectionFailed:
                case EventType::disconnected:
                    listener->disconnected(newEvent.getEventAP());
                    break;
                case EventType::invalid:
                    // Invalid events shouldn't be added!
                    jassertfalse;
            }
        });
    }
}

/*
 * Adds a new event to the list of saved events if the most recent saved event 
 * doesn't have the same access point and event type.
 */
void WifiConnect::RecordResource::addEventIfNotDuplicate(const Event newEvent)
{
    Event latestEvent = getLastEvent();
    if(latestEvent.getEventAP() != newEvent.getEventAP()
            || latestEvent.getEventType() != newEvent.getEventType())
    {
        addConnectionEvent(newEvent);
    }
}

/*
 * Removes all saved network connections that match a particular access point.
 */
void WifiConnect::RecordResource::removeSavedConnection
(const AccessPoint toRemove)
{
    const LibNM::ThreadHandler nmThread;
    nmThread.call([this, &toRemove]()
    {
        const APList::NMReader nmList;
        const LibNM::AccessPoint nmAccessPoint 
                = nmList.getStrongestNMAccessPoint(toRemove);
        if(nmAccessPoint.isNull())
        {
            return;
        }
        const juce::Array<LibNM::SavedConnection> matchingConnections 
                = savedConnections.findConnectionsForAP(nmAccessPoint);
        for(LibNM::SavedConnection savedConn : matchingConnections)
        {
            savedConn.deleteConnection();
        }
    });
}

/*
 * Connects with NetworkManager to build the initial set of connection records.
 */
void WifiConnect::RecordResource::updateRecords()
{
    connectionEvents.clear();
    const LibNM::ThreadHandler nmThread;
    nmThread.call([this, &nmThread]()
    {
        const LibNM::DeviceWifi wifiDevice = nmThread.getWifiDevice();
        const LibNM::Client networkClient = nmThread.getClient();
        
        LibNM::ActiveConnection connection 
                = networkClient.getActivatingConnection();
        const bool isConnecting = !connection.isNull();
        if(!isConnecting)
        {
            connection = networkClient.getPrimaryConnection();
        }

        if(connection.isNull())
        {
            return;
        }

        const LibNM::AccessPoint nmAP = wifiDevice.getAccessPoint
                (connection.getAccessPointPath());
        if(nmAP.isNull())
        {
            return;
        }

        const APList::Reader accessPointList;
        AccessPoint activeAP = accessPointList.getAccessPoint
                (nmAP.generateHash());
        jassert(!activeAP.isNull());

        Event initialEvent(activeAP, isConnecting ?
                EventType::startedConnecting : EventType::connected);
        connectionEvents.addSorted(eventSorter, initialEvent);
    });
}

/*
 * Gets the most recent connection event in the connection history.
 */
WifiConnect::Event WifiConnect::RecordResource::getLastEvent() const
{
    return connectionEvents.getFirst();
}

/*
 * Gets the most recent connection event that involves a particular access
 * point.
 */
WifiConnect::Event WifiConnect::RecordResource::getLastEvent
(const AccessPoint eventAP) const
{
    if(eventAP.isNull())
    {
        return Event();
    }
    for(Event& event : connectionEvents)
    {
        if(event.getEventAP() == eventAP)
        {
            return event;
        }
    }
    return Event();
}

/*
 * Gets the most recent connection event with a specific event type.
 */
WifiConnect::Event WifiConnect::RecordResource::getLastEvent
(const EventType eventType) const
{
    if(eventType == EventType::invalid)
    {
        return Event();
    }
    for(Event& event : connectionEvents)
    {
        if(event.getEventType() == eventType)
        {
            return event;
        }
    }
    return Event();
}

/*
 * Gets the most recent connection event with a specific event type and access
 * point.
 */
WifiConnect::Event WifiConnect::RecordResource::getLastEvent
(const AccessPoint eventAP, const EventType eventType) const
{
    if(eventAP.isNull() || eventType == EventType::invalid)
    {
        return Event();
    }
    for(Event& event : connectionEvents)
    {
        if(event.getEventAP() == eventAP && event.getEventType() == eventType)
        {
            return event;
        }
    }
    return Event();
}
