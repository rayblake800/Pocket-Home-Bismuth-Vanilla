#define WIFI_IMPLEMENTATION
#include "Wifi/Connection/RecordWriter.h"
#include "Wifi/Connection/RecordResource.h"
#include "Wifi/Connection/Event.h"
#include "Wifi/AccessPoint/AccessPoint.h"

WifiConnect::RecordWriter::RecordWriter() { }

/*
 * Connects to NetworkManager to rebuild the list of connection records.
 */
void WifiConnect::RecordWriter::updateRecords() const
{
    SharedResource::LockedPtr<RecordResource> wifiRecords
        = getWriteLockedResource();
    wifiRecords->updateRecords();
}

/*
 * Removes all saved network connections that match a particular access point.
 */
void WifiConnect::RecordWriter::removeSavedConnection
(const AccessPoint toRemove) const
{
    SharedResource::LockedPtr<RecordResource> wifiRecords
        = getWriteLockedResource();
    wifiRecords->removeSavedConnection(toRemove);
}

/*
 * Adds a new event to the list of saved events.
 */
void WifiConnect::RecordWriter::addConnectionEvent(const Event newEvent) const
{
    SharedResource::LockedPtr<RecordResource> wifiRecords
        = getWriteLockedResource();
    wifiRecords->addConnectionEvent(newEvent);
}
    
/*
 * Adds a new event to the list of saved events.
 */
void WifiConnect::RecordWriter::addConnectionEvent(const AccessPoint eventAP, 
        const EventType eventType, const juce::Time eventTime)
{
    addConnectionEvent(Event(eventAP, eventType, eventTime));
}

/*
 * Adds a new event to the list of saved events if the most recent saved event 
 * doesn't have the same access point and event type.
 */
void WifiConnect::RecordWriter::addEventIfNotDuplicate(const Event newEvent)
{
    SharedResource::LockedPtr<RecordResource> wifiRecords
        = getWriteLockedResource();
    wifiRecords->addEventIfNotDuplicate(newEvent);
}

/*
 * Adds a new event to the list of saved events if the most recent saved event 
 * doesn't have the same access point and event type.
 */
void WifiConnect::RecordWriter::addEventIfNotDuplicate(
        const AccessPoint eventAP, 
        const EventType eventType, 
        const juce::Time eventTime)
{
    addEventIfNotDuplicate(Event(eventAP, eventType, eventTime));
}
