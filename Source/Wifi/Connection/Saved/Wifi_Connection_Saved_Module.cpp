#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Saved_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_APList_Module.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_Connection.h"
#include "Wifi_LibNM_ContextTest.h"

/*
 * Loads all connections saved by NetworkManager.
 */
Wifi::Connection::Saved::Module::Module(Resource& wifiResource) :
SharedResource::Modular::Module<Resource>(wifiResource)
{
    savedConnections.updateSavedConnections();
}

/*
 * Checks if NetworkManager has a saved connection that is compatible with an
 * access point.
 */
bool Wifi::Connection::Saved::Module::hasSavedConnection
(const AccessPoint toCheck)
{
    return !getMatchingConnections(toCheck).isEmpty();
}

/*
 * Checks if NetworkManager has a saved connection that is compatible with an
 * access point.
 */
bool Wifi::Connection::Saved::Module::hasSavedConnection
(const LibNM::AccessPoint toCheck) const
{
    return savedConnections.matchingConnectionExists(toCheck);
}

/*
 * Attempts to find and return a saved connection object that is compatible
 * with an access point.
 */
Wifi::LibNM::Connection 
Wifi::Connection::Saved::Module::getSavedConnection
(const AccessPoint connectionAP)
{
    juce::Array<LibNM::DBus::SavedConnection> matchingConnections
            = getMatchingConnections(connectionAP);
    if(matchingConnections.isEmpty())
    {
        return LibNM::Connection();
    }
    return matchingConnections[0].getNMConnection();
}

/*
 * Finds the last time the system was fully connected to a particular wifi
 * access point's connection.
 */
juce::Time Wifi::Connection::Saved::Module::lastConnectionTime
(const AccessPoint connectionAP)
{
    juce::Time connectionTime;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, &connectionAP, &connectionTime]()
    {
        using LibNM::DBus::SavedConnection;
        juce::Array<SavedConnection> matchingConnections
                = getMatchingConnections(connectionAP);
        for(const SavedConnection& connection : matchingConnections)
        {
            juce::Time timestamp = connection.lastConnectionTime();
            if(timestamp.toMilliseconds() > connectionTime.toMilliseconds())
            {
                connectionTime = timestamp;
            }
        }
    });
    return connectionTime;
}

/*
 * Removes all saved network connections that match a particular access point.
 */
void Wifi::Connection::Saved::Module::removeSavedConnection
(AccessPoint toRemove)
{
    auto* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, &toRemove]()
    {
        using LibNM::DBus::SavedConnection;
        const APList::Module* apList = getConstSiblingModule<APList::Module>();
        LibNM::AccessPoint nmAP = apList->getStrongestNMAccessPoint(toRemove);

        const juce::Array<SavedConnection> matchingConnections 
                = savedConnections.findConnectionsForAP(nmAP);
        for(SavedConnection savedConn : matchingConnections)
        {
            savedConn.deleteConnection();
        }
    });
    APInterface::SavedConnection* updateInterface
            = static_cast<APInterface::SavedConnection*>(&toRemove);
    updateInterface->setHasSavedConnection(false);
    updateInterface->setLastConnectionTime(0);
}

/*
 * Gets all saved connections compatible with a particular AccessPoint object.
 */
juce::Array<Wifi::LibNM::DBus::SavedConnection>
Wifi::Connection::Saved::Module::getMatchingConnections
(const Wifi::AccessPoint toMatch)
{
    using LibNM::DBus::SavedConnection;
    juce::Array<SavedConnection> matchingConnections;
    auto* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, &matchingConnections, &toMatch]()
    {
        const APList::Module* apList = getConstSiblingModule<APList::Module>();
        LibNM::AccessPoint nmAP = apList->getStrongestNMAccessPoint(toMatch);

        matchingConnections = savedConnections.findConnectionsForAP(nmAP);
    });
    return matchingConnections;
}

/*
 * Updates an access point's saved data to store whether any matching saved 
 * connection exists and the most recent time any matching saved connection 
 * was connected.
 */
void Wifi::Connection::Saved::Module::updateSavedAPData(AccessPoint toUpdate)
{
    using LibNM::DBus::SavedConnection;
    juce::int64 lastConnectionTime = 0;
    juce::Array<SavedConnection> matchingConnections 
            = getMatchingConnections(toUpdate);
    for(const SavedConnection& savedConnection : matchingConnections)
    {
        lastConnectionTime = std::max(lastConnectionTime, 
                savedConnection.lastConnectionTime().toMilliseconds());
    }
    APInterface::SavedConnection* updateInterface
            = static_cast<APInterface::SavedConnection*>(&toUpdate);
    updateInterface->setHasSavedConnection(!matchingConnections.isEmpty());
    updateInterface->setLastConnectionTime(lastConnectionTime);
}
