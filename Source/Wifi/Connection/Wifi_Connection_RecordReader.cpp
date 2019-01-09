#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_RecordReader.h"
#include "Wifi_Connection_RecordResource.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_AccessPoint.h"

namespace WifiConnect = Wifi::Connection;

/*
 * Initializes the RecordResource if necessary.
 */
WifiConnect::RecordReader::RecordReader() { }

/*
 * Checks if the system has an active, established Wifi network connection.
 */
bool WifiConnect::RecordReader::isConnected() const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->isConnected();
}

/*
 * Checks if NetworkManager is currently opening a network connection.
 */
bool WifiConnect::RecordReader::isConnecting() const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->isConnecting();
}

/*
 * Gets the access point being used by the active or activating connection.
 */
Wifi::AccessPoint WifiConnect::RecordReader::getActiveAP() const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->getActiveAP();
}

/*
 * Checks if NetworkManager has a saved connection that is compatible with an
 * access point.
 */
bool WifiConnect::RecordReader::hasSavedConnection
(const AccessPoint toCheck) const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->hasSavedConnection(toCheck);
}

/*
 * Finds the last time the system was fully connected to a particular wifi 
 * access point's connection.
 */
juce::Time WifiConnect::RecordReader::lastConnectionTime
(const AccessPoint connectionAP) const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->lastConnectionTime(connectionAP);
}

/*
 * Gets the most recent connection event in the connection history.
 */
WifiConnect::Event WifiConnect::RecordReader::getLastEvent() const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->getLastEvent();
}

/*
 * Gets the most recent connection event that involves a particular access 
 * point.
 */
WifiConnect::Event WifiConnect::RecordReader::getLastEvent
(const AccessPoint eventAP) const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->getLastEvent(eventAP);
}

/*
 * Gets the most recent connection event with a specific event type.
 */
WifiConnect::Event WifiConnect::RecordReader::getLastEvent
(const EventType eventType) const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->getLastEvent(eventType);
}

/*
 * Gets the most recent connection event with a specific event type and access 
 * point.
 */
WifiConnect::Event WifiConnect::RecordReader::getLastEvent
(const AccessPoint eventAP, const EventType eventType) const
{
    SharedResource::LockedPtr<RecordResource> connectionRecord
        = getReadLockedResource();
    return connectionRecord->getLastEvent(eventAP, eventType);
}
