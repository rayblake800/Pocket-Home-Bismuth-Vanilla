#include "Wifi/Connection/History/ConnectionHistory.h"

Wifi::ConnectionHistory::ConnectionHistory() { }

/* Sorts the event list from newest to oldest. */
class
{
public:
    static int compareElements(Wifi::ConnectionEvent first, 
            Wifi::ConnectionEvent second)
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
 * Adds a new event to the list of saved events.
 */
void Wifi::ConnectionHistory::addConnectionEvent
(const ConnectionEvent newEvent)
{
    if(!newEvent.isNull())
    {
        connectionEvents.addSorted(eventSorter, newEvent);
    }
}

/*
 * Gets the most recent connection event in the connection history.
 */
Wifi::ConnectionEvent Wifi::ConnectionHistory::getLastEvent() const
{
    return connectionEvents.getFirst();
}

/*
 * Gets the most recent connection event that involves a particular access
 * point.
 */
Wifi::ConnectionEvent Wifi::ConnectionHistory::getLastEvent
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
Wifi::ConnectionEvent Wifi::ConnectionHistory::getLastEvent
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
Wifi::ConnectionEvent Wifi::ConnectionHistory::getLastEvent
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
