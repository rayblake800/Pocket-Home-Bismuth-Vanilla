#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Record_Handler.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_AccessPoint.h"

namespace WifiRecord = Wifi::Connection::Record;

// Initializes the RecordModule if necessary.
WifiRecord::Handler::Handler() { }


// Checks if the system has an active, established Wifi network connection.
bool WifiRecord::Handler::isConnected() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->isConnected();
}


// Checks if NetworkManager is currently opening a network connection.
bool WifiRecord::Handler::isConnecting() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->isConnecting();
}


// Gets the access point being used by the active or activating connection.
Wifi::AccessPoint WifiRecord::Handler::getActiveAP() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getActiveAP();
}


// Gets the most recent connection event in the connection history.
Wifi::Connection::Event WifiRecord::Handler::getLatestEvent() const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent();
}


// Gets the most recent connection event that involves a particular access
// point.
Wifi::Connection::Event WifiRecord::Handler::getLatestEvent
(const AccessPoint eventAP) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent(eventAP);
}


// Gets the most recent connection event with a specific event type.
Wifi::Connection::Event WifiRecord::Handler::getLatestEvent
(const EventType eventType) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent(eventType);
}


// Gets the most recent connection event with a specific event type and access
// point.
Wifi::Connection::Event WifiRecord::Handler::getLatestEvent
(const AccessPoint eventAP, const EventType eventType) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> recordModule
            = getReadLockedResource();
    return recordModule->getLatestEvent(eventAP, eventType);
}


// Connects with NetworkManager to initialize or update the most recent
// connection record.
void WifiRecord::Handler::updateRecords()
{
    SharedResource::Modular::LockedPtr<Resource, Module> recordModule
            = getWriteLockedResource();
    recordModule->updateRecords();
}
