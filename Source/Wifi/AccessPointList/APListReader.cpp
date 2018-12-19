#define WIFI_IMPLEMENTATION
#include "Wifi/AccessPointList/APListReader.h"
#include "Wifi/AccessPointList/APList.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "LibNM/Data/APHash.h"

Wifi::APListReader::APListReader() { }

/*
 * Gets Wifi::AccessPoint objects for all visible access points.
 */
juce::Array<Wifi::AccessPoint> Wifi::APListReader::getAccessPoints() const
{
    SharedResource::LockedPtr<APList> apList = getReadLockedResource();
    return apList->getAccessPoints();
    
}

/*
 * Finds a single Wifi::AccessPoint
 */
Wifi::AccessPoint Wifi::APListReader::getAccessPoint
(LibNM::APHash apHash) const
{
    SharedResource::LockedPtr<APList> apList = getReadLockedResource();
    return apList->getAccessPoint(apHash);
}
