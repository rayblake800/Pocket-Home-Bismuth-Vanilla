#include "Wifi/Connection/RecordResource.h"
#include "Wifi/Connection/Event.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPointList/APListReader.h"
#include "Wifi/AccessPointList/NMAPListReader.h"
#include "LibNM/ThreadHandler.h"

namespace WifiConnect = Wifi::Connection;

/* SharedResource object instance key: */
const juce::Identifier WifiConnect::RecordResource::resourceKey 
        = "Wifi::Connection::RecordResource";

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
WifiConnect::RecordResource::RecordResource() 
{
    connectionEvents.clear();
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
        NMAPListReader nmList;
        LibNM::AccessPoint nmAccessPoint 
                = nmList.getStrongestNMAccessPoint(toCheck);
        if(nmAccessPoint.isNull())
        {
            return;
        }
        matchFound = !savedConnections.findConnectionsForAP(nmAccessPoint)
                        .isEmpty();
    });
    return matchFound;
}

/*
 * Adds a new event to the list of saved events.
 */
void WifiConnect::RecordResource::addConnectionEvent(const Event newEvent)
{
    if(!newEvent.isNull())
    {
        connectionEvents.addSorted(eventSorter, newEvent);
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
        const NMAPListReader nmList;
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
    const LibNM::ThreadHandler nmThread;
    nmThread.call([this]()
    {
        APListReader apList;
        NMAPListReader nmList;
        juce::Array<AccessPoint> visibleAPs = apList.getAccessPoints();
        for(const AccessPoint& ap : visibleAPs)
        {
            
        }
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
        return ConnectionEvent();
    }
    for(ConnectionEvent& event : connectionEvents)
    {
        if(event.getEventAP() == eventAP)
        {
            return event;
        }
    }
    return ConnectionEvent();
}

/*
 * Gets the most recent connection event with a specific event type.
 */
WifiConnect::Event WifiConnect::RecordResource::getLastEvent
(const EventType eventType) const
{
    if(eventType == EventType::invalid)
    {
        return ConnectionEvent();
    }
    for(ConnectionEvent& event : connectionEvents)
    {
        if(event.getEventType() == eventType)
        {
            return event;
        }
    }
    return ConnectionEvent();
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
        return ConnectionEvent();
    }
    for(ConnectionEvent& event : connectionEvents)
    {
        if(event.getEventAP() == eventAP && event.getEventType() == eventType)
        {
            return event;
        }
    }
    return ConnectionEvent();
}
