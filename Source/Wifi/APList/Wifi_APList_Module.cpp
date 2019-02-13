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

/* All visible Wifi::AccessPoint objects, mapped by hash value. */
static std::map<Wifi::LibNM::APHash, Wifi::AccessPoint> wifiAccessPoints;

/* All visible LibNM::AccessPoint objects, mapped by hash value. */
static std::map<Wifi::LibNM::APHash, juce::Array<Wifi::LibNM::AccessPoint>> 
        nmAccessPoints;

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Wifi::APList::Module::";
/**
 * @brief  Prints the access point list's data maps for debugging.
 */
void printAPList()
{
    DBG(dbgPrefix << __func__ << ": " << (int) wifiAccessPoints.size()
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
 * Reads initial access point data from LibNM, using it to construct the access
 * point list.
 */
Wifi::APList::Module::Module(Resource& wifiResource) : 
    SharedResource::Modular::Module<Resource>(wifiResource) 
{ 
    DBG(dbgPrefix << __func__ << ": Creating Wifi::APList module.");
}

/*
 * Clears all access point data on destruction.
 */
Wifi::APList::Module::~Module()
{
    wifiAccessPoints.clear();
    nmAccessPoints.clear();
    DBG(dbgPrefix << __func__ << ": Finished destroying Wifi::APList module.");
}

/*
 * Gets AccessPoint objects for all visible access points.
 */
juce::Array<Wifi::AccessPoint> 
Wifi::APList::Module::getAccessPoints() const
{
    juce::Array<Wifi::AccessPoint> accessPoints;
    for(auto apIterator : wifiAccessPoints)
    {
        accessPoints.add(apIterator.second);
    }
    return accessPoints;
}

/*
 * Finds a single AccessPoint using its hash value.
 */
Wifi::AccessPoint 
Wifi::APList::Module::getAccessPoint(LibNM::APHash apHash) const
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
#ifdef JUCE_DEBUG
        printAPList();
#endif
        return Wifi::AccessPoint();
    }
}

/*
 * Gets the strongest visible LibNM::AccessPoint represented by an AccessPoint 
 * object.
 */
Wifi::LibNM::AccessPoint Wifi::APList::Module::getStrongestNMAccessPoint
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
 * Gets the list of all LibNM::AccessPoint objects provided by LibNM.
 */
juce::Array<Wifi::LibNM::AccessPoint> Wifi::APList::Module::getNMAccessPoints() 
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
 * Gets all LibNM::AccessPoint objects that are represented by an AccessPoint 
 * object.
 */
juce::Array<Wifi::LibNM::AccessPoint> Wifi::APList::Module::getNMAccessPoints
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
 * Adds a new LibNM::AccessPoint to the list, creating an AccessPoint object to
 * represent it if necessary. 
 */
void Wifi::APList::Module::addAccessPoint(const LibNM::AccessPoint addedAP)
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
 * Removes a LibNM::AccessPoint from the list.
 */
void Wifi::APList::Module::removeAccessPoint
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
 * signal strength of any matching LibNM::AccessPoint object.
 */
void Wifi::APList::Module::updateSignalStrength(AccessPoint toUpdate)
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
 * Clears the list of saved AccessPoint and LibNM::AccessPoint objects.
 */
void Wifi::APList::Module::clearAccessPoints()
{
    getSiblingModule<LibNM::Thread::Module>()->call([]()
    {
        wifiAccessPoints.clear();
        nmAccessPoints.clear();
    });
}

/*
 * Reloads all LibNM::AccessPoint objects from LibNM, updating the list of 
 * AccessPoint objects as necessary.
 */
void Wifi::APList::Module::updateAllAccessPoints()
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
void Wifi::APList::Module::removeInvalidatedAccessPoints()
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
