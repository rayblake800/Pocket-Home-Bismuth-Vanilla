#include "Wifi/AccessPointList/NMAPListReader.h"

Wifi::NMAPListReader::NMAPListReader()
{
}

/*
 * Gets the strongest visible LibNM::AccessPoint that matches a
 * Wifi::AccessPoint.
 */
LibNM::AccessPoint Wifi::NMAPListReader::getStrongestNMAccessPoint
(const AccessPoint accessPoint) const
{
}

/*
 * Gets all LibNM::AccessPoint objects described by a Wifi::AccessPoint.
 */
juce::Array<LibNM::AccessPoint> Wifi::NMAPListReader::getNMAccessPoints
(const AccessPoint accessPoint) const
{
}