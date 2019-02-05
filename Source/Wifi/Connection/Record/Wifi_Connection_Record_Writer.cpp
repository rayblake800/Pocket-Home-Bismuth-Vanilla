#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Record_Writer.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_AccessPoint.h"

namespace WifiRecord = Wifi::Connection::Record;

WifiRecord::Writer::Writer() { }

/*
 * Connects to NetworkManager to rebuild the list of connection records.
 */
void WifiRecord::Writer::updateRecords() const
{
    SharedResource::Modular::LockedPtr<Resource, Module> connectionRecord
        = getWriteLockedResource();
    connectionRecord->updateRecords();
}

/*
 * Adds a new event to the list of saved events.
 */
void WifiRecord::Writer::addConnectionEvent(const Event newEvent) const
{
    SharedResource::Modular::LockedPtr<Resource, Module> connectionRecord
        = getWriteLockedResource();
    connectionRecord->addConnectionEvent(newEvent);
}
    
/*
 * Adds a new event to the list of saved events.
 */
void WifiRecord::Writer::addConnectionEvent(const AccessPoint eventAP, 
        const EventType eventType, const juce::Time eventTime)
{
    addConnectionEvent(Event(eventAP, eventType, eventTime));
}

/*
 * Adds a new event to the list of saved events if the most recent saved event 
 * doesn't have the same access point and event type.
 */
void WifiRecord::Writer::addEventIfNotDuplicate(const Event newEvent)
{
    SharedResource::Modular::LockedPtr<Resource, Module> connectionRecord
        = getWriteLockedResource();
    connectionRecord->addEventIfNotDuplicate(newEvent);
}

/*
 * Adds a new event to the list of saved events if the most recent saved event 
 * doesn't have the same access point and event type.
 */
void WifiRecord::Writer::addEventIfNotDuplicate(
        const AccessPoint eventAP, 
        const EventType eventType, 
        const juce::Time eventTime)
{
    addEventIfNotDuplicate(Event(eventAP, eventType, eventTime));
}
