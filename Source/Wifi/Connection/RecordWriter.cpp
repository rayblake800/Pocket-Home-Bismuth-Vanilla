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
