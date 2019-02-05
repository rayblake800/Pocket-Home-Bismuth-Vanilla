#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_AccessPoint.h"

namespace WifiRecord = Wifi::Connection::Record;

/*
 * Initializes the RecordModule if necessary.
 */
WifiRecord::Reader::Reader() { }

/*
 * Checks if the system has an active, established Wifi network connection.
 */
bool WifiRecord::Reader::isConnected() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->isConnected();
}

/*
 * Checks if NetworkManager is currently opening a network connection.
 */
bool WifiRecord::Reader::isConnecting() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->isConnecting();
}

/*
 * Gets the access point being used by the active or activating connection.
 */
Wifi::AccessPoint WifiRecord::Reader::getActiveAP() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getActiveAP();
}

/*
 * Gets the most recent connection event in the connection history.
 */
Wifi::Connection::Event WifiRecord::Reader::getLatestEvent() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent();
}

/*
 * Gets the most recent connection event that involves a particular access 
 * point.
 */
Wifi::Connection::Event WifiRecord::Reader::getLatestEvent
(const AccessPoint eventAP) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent(eventAP);
}

/*
 * Gets the most recent connection event with a specific event type.
 */
Wifi::Connection::Event WifiRecord::Reader::getLatestEvent
(const EventType eventType) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent(eventType);
}

/*
 * Gets the most recent connection event with a specific event type and access 
 * point.
 */
Wifi::Connection::Event WifiRecord::Reader::getLatestEvent
(const AccessPoint eventAP, const EventType eventType) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent(eventAP, eventType);
}
