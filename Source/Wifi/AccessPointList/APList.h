#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi/AccessPoint/APList.h
 *
 * @brief  Tracks all visible Wifi access points, using NetworkManager signals
 *         and data to construct and update Wifi::AccessPoint objects.
 */

#include "Wifi/AccessPoint/AccessPoint.h"
#include "LibNM/APData/APHash.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "SharedResource/Resource.h"

namespace Wifi { class APList; }

class Wifi::APList : public SharedResource::Resource
{
public:
    /* SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Reads initial access point data from LibNM, using it to construct
     *         the access point list.
     */
    APList();

    virtual ~APList() { }

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

    /**
     * @brief  Adds a new LibNM::AccessPoint to the list, constructing a
     *         matching Wifi::AccessPoint if one does not yet exist.
     *
     * @param addedAP  A new LibNM::AccessPoint discovered by NetworkManager.
     */
    void addAccessPoint(const LibNM::AccessPoint addedAP);

    /**
     * @brief  Removes a LibNM::AccessPoint from the list, removing the matching
     *         Wifi::AccessPoint if it no longer has any matching 
     *         LibNM::AccessPoints.
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

private:
    /* All visible Wifi::AccessPoint objects, mapped by hash value. */
    std::map<LibNM::APHash, AccessPoint> wifiAccessPoints;
    
    /* All visible LibNM::AccessPoint objects, mapped by hash value. */
    std::map<LibNM::APHash, juce::Array<LibNM::AccessPoint>> nmAccessPoints;
};
