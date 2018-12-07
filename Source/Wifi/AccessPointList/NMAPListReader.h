#pragma once
#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi/AccessPointList/NMAPListReader.h
 *
 * @brief  Gets LibNM::AccessPoints held by the APList resource.
 */

#include "SharedResource/Handler.h"

namespace Wifi { class NMAPListReader; }
namespace Wifi { class APList; }
namespace Wifi { class AccessPoint; }
namespace LibNM { class AccessPoint; }
namespace LibNM { class APHash; }

class Wifi::NMAPListReader : public SharedResource::Handler<APList>
{
public:
    NMAPListReader();

    virtual ~NMAPListReader() { }

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
