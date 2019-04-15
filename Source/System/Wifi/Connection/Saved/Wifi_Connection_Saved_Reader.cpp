#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Saved_Reader.h"
#include "Wifi_Connection_Saved_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_Connection.h"

// Initializes the Wifi::Resource object if necessary.
Wifi::Connection::Saved::Reader::Reader() { }


// Checks if NetworkManager has a saved connection that is compatible with an
// access point.
bool Wifi::Connection::Saved::Reader::hasSavedConnection
(const AccessPoint toCheck)
{
    SharedResource::Modular::LockedPtr<Resource, Module> savedConnections
            = getWriteLockedResource();
    return savedConnections->hasSavedConnection(toCheck);
}


// Attempts to find and return a saved connection object that is compatible
// with an access point.
Wifi::LibNM::Connection Wifi::Connection::Saved::Reader::getSavedConnection
(const AccessPoint connectionAP)
{
    SharedResource::Modular::LockedPtr<Resource, Module> savedConnections
            = getWriteLockedResource();
    return savedConnections->getSavedConnection(connectionAP);
}


// Finds the last time the system was fully connected to a particular wifi
// access point's connection.
juce::Time Wifi::Connection::Saved::Reader::lastConnectionTime
(const AccessPoint connectionAP)
{
    SharedResource::Modular::LockedPtr<Resource, Module> savedConnections
            = getWriteLockedResource();
    return savedConnections->lastConnectionTime(connectionAP);
}
