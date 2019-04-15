#pragma once
/**
 * @file  Wifi_APList_Reader.h
 *
 * @brief  Reads the list of visible Wifi access points.
 */

#include "SharedResource_Modular_Handler.h"

namespace Wifi
{
    namespace APList
    {
        class Reader;
        class Module;
    }
    class Resource;
    class AccessPoint;
    namespace LibNM { class APHash; }
}

/**
 * @brief  Connects to the APList::Module to read access point list data.
 */
class Wifi::APList::Reader :
    public SharedResource::Modular::Handler<Resource, Module>
{
public:
    Reader();

    virtual ~Reader() { }

    /**
     * @brief  Gets Wifi::AccessPoint objects for all visible access points.
     *
     * @return  The entire list of visible access points.
     */
    juce::Array<AccessPoint> getAccessPoints() const;

    /**
     * @brief  Finds a single Wifi::AccessPoint using its hash value.
     *
     * @param apHash  An access point hash value.
     *
     * @return        The matching AccessPoint, or a null AccessPoint if no
     *                match is found.
     */
    AccessPoint getAccessPoint(LibNM::APHash apHash) const;
};
