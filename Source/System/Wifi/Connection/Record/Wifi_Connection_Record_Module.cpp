#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Connection_Record_UpdateInterface.h"
#include "Wifi_Resource.h"
#include "Wifi_APList_Module.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_Client.h"
#include "Wifi_LibNM_ContextTest.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Wifi::Connection::Record::Module::";
#endif

namespace WifiRecord = Wifi::Connection::Record;

/* Stores all connection events: */
static juce::Array<Wifi::Connection::Event> connectionEvents;

/* Sorts the event list from newest to oldest. */
class
{
public:
    static int compareElements(Wifi::Connection::Event first, 
            Wifi::Connection::Event second)
    {
        juce::int64 timeDifference = first.getEventTime().toMilliseconds()
            - second.getEventTime().toMilliseconds();
        if(timeDifference == 0)
        {
            return 0;
        }
        return ((timeDifference < 0) ? 1 : -1);
    }
} eventSorter;

/*
 * Connects the module to its Resource.
 */
WifiRecord::Module::Module(Resource& parentResource) : 
Wifi::Module(parentResource) { }

/*
 * Checks if the system has an active, established Wifi network connection.
 */
bool WifiRecord::Module::isConnected() const
{
    return connectionEvents.getFirst().getEventType() == EventType::connected;
}

/*
 * Checks if NetworkManager is currently opening a network connection.
 */
bool WifiRecord::Module::isConnecting() const
{
    EventType latestEventType = connectionEvents.getFirst().getEventType();
    return latestEventType == EventType::connectionRequested
        || latestEventType == EventType::startedConnecting;
}

/*
 * Gets the access point being used by the active or activating connection.
 */
Wifi::AccessPoint WifiRecord::Module::getActiveAP() const
{
    Event latestEvent = connectionEvents.getFirst();
    if(latestEvent.getEventType() == EventType::disconnected
            || latestEvent.getEventType() == EventType::invalid)
    {
        return AccessPoint();
    }
    return latestEvent.getEventAP();
}


/*
 * Adds a new event to the list of saved events.
 */
void WifiRecord::Module::addConnectionEvent(const Event newEvent)
{
    if(!newEvent.isNull())
    {
        DBG(dbgPrefix << __func__ << ": adding " << newEvent.toString());
        connectionEvents.addSorted(eventSorter, newEvent);
        foreachModuleHandler<UpdateInterface>([this, newEvent]
                (UpdateInterface* listener)
        {
            switch(newEvent.getEventType())
            {
                case EventType::connectionRequested:
                case EventType::startedConnecting:
                    listener->startedConnecting(newEvent.getEventAP());
                    break;
                case EventType::connectionAuthFailed:
                    listener->connectionAuthFailed(newEvent.getEventAP());
                    break;
                case EventType::connected:
                    listener->connected(newEvent.getEventAP());
                    break;
                case EventType::connectionFailed:
                case EventType::disconnected:
                    listener->disconnected(newEvent.getEventAP());
                    break;
                case EventType::invalid:
                    // Invalid events shouldn't be added!
                    jassertfalse;
            }
        });
    }
}

/*
 * Adds a new event to the list of saved events if the most recent saved event 
 * doesn't have the same access point and event type.
 */
void WifiRecord::Module::addEventIfNotDuplicate(const Event newEvent)
{
    if(newEvent.getEventAP().isNull())
    {
        return;
    }
    Event latestEvent = getLatestEvent();
    if(latestEvent.getEventAP() != newEvent.getEventAP()
            || latestEvent.getEventType() != newEvent.getEventType())
    {
        addConnectionEvent(newEvent);
    }
}

/*
 * Connects with NetworkManager to initialize or update the most recent 
 * connection record.
 */
void WifiRecord::Module::updateRecords()
{
    connectionEvents.clear();
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, &nmThread]()
    {
        const LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
        const LibNM::Client networkClient = nmThread->getClient();
        
        LibNM::ActiveConnection connection 
                = networkClient.getActivatingConnection();
        if(connection.isNull())
        {
            connection = networkClient.getPrimaryConnection();
        }
        const bool isConnecting 
                = wifiDevice.getState() != NM_DEVICE_STATE_ACTIVATED; 

        if(connection.isNull())
        {
            return;
        }

        const LibNM::AccessPoint nmAP = wifiDevice.getAccessPoint
                (connection.getAccessPointPath());
        if(nmAP.isNull())
        {
            return;
        }

        const LibNM::APHash apHash = nmAP.generateHash();
        jassert(!apHash.isNull());
        Wifi::APList::Module* apList = getSiblingModule<APList::Module>();
        AccessPoint activeAP = apList->getAccessPoint(nmAP.generateHash());
        if(activeAP.isNull())
        {
            apList->updateAllAccessPoints();
            activeAP = apList->getAccessPoint(apHash);
            jassert(!activeAP.isNull());
        }

        Event initialEvent(activeAP, isConnecting ?
                EventType::startedConnecting : EventType::connected);
        DBG(dbgPrefix << __func__ << ": Initial event:"
                << initialEvent.toString());
        addConnectionEvent(initialEvent);
    });
}

/*
 * Gets the most recent connection event in the connection history.
 */
Wifi::Connection::Event WifiRecord::Module::getLatestEvent() const
{
    return connectionEvents.getFirst();
}

/*
 * Gets the most recent connection event that involves a particular access
 * point.
 */
Wifi::Connection::Event WifiRecord::Module::getLatestEvent
(const AccessPoint eventAP) const
{
    if(eventAP.isNull())
    {
        return Event();
    }
    for(Event& event : connectionEvents)
    {
        if(event.getEventAP() == eventAP)
        {
            return event;
        }
    }
    return Event();
}

/*
 * Gets the most recent connection event with a specific event type.
 */
Wifi::Connection::Event WifiRecord::Module::getLatestEvent
(const EventType eventType) const
{
    if(eventType == EventType::invalid)
    {
        return Event();
    }
    for(Event& event : connectionEvents)
    {
        if(event.getEventType() == eventType)
        {
            return event;
        }
    }
    return Event();
}

/*
 * Gets the most recent connection event with a specific event type and access
 * point.
 */
Wifi::Connection::Event WifiRecord::Module::getLatestEvent
(const AccessPoint eventAP, const EventType eventType) const
{
    if(eventAP.isNull() || eventType == EventType::invalid)
    {
        return Event();
    }
    for(Event& event : connectionEvents)
    {
        if(event.getEventAP() == eventAP && event.getEventType() == eventType)
        {
            return event;
        }
    }
    return Event();
}
