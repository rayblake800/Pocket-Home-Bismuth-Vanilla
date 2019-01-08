#define WIFI_IMPLEMENTATION
#include "Wifi_APList_Writer.h"
#include "Wifi_APList_ListResource.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/ThreadHandler.h"

namespace WifiAPList = Wifi::APList;

WifiAPList::Writer::Writer() { }

/*
 * Adds a new LibNM::AccessPoint to the list, constructing a matching
 * Wifi::AccessPoint if one does not yet exist.
 */
void WifiAPList::Writer::addAccessPoint(const LibNM::AccessPoint addedAP)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &addedAP]()
    {
        SharedResource::LockedPtr<ListResource> apList = getWriteLockedResource();
        apList->addAccessPoint(addedAP);
    });
}

/*
 * Removes a LibNM::AccessPoint from the list, removing the matching
 * Wifi::AccessPoint if it no longer has any matching LibNM::AccessPoints.
 */
void WifiAPList::Writer::removeAccessPoint(const LibNM::AccessPoint removedAP)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &removedAP]()
    {
        SharedResource::LockedPtr<ListResource> apList = getWriteLockedResource();
        apList->removeAccessPoint(removedAP);
    });
}

/*
 * Updates the signal strength of an AccessPoint, setting it to the strongest
 * signal strength of its LibNM::AccessPoints.
 */
void WifiAPList::Writer::updateSignalStrength(AccessPoint toUpdate)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &toUpdate]()
    {
        SharedResource::LockedPtr<ListResource> apList = getWriteLockedResource();
        apList->updateSignalStrength(toUpdate);
    });
}

/*
 * Removes all saved Wifi::AccessPoints and LibNM::AccessPoints.
 */
void WifiAPList::Writer::clearAccessPoints()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this]()
    {
        SharedResource::LockedPtr<ListResource> apList = getWriteLockedResource();
        apList->clearAccessPoints();
    });
}

/*
 * Reloads all LibNM::AccessPoints from the NetworkManager, updating
 * Wifi::AccessPoints as necessary.
 */
void WifiAPList::Writer::updateAllAccessPoints()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this]()
    {
        SharedResource::LockedPtr<ListResource> apList = getWriteLockedResource();
        apList->updateAllAccessPoints();
    });
}
