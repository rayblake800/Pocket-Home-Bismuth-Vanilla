#define WIFI_IMPLEMENTATION
#include "Wifi/AccessPointList/APListWriter.h"
#include "Wifi/AccessPointList/APList.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/ThreadHandler.h"

Wifi::APListWriter::APListWriter() { }

/*
 * Adds a new LibNM::AccessPoint to the list, constructing a matching
 * Wifi::AccessPoint if one does not yet exist.
 */
void Wifi::APListWriter::addAccessPoint(const LibNM::AccessPoint addedAP)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &addedAP]()
    {
        SharedResource::LockedPtr<APList> apList = getWriteLockedResource();
        apList->addAccessPoint(addedAP);
    });
}

/*
 * Removes a LibNM::AccessPoint from the list, removing the matching
 * Wifi::AccessPoint if it no longer has any matching LibNM::AccessPoints.
 */
void Wifi::APListWriter::removeAccessPoint(const LibNM::AccessPoint removedAP)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &removedAP]()
    {
        SharedResource::LockedPtr<APList> apList = getWriteLockedResource();
        apList->removeAccessPoint(removedAP);
    });
}

/*
 * Updates the signal strength of an AccessPoint, setting it to the strongest
 * signal strength of its LibNM::AccessPoints.
 */
void Wifi::APListWriter::updateSignalStrength(AccessPoint toUpdate)
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this, &toUpdate]()
    {
        SharedResource::LockedPtr<APList> apList = getWriteLockedResource();
        apList->updateSignalStrength(toUpdate);
    });
}

/*
 * Removes all saved Wifi::AccessPoints and LibNM::AccessPoints.
 */
void Wifi::APListWriter::clearAccessPoints()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this]()
    {
        SharedResource::LockedPtr<APList> apList = getWriteLockedResource();
        apList->clearAccessPoints();
    });
}

/*
 * Reloads all LibNM::AccessPoints from the NetworkManager, updating
 * Wifi::AccessPoints as necessary.
 */
void Wifi::APListWriter::updateAllAccessPoints()
{
    const LibNM::ThreadHandler nmThreadHandler;
    nmThreadHandler.call([this]()
    {
        SharedResource::LockedPtr<APList> apList = getWriteLockedResource();
        apList->updateAllAccessPoints();
    });
}
