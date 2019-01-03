#define WIFI_IMPLEMENTATION
#include "Wifi/AccessPointList/NMAPListReader.h"
#include "Wifi/AccessPointList/APList.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"

Wifi::NMAPListReader::NMAPListReader() { }

/*
 * Gets the strongest visible LibNM::AccessPoint that matches a
 * Wifi::AccessPoint.
 */
LibNM::AccessPoint Wifi::NMAPListReader::getStrongestNMAccessPoint
(const AccessPoint accessPoint) const
{
    SharedResource::LockedPtr<APList> apList = getReadLockedResource();
    return apList->getStrongestNMAccessPoint(accessPoint);

}

/*
 * Gets all LibNM::AccessPoint objects described by a Wifi::AccessPoint.
 */
juce::Array<LibNM::AccessPoint> Wifi::NMAPListReader::getNMAccessPoints
(const AccessPoint accessPoint) const
{
    SharedResource::LockedPtr<APList> apList = getReadLockedResource();
    return apList->getNMAccessPoints();
}
