#define WIFI_IMPLEMENTATION
#include "Wifi_APList_Writer.h"
#include "Wifi_APList_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ContextTest.h"

Wifi::APList::Writer::Writer() { }

/*
 * Adds a new LibNM::AccessPoint to the list, constructing a matching
 * Wifi::AccessPoint if one does not yet exist.
 */
void Wifi::APList::Writer::addAccessPoint(const LibNM::AccessPoint addedAP)
{
    ASSERT_NM_CONTEXT;
    SharedResource::Modular::LockedPtr<Resource, Module> apList 
            = getWriteLockedResource();
    apList->addAccessPoint(addedAP);
}

/*
 * Removes a LibNM::AccessPoint from the list, removing the matching
 * Wifi::AccessPoint if it no longer has any matching LibNM::AccessPoints.
 */
void Wifi::APList::Writer::removeAccessPoint(const LibNM::AccessPoint removedAP)
{
    ASSERT_NM_CONTEXT;
    SharedResource::Modular::LockedPtr<Resource, Module> apList 
            = getWriteLockedResource();
    apList->removeAccessPoint(removedAP);
}

/*
 * Updates the signal strength of an AccessPoint, setting it to the strongest
 * signal strength of its LibNM::AccessPoints.
 */
void Wifi::APList::Writer::updateSignalStrength(AccessPoint toUpdate)
{
    ASSERT_NM_CONTEXT;
    SharedResource::Modular::LockedPtr<Resource, Module> apList 
            = getWriteLockedResource();
    apList->updateSignalStrength(toUpdate);
}

/*
 * Removes all saved Wifi::AccessPoints and LibNM::AccessPoints.
 */
void Wifi::APList::Writer::clearAccessPoints()
{
    ASSERT_NM_CONTEXT;
    SharedResource::Modular::LockedPtr<Resource, Module> apList 
            = getWriteLockedResource();
    apList->clearAccessPoints();
}

/*
 * Reloads all LibNM::AccessPoints from the NetworkManager, updating
 * Wifi::AccessPoint objects as necessary.
 */
void Wifi::APList::Writer::updateAllAccessPoints()
{
    ASSERT_NM_CONTEXT;
    SharedResource::Modular::LockedPtr<Resource, Module> apList 
            = getWriteLockedResource();
    apList->updateAllAccessPoints();
}

/*
 * Removes all LibNM::AccessPoint objects that are no longer valid.
 */
void Wifi::APList::Writer::removeInvalidatedAccessPoints()
{
    ASSERT_NM_CONTEXT;
    SharedResource::Modular::LockedPtr<Resource, Module> apList 
            = getWriteLockedResource();
    apList->removeInvalidatedAccessPoints();
}
