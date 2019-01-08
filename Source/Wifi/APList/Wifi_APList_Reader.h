#pragma once
/**
 * @file  Wifi_APList_Reader.h
 *
 * @brief  Reads the list of visible Wifi access points.
 */

#include "SharedResource_Handler.h"

namespace Wifi { namespace APList { class Reader; } }
namespace Wifi { namespace APList { class ListResource; } }
namespace Wifi { class AccessPoint; }
namespace LibNM { class APHash; }

class Wifi::APList::Reader : public SharedResource::Handler<ListResource>
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
