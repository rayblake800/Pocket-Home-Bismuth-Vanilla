#define WIFI_IMPLEMENTATION
#include "Wifi_APList_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_Connection_Saved_Module.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_AP_UpdateInterface.h"
#include "Wifi_APList_UpdateInterface.h"
#include "Wifi_LibNM_APHash.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_LibNM_Thread_Module.h"
#include <map>

namespace WifiAPList = Wifi::APList;
namespace WifiNM = Wifi::LibNM;

/* All visible Wifi::AccessPoint objects, mapped by hash value. */
static std::map<WifiNM::APHash, Wifi::AccessPoint> wifiAccessPoints;

/* All visible LibNM::AccessPoint objects, mapped by hash value. */
static std::map<WifiNM::APHash, juce::Array<WifiNM::AccessPoint>> 
        nmAccessPoints;

#ifdef JUCE_DEBUG
/* Print the full class name before debug messages: */
static const constexpr char* dbgPrefix = "Wifi::APList::Module::";

/**
 * @brief Prints the access point list's data maps for debugging.
 */
void printAPList()
{
    DBG(dbgPrefix << __func__ << ": " << wifiAccessPoints.size()
            << " access points found.");
    for(auto& iter : wifiAccessPoints)
    {
        DBG("  [" << iter.first.toString() << "] ("
                << nmAccessPoints[iter.first].size() <<  " LibNM AP)");
        DBG("   = " << iter.second.toString());
    }
}
#endif

/*
 * Reads initial access point data from LibNM.
 */
WifiAPList::Module::Module(Resource& wifiResource) : 
    SharedResource::Modular::Module<Resource>(wifiResource) 
{ 
    DBG(dbgPrefix << __func__ << ": Creating Wifi::APList module.");
}

/*
 * Clears all access point data on destruction.
 */
WifiAPList::Module::~Module()
{
    wifiAccessPoints.clear();
    nmAccessPoints.clear();
    DBG(dbgPrefix << __func__ << ": Finished destroying Wifi::APList module.");
}

/*
 * Gets Wifi::AccessPoint objects for all visible access points.
 */
juce::Array<Wifi::AccessPoint> 
WifiAPList::Module::getAccessPoints() const
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
Wifi::AccessPoint 
WifiAPList::Module::getAccessPoint(LibNM::APHash apHash) const
{
    try
    {
        jassert(!apHash.isNull());
        AccessPoint matchingAP = wifiAccessPoints.at(apHash);
        jassert(!matchingAP.isNull());
        jassert(matchingAP.getHashValue() == apHash);
        return matchingAP;
    }
    catch(std::out_of_range e)
    {
        DBG(dbgPrefix << __func__ << ": Failed to find hash value \""
                << apHash.toString() << "\"");
        DBG(dbgPrefix << __func__ << ": Full access point list:");
        printAPList();
        return Wifi::AccessPoint();
    }
}

/*
 * Gets the strongest visible LibNM::AccessPoint that matches a
 * Wifi::AccessPoint.
 */
WifiNM::AccessPoint WifiAPList::Module::getStrongestNMAccessPoint
(const AccessPoint accessPoint) const
{
    ASSERT_NM_CONTEXT;
    juce::Array<LibNM::AccessPoint>& apList 
            = nmAccessPoints[accessPoint.getHashValue()];
    apList.removeIf([](LibNM::AccessPoint ap) 
    { 
        return ap.isNull(); 
    });

    LibNM::AccessPoint strongestAP;
    int bestSignalStrength = 0;
    for(LibNM::AccessPoint& nmAccessPoint : apList)
    {
        const int apStrength = nmAccessPoint.getSignalStrength();
        if(apStrength > bestSignalStrength)
        {
            bestSignalStrength = apStrength;
            strongestAP = nmAccessPoint;
        }
    }
    return strongestAP;
}

/*
 * Gets LibNM::AccessPoint objects for all access point devices visible through
 * the Wifi device.
 */
juce::Array<WifiNM::AccessPoint> WifiAPList::Module::getNMAccessPoints() 
        const
{
    ASSERT_NM_CONTEXT;
    juce::Array<LibNM::AccessPoint> nmAPs;
    for(auto& apListIter : nmAccessPoints)
    {
        nmAPs.addArray(apListIter.second);
    }
    return nmAPs;
}

/*
 * Gets all LibNM::AccessPoint objects described by a Wifi::AccessPoint.
 */
juce::Array<WifiNM::AccessPoint> WifiAPList::Module::getNMAccessPoints
(const AccessPoint accessPoint) const
{
    ASSERT_NM_CONTEXT;
    juce::Array<LibNM::AccessPoint>& matchingAPs
        = nmAccessPoints[accessPoint.getHashValue()];
    matchingAPs.removeIf([](LibNM::AccessPoint ap) 
    { 
        return ap.isNull(); 
    });

    return matchingAPs;
}

/*
 * Adds a new LibNM::AccessPoint to the list, constructing a matching
 * Wifi::AccessPoint if one does not yet exist.
 */
void WifiAPList::Module::addAccessPoint(const LibNM::AccessPoint addedAP)
{
    ASSERT_NM_CONTEXT;
    jassert(!addedAP.isNull());
    const LibNM::APHash apHash = addedAP.generateHash();
    jassert(!apHash.isNull());
    nmAccessPoints[apHash].addIfNotAlreadyThere(addedAP);
    if(wifiAccessPoints.count(apHash) == 0)
    {
        Connection::Saved::Module* savedConnections
                = getSiblingModule<Connection::Saved::Module>();
        AccessPoint newWifiAP(addedAP);
        savedConnections->updateSavedAPData(newWifiAP);
        jassert(!newWifiAP.isNull());
        jassert(newWifiAP.getHashValue() == apHash);
        wifiAccessPoints[apHash] = newWifiAP;
        foreachHandler<UpdateInterface>([&newWifiAP]
                (UpdateInterface* updateHandler)
        {
            updateHandler->accessPointAdded(newWifiAP);
        });
    }
    
    // Update signal strength if necessary.
    AccessPoint& wifiAP = wifiAccessPoints[apHash];
    const unsigned int newSignalStrength = addedAP.getSignalStrength();
    if(newSignalStrength > wifiAP.getSignalStrength())
    {
        static_cast<APInterface::SignalStrength*>(&wifiAP)->setSignalStrength
                (newSignalStrength);
        AccessPoint updatedAP = wifiAccessPoints[apHash];
        foreachHandler<AP::UpdateInterface>([&updatedAP]
                (AP::UpdateInterface* updateHandler)
        {
            updateHandler->signalStrengthChanged(updatedAP);
        });
    }
}

/*
 * Removes a LibNM::AccessPoint from the list, removing the matching
 * Wifi::AccessPoint if it no longer has any matching LibNM::AccessPoints.
 */
void WifiAPList::Module::removeAccessPoint
(const LibNM::AccessPoint removedAP)
{
    const LibNM::APHash apHash = removedAP.generateHash();
    nmAccessPoints[apHash].removeAllInstancesOf(removedAP);
    if(nmAccessPoints[apHash].isEmpty())
    {
        const AccessPoint toRemove = wifiAccessPoints[apHash];
        wifiAccessPoints.erase(apHash);
        foreachHandler<UpdateInterface>([&toRemove]
                (UpdateInterface* updateHandler)
        {
            updateHandler->accessPointRemoved(toRemove);
        });
    }
}

/*
 * Updates the signal strength of an AccessPoint, setting it to the strongest
 * signal strength of its LibNM::AccessPoints.
 */
void WifiAPList::Module::updateSignalStrength(AccessPoint toUpdate)
{
    const LibNM::APHash apHash = toUpdate.getHashValue();
    const unsigned int oldSignalStrength 
        = wifiAccessPoints[apHash].getSignalStrength();
    unsigned int bestSignalStrength = 0;
    for(LibNM::AccessPoint accessPoint : nmAccessPoints[apHash])
    {
        bestSignalStrength = std::max(bestSignalStrength, 
                accessPoint.getSignalStrength());
    }
    if(bestSignalStrength != oldSignalStrength)
    {
        static_cast<APInterface::SignalStrength*>(&toUpdate)->setSignalStrength
                (bestSignalStrength);
        foreachHandler<AP::UpdateInterface>([&toUpdate]
                (AP::UpdateInterface* updateHandler)
        {
            updateHandler->signalStrengthChanged(toUpdate);
        });
    }
}

/*
 * Removes all saved Wifi::AccessPoints and LibNM::AccessPoints.
 */
void WifiAPList::Module::clearAccessPoints()
{
    getSiblingModule<LibNM::Thread::Module>()->call([]()
    {
        wifiAccessPoints.clear();
        nmAccessPoints.clear();
    });
}

/*
 * Reloads all LibNM::AccessPoints from the NetworkResource, updating
 * Wifi::AccessPoints as necessary.
 */
void WifiAPList::Module::updateAllAccessPoints()
{
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, nmThread]()
    {
        LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice(); 
        nmAccessPoints.clear();
        juce::Array<LibNM::AccessPoint> nmAPs = wifiDevice.getAccessPoints();
        for(LibNM::AccessPoint& nmAccessPoint : nmAPs)
        {
            addAccessPoint(nmAccessPoint);
        }
    });
}

/*
 * Removes all LibNM::AccessPoint objects that are no longer valid.
 */
void WifiAPList::Module::removeInvalidatedAccessPoints()
{
    getSiblingModule<LibNM::Thread::Module>()->call([this]()
    {
        juce::Array<LibNM::APHash> toClear;
        for(auto& iter : nmAccessPoints)
        {
            iter.second.removeIf([](LibNM::AccessPoint& nmAP)->bool
            {
                return nmAP.isNull();
            });
            if(iter.second.isEmpty())
            {
                toClear.add(iter.first);
            }
        }
        for(const LibNM::APHash hashValue : toClear)
        {
            wifiAccessPoints.erase(hashValue);
            nmAccessPoints.erase(hashValue);
        }
    });
}
