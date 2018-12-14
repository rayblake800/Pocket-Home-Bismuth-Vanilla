#include "Wifi/AccessPointList/APList.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPoint/SignalUpdateInterface.h"
#include "Wifi/AccessPointList/APUpdateInterface.h"
#include "LibNM/APData/APHash.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/NMObjects/DeviceWifi.h"
#include "LibNM/ThreadHandler.h"

/* SharedResource object instance key: */
const juce::Identifier Wifi::APList::resourceKey = "Wifi::APList";

/* All visible Wifi::AccessPoint objects, mapped by hash value. */
static std::map<LibNM::APHash, Wifi::AccessPoint> wifiAccessPoints;

/* All visible LibNM::AccessPoint objects, mapped by hash value. */
static std::map<LibNM::APHash, juce::Array<LibNM::AccessPoint>> nmAccessPoints;

/*
 * Reads initial access point data from LibNM.
 */
Wifi::APList::APList() : SharedResource::Resource(resourceKey)
{
    clearAccessPoints();
    updateAllAccessPoints();
}

/*
 * Gets Wifi::AccessPoint objects for all visible access points.
 */
juce::Array<Wifi::AccessPoint> Wifi::APList::getAccessPoints() const
{
    juce::Array<Wifi::AccessPoint> accessPoints;
    for(auto apIterator : wifiAccessPoints)
    {
        accessPoints.add(apIterator.second);
    }
    return accessPoints;
}

/*
 * Finds a single Wifi::AccessPoint
 */
Wifi::AccessPoint Wifi::APList::getAccessPoint(LibNM::APHash apHash) const
{
    try
    {
        return wifiAccessPoints.at(apHash);
    }
    catch(std::out_of_range e)
    {
        return Wifi::AccessPoint();
    }
}

/*
 * Gets the strongest visible LibNM::AccessPoint that matches a
 * Wifi::AccessPoint.
 */
LibNM::AccessPoint Wifi::APList::getStrongestNMAccessPoint
(const AccessPoint accessPoint) const
{
    LibNM::AccessPoint strongestAP;
    juce::Array<LibNM::AccessPoint>& apList 
            = nmAccessPoints[accessPoint.getHashValue()];
    int bestSignalStrength = 0;
    for(const LibNM::AccessPoint& nmAP : apList)
    {
        const int apStrength = nmAP.getSignalStrength();
        if(apStrength > bestSignalStrength)
        {
            bestSignalStrength = apStrength;
            strongestAP = nmAP;
        }
    }
    return strongestAP;
}

/*
 * Gets LibNM::AccessPoint objects for all access point devices visible through
 * the Wifi device.
 */
juce::Array<LibNM::AccessPoint> Wifi::APList::getNMAccessPoints() const
{
    juce::Array<LibNM::AccessPoint> nmAPs;
    for(auto apListIter : nmAccessPoints)
    {
        for(LibNM::AccessPoint& accessPoint : apListIter.second)
        {
            nmAPs.add(accessPoint);
        }
    }
    return nmAPs;
}

/*
 * Gets all LibNM::AccessPoint objects described by a Wifi::AccessPoint.
 */
juce::Array<LibNM::AccessPoint> Wifi::APList::getNMAccessPoints
(const AccessPoint accessPoint) const
{
    return nmAccessPoints[accessPoint.getHashValue()];
}

/*
 * Adds a new LibNM::AccessPoint to the list, constructing a matching
 * Wifi::AccessPoint if one does not yet exist.
 */
void Wifi::APList::addAccessPoint(const LibNM::AccessPoint addedAP)
{
    jassert(!addedAP.isNull());
    const LibNM::APHash apHash = addedAP.generateHash();
    nmAccessPoints[apHash].addIfNotAlreadyThere(addedAP);
    if(wifiAccessPoints.count(apHash) == 0)
    {
        wifiAccessPoints[apHash] = AccessPoint(addedAP);
        foreachHandler<APUpdateInterface>([&apHash]
                (APUpdateInterface* updateHandler)
        {
            updateHandler->accessPointAdded(wifiAccessPoints[apHash]);
        });
    }
    
    // Update signal strength if necessary.
    AccessPoint& wifiAP = wifiAccessPoints[apHash];
    const unsigned int newSignalStrength = addedAP.getSignalStrength();
    if(newSignalStrength > wifiAP.getSignalStrength())
    {
        wifiAP.setSignalStrength(newSignalStrength);
        AccessPoint updatedAP = wifiAccessPoints[apHash];
        foreachHandler<SignalUpdateInterface>([&updatedAP]
                (SignalUpdateInterface* updateHandler)
        {
            updateHandler->signalStrengthChanged(updatedAP);
        });
    }
}

/*
 * Removes a LibNM::AccessPoint from the list, removing the matching
 * Wifi::AccessPoint if it no longer has any matching LibNM::AccessPoints.
 */
void Wifi::APList::removeAccessPoint(const LibNM::AccessPoint removedAP)
{
    const LibNM::APHash apHash = removedAP.generateHash();
    nmAccessPoints[apHash].removeAllInstancesOf(removedAP);
    if(nmAccessPoints[apHash].isEmpty())
    {
        const AccessPoint toRemove = wifiAccessPoints[apHash];
        wifiAccessPoints.erase(apHash);
        foreachHandler<APUpdateInterface>([&toRemove]
                (APUpdateInterface* updateHandler)
        {
            updateHandler->accessPointRemoved(toRemove);
        });
    }
}

/*
 * Updates the signal strength of an AccessPoint, setting it to the strongest
 * signal strength of its LibNM::AccessPoints.
 */
void Wifi::APList::updateSignalStrength(AccessPoint toUpdate)
{
    const LibNM::APHash apHash = toUpdate.getHashValue();
    const unsigned int oldSignalStrength 
        = wifiAccessPoints[apHash].getSignalStrength();
    unsigned int bestSignalStrength = 0;
    for(LibNM::AccessPoint& accessPoint : nmAccessPoints[apHash])
    {
        bestSignalStrength = std::max(bestSignalStrength, 
                accessPoint.getSignalStrength());
    }
    if(bestSignalStrength != oldSignalStrength)
    {
        toUpdate.setSignalStrength(bestSignalStrength);
        foreachHandler<SignalUpdateInterface>([&toUpdate]
                (SignalUpdateInterface* updateHandler)
        {
            updateHandler->signalStrengthChanged(toUpdate);
        });
    }
}

/*
 * Removes all saved Wifi::AccessPoints and LibNM::AccessPoints.
 */
void Wifi::APList::clearAccessPoints()
{
    LibNM::ThreadHandler nmThread;
    nmThread.call([]()
    {
        wifiAccessPoints.clear();
        nmAccessPoints.clear();
    });
}

/*
 * Reloads all LibNM::AccessPoints from the NetworkManager, updating
 * Wifi::AccessPoints as necessary.
 */
void Wifi::APList::updateAllAccessPoints()
{
    LibNM::ThreadHandler nmThread;
    nmThread.call([this, &nmThread]()
    {
        LibNM::DeviceWifi wifiDevice = nmThread.getWifiDevice(); 

        juce::Array<LibNM::AccessPoint> nmAPs = wifiDevice.getAccessPoints();
        for(LibNM::AccessPoint& nmAccessPoint : nmAPs)
        {
            LibNM::APHash apHash = nmAccessPoint.generateHash();
            wifiAccessPoints[apHash] = Wifi::AccessPoint(nmAccessPoint);
            nmAccessPoints[apHash].add(nmAccessPoint);
        }
    });
}
