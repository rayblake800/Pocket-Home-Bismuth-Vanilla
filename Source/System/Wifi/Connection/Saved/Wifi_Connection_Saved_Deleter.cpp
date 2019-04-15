#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Saved_Deleter.h"
#include "Wifi_Connection_Saved_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_AccessPoint.h"

Wifi::Connection::Saved::Deleter::Deleter() { }


// Removes all saved network connections that match a particular access point.
void Wifi::Connection::Saved::Deleter::removeSavedConnection
(const AccessPoint toRemove) const
{
    SharedResource::Modular::LockedPtr<Resource, Module> savedConnections
            = getWriteLockedResource();
    savedConnections->removeSavedConnection(toRemove);
}
