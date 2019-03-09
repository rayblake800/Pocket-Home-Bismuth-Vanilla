#pragma once
#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi_APList_NMReader.h
 *
 * @brief  Gets LibNM::AccessPoints held by the APList resource.
 */

#include "SharedResource_Modular_Handler.h"

namespace Wifi 
{ 
    namespace APList 
    { 
        class NMReader;
        class Module;
    } 
    class Resource;
    class AccessPoint;
    namespace LibNM { class AccessPoint; }
}

class Wifi::APList::NMReader : 
    public SharedResource::Modular::Handler<Resource, Module>
{
public:
    NMReader();

    virtual ~NMReader() { }

    /**
     * @brief  Gets the strongest visible LibNM::AccessPoint that matches a
     *         Wifi::AccessPoint.
     *
     * @param accessPoint  A Wifi::AccessPoint describing one or more 
     *                     LibNM::AccessPoints. 
     *
     * @return             The corresponding LibNM::AccessPoint with the 
     *                     strongest signal strength, or a null 
     *                     LibNM::AccessPoint if no matching LibNM::AccessPoint
     *                     is found.
     */
    LibNM::AccessPoint getStrongestNMAccessPoint(const AccessPoint accessPoint) 
        const;

    /**
     * @brief  Gets all LibNM::AccessPoint objects described by a 
     *         Wifi::AccessPoint.
     *
     * @param accessPoint  A Wifi::AccessPoint describing one or more
     *                     LibNM::AccessPoints.
     *
     * @return             All distinct LibNM::AccessPoint objects that share an
     *                     APHash with accessPoint.
     */
    juce::Array<LibNM::AccessPoint> getNMAccessPoints
        (const AccessPoint accessPoint) const;

};
