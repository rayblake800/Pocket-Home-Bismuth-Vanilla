#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_APList_Writer.h
 *
 * @brief  Updates the list of visible Wifi access points.
 */

#include "SharedResource_Handler.h"

namespace Wifi { namespace APList { class Writer; } }
namespace Wifi { namespace APList { class ListResource; } }
namespace Wifi { class AccessPoint; }
namespace LibNM { class AccessPoint; }

class Wifi::APList::Writer : public SharedResource::Handler<ListResource>
{
public:
    Writer();

    virtual ~Writer() { }
    
    /**
     * @brief  Adds a new LibNM::AccessPoint to the list, constructing a
     *         matching Wifi::AccessPoint if one does not yet exist.
     *
     * Like all methods that get or set LibNM::Object objects, this should only 
     * be called within the LibNM::ThreadHandler's call or callAsync methods.
     *
     * @param addedAP  A new LibNM::AccessPoint discovered by NetworkManager.
     */
    void addAccessPoint(const LibNM::AccessPoint addedAP);

    /**
     * @brief  Removes a LibNM::AccessPoint from the list, removing the matching
     *         Wifi::AccessPoint if it no longer has any matching 
     *         LibNM::AccessPoints.
     *
     * Like all methods that get or set LibNM::Object objects, this should only 
     * be called within the LibNM::ThreadHandler's call or callAsync methods.
     *
     * @param removedAP  The LibNM::AccessPoint that NetworkManager can no
     *                   longer find.
     */
    void removeAccessPoint(const LibNM::AccessPoint removedAP);

    /**
     * @brief  Updates the signal strength of an AccessPoint, setting it to the
     *         strongest signal strength of its LibNM::AccessPoints.
     *
     * @param toUpdate  The AccessPoint to update.
     */
    void updateSignalStrength(AccessPoint toUpdate);

    /**
     * @brief  Removes all saved Wifi::AccessPoints and LibNM::AccessPoints.
     */
    void clearAccessPoints();

    /**
     * @brief  Reloads all LibNM::AccessPoints from the NetworkManager, updating
     *         Wifi::AccessPoints as necessary.
     */
    void updateAllAccessPoints();
};
