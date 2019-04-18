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

// All visible AccessPoint objects, mapped by hash value.
static std::map<Wifi::LibNM::APHash, Wifi::AccessPoint> wifiAccessPoints;

// All visible LibNM::AccessPoint objects, mapped by hash value.
static std::map<Wifi::LibNM::APHash, juce::Array<Wifi::LibNM::AccessPoint>>
        nmAccessPoints;

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Wifi::APList::Module::";

/**
 * @brief  Prints the access point list's data maps for debugging.
 */
void printAPList()
{
    DBG(dbgPrefix << __func__ << ": " << (int) wifiAccessPoints.size()
            << " access points found.");
    for (auto& iter : wifiAccessPoints)
    {
        DBG("  [" << iter.first.toString() << "] ("
                << nmAccessPoints[iter.first].size() <<  " LibNM AP)");
        DBG("   = " << iter.second.toString());
    }
}
#endif

// Creates the APList module. The constructed list module will be empty, and
// should be initialized with the updateAllAccessPoints function from within the
// LibNM thread.
Wifi::APList::Module::Module(Resource& wifiResource) :
    Wifi::Module(wifiResource)
{
    DBG(dbgPrefix << __func__ << ": Creating Wifi::APList module.");
}


// Clears all access point data on destruction.
Wifi::APList::Module::~Module()
{
    wifiAccessPoints.clear();
    nmAccessPoints.clear();
    DBG(dbgPrefix << __func__ << ": Finished destroying Wifi::APList module.");
}


// Gets AccessPoint objects for all visible access points.
juce::Array<Wifi::AccessPoint>
Wifi::APList::Module::getAccessPoints() const
{
    juce::Array<Wifi::AccessPoint> accessPoints;
    for (auto& apIterator : wifiAccessPoints)
    {
        // Ignore access points with no signal strength, they are not currently
        // visible or valid.
        if (apIterator.second.getSignalStrength() > 0)
        {
            accessPoints.add(apIterator.second);
        }
    }
    return accessPoints;
}


// Finds a single AccessPoint using its hash value.
Wifi::AccessPoint Wifi::APList::Module::getAccessPoint
(LibNM::APHash apHash) const
{
    try
    {
        jassert(!apHash.isNull());
        AccessPoint matchingAP = wifiAccessPoints.at(apHash);
        // Ignore access points with no signal strength, they are not currently
        // visible or valid.
        if (matchingAP.getSignalStrength() > 0)
        {
            return matchingAP;
        }
    }
    catch(std::out_of_range e)
    {
        DBG(dbgPrefix << __func__ << ": Failed to find hash value \""
                << apHash.toString() << "\"");
        DBG(dbgPrefix << __func__ << ": Full access point list:");
#ifdef JUCE_DEBUG
        printAPList();
#endif
    }
    return AccessPoint();
}


// Gets the strongest visible LibNM::AccessPoint represented by an AccessPoint
// object.
Wifi::LibNM::AccessPoint Wifi::APList::Module::getStrongestNMAccessPoint
(const AccessPoint accessPoint) const
{
    ASSERT_NM_CONTEXT;
    juce::Array<LibNM::AccessPoint>& apList
            = nmAccessPoints[accessPoint.getHashValue()];
    apList.removeIf([] (LibNM::AccessPoint ap)
    {
        return ap.isNull();
    });

    LibNM::AccessPoint strongestAP;
    int bestSignalStrength = 0;
    for (LibNM::AccessPoint& nmAccessPoint : apList)
    {
        const int apStrength = nmAccessPoint.getSignalStrength();
        if (apStrength > bestSignalStrength)
        {
            bestSignalStrength = apStrength;
            strongestAP = nmAccessPoint;
        }
    }
    return strongestAP;
}


// Gets the list of all LibNM::AccessPoint objects provided by LibNM.
juce::Array<Wifi::LibNM::AccessPoint> Wifi::APList::Module::getNMAccessPoints()
        const
{
    ASSERT_NM_CONTEXT;
    juce::Array<LibNM::AccessPoint> nmAPs;
    for (auto& apListIter : nmAccessPoints)
    {
        nmAPs.addArray(apListIter.second);
    }
    return nmAPs;
}


// Gets all LibNM::AccessPoint objects that are represented by an AccessPoint
// object.
juce::Array<Wifi::LibNM::AccessPoint> Wifi::APList::Module::getNMAccessPoints
(const AccessPoint accessPoint) const
{
    ASSERT_NM_CONTEXT;
    juce::Array<LibNM::AccessPoint>& matchingAPs
            = nmAccessPoints[accessPoint.getHashValue()];
    matchingAPs.removeIf([] (LibNM::AccessPoint ap)
    {
        return ap.isNull();
    });

    return matchingAPs;
}


// Adds a new LibNM::AccessPoint to the list, creating an AccessPoint object to
// represent it if necessary.
void Wifi::APList::Module::addAccessPoint(const LibNM::AccessPoint addedAP)
{
    ASSERT_NM_CONTEXT;
    jassert(!addedAP.isNull());
    const LibNM::APHash apHash = addedAP.generateHash();
    jassert(!apHash.isNull());

    const bool newConnectionVisible = nmAccessPoints[apHash].isEmpty();
    nmAccessPoints[apHash].addIfNotAlreadyThere(addedAP);

    // Create a matching AccessPoint object if one doesn't already exist:
    if (wifiAccessPoints.count(apHash) == 0)
    {
        Connection::Saved::Module* savedConnections
                = getSiblingModule<Connection::Saved::Module>();
        AccessPoint newWifiAP(addedAP.getSSID(),
                addedAP.generateHash(),
                addedAP.getSecurityType(),
                addedAP.getSignalStrength());
        savedConnections->updateSavedAPData(newWifiAP);
        jassert(!newWifiAP.isNull());
        jassert(newWifiAP.getHashValue() == apHash);
        wifiAccessPoints[apHash] = newWifiAP;
    }

    // Update signal strength if necessary:
    AccessPoint& wifiAP = wifiAccessPoints[apHash];
    const unsigned int newSignalStrength = addedAP.getSignalStrength();
    const bool signalStrengthChanged
            = (newSignalStrength > wifiAP.getSignalStrength());
    if (newSignalStrength > wifiAP.getSignalStrength())
    {
        static_cast<APInterface::SignalStrength*> (&wifiAP)->setSignalStrength
                (newSignalStrength);
    }

    // If relevant updates were made, share those updates with the appropriate
    // Listener objects:
    AccessPoint updatedAP = wifiAccessPoints[apHash];
    if (newConnectionVisible)
    {
        foreachModuleHandler<UpdateInterface>([updatedAP]
                (UpdateInterface* updateHandler)
        {
            updateHandler->accessPointAdded(updatedAP);
        });
    }
    else if (signalStrengthChanged)
    {
        foreachModuleHandler<AP::UpdateInterface>([updatedAP]
                (AP::UpdateInterface* updateHandler)
        {
            updateHandler->signalStrengthChanged(updatedAP);
        });
    }
}


// Removes a LibNM::AccessPoint from the list.
void Wifi::APList::Module::removeAccessPoint
(const LibNM::AccessPoint removedAP)
{
    ASSERT_NM_CONTEXT;
    const LibNM::APHash apHash = removedAP.generateHash();
    nmAccessPoints[apHash].removeAllInstancesOf(removedAP);
    updateAPIfLost(wifiAccessPoints[apHash]);
}


// Updates the signal strength of an AccessPoint, setting it to the strongest
// signal strength of any matching LibNM::AccessPoint object.
void Wifi::APList::Module::updateSignalStrength(AccessPoint toUpdate)
{
    ASSERT_NM_CONTEXT;
    const LibNM::APHash apHash = toUpdate.getHashValue();
    const unsigned int oldSignalStrength
            = wifiAccessPoints[apHash].getSignalStrength();
    unsigned int bestSignalStrength = 0;
    for (LibNM::AccessPoint accessPoint : nmAccessPoints[apHash])
    {
        bestSignalStrength = std::max(bestSignalStrength,
                accessPoint.getSignalStrength());
    }
    if (bestSignalStrength != oldSignalStrength)
    {
        static_cast<APInterface::SignalStrength*>
                (&toUpdate)->setSignalStrength(bestSignalStrength);
        foreachModuleHandler<AP::UpdateInterface>([toUpdate]
                (AP::UpdateInterface* updateHandler)
        {
            updateHandler->signalStrengthChanged(toUpdate);
        });
    }
}


// Clears the list of saved AccessPoint and LibNM::AccessPoint objects.
void Wifi::APList::Module::clearAccessPoints()
{
    getSiblingModule<LibNM::Thread::Module>()->call([]()
    {
        wifiAccessPoints.clear();
        nmAccessPoints.clear();
    });
}


// Reloads all LibNM::AccessPoint objects from LibNM, updating the list of
// AccessPoint objects as necessary.
void Wifi::APList::Module::updateAllAccessPoints()
{
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, nmThread]()
    {
        removeInvalidatedAccessPoints();
        LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
        juce::Array<LibNM::AccessPoint> nmAPs = wifiDevice.getAccessPoints();
        for (LibNM::AccessPoint& nmAccessPoint : nmAPs)
        {
            addAccessPoint(nmAccessPoint);
        }
    });
    DBG(dbgPrefix << __func__ << ": List contains "
            << (int) wifiAccessPoints.size() << " AP objects representing "
            << (int) nmAccessPoints.size() << " LibNM AP objects");
}


// Removes all LibNM::AccessPoint objects that are no longer valid.
void Wifi::APList::Module::removeInvalidatedAccessPoints()
{
    ASSERT_NM_CONTEXT;
    for (auto& iter : nmAccessPoints)
    {
        iter.second.removeIf([] (LibNM::AccessPoint& nmAP)->bool
        {
            return nmAP.isNull();
        });
    }
    for (auto& iter : wifiAccessPoints)
    {
        updateAPIfLost(iter.second);
    }
}


// Sets an AccessPoint object's signal strength to zero and notifies all
// listeners that it has been removed, if and only if all LibNM::AccessPoint
// objects represented by the access point have been removed.
void Wifi::APList::Module::updateAPIfLost(AccessPoint toCheck)
{
    LibNM::APHash apHash = toCheck.getHashValue();
    if (nmAccessPoints[apHash].isEmpty())
    {
        static_cast<APInterface::SignalStrength*> (&toCheck)
                ->setSignalStrength(0);
        foreachModuleHandler<UpdateInterface>([toCheck]
                (UpdateInterface* updateHandler)
        {
            updateHandler->accessPointRemoved(toCheck);
        });
    }
}
