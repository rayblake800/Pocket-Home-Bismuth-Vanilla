#pragma once
/**
 * @file  Wifi/AccessPointList/APListReader.h
 *
 * @brief  Reads the list of visible Wifi access points.
 */

#include "SharedResource/Handler.h"

namespace Wifi { class APListReader; }
namespace Wifi { class APList; }
namespace Wifi { class AccessPoint; }
namespace LibNM { class APHash; }

class Wifi::APListReader : public SharedResource::Handler<APList>
{
public:
    APListReader();

    virtual ~APListReader() { }
    
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
