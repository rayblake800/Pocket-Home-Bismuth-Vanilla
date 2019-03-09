#define WIFI_IMPLEMENTATION
#include "Wifi_Signal_DeviceModule.h"
#include "Wifi_Resource.h"
#include "Wifi_Signal_APModule.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Connection_Control_Module.h"
#include "Wifi_APList_Module.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_Connection.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_ContextTest.h"

#ifdef JUCE_DEBUG
#include "Wifi_DebugOutput.h"
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Wifi::Signal::DeviceModule::";
#endif

/*
 * Connects the module to its Resource.
 */
Wifi::Signal::DeviceModule::DeviceModule(Resource& parentResource) :
Wifi::Module(parentResource) { }

/*
 * Starts tracking the LibNM thread's DeviceWifi object.
 */
void Wifi::Signal::DeviceModule::connect()
{
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, nmThread]()
    {
        LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
        connectAllSignals(wifiDevice);
        juce::Array<LibNM::AccessPoint> visibleAPs 
                = wifiDevice.getAccessPoints();
        for(LibNM::AccessPoint& accessPoint : visibleAPs)
        {
            APModule* apHandler = getSiblingModule<APModule>();
            apHandler->connectAllSignals(accessPoint);
        }
    });
}

/*
 * Stops tracking the LibNM::ThreadResource's DeviceWifi object.
 */
void Wifi::Signal::DeviceModule::disconnect()
{
    disconnectAll();
    APModule* apHandler = getSiblingModule<APModule>();
    apHandler->disconnect();
}

/*
 * Handles Wifi device state changes.
 */
void Wifi::Signal::DeviceModule::stateChanged
(NMDeviceState newState, NMDeviceState oldState, NMDeviceStateReason reason)
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write,
            [this, newState, oldState, reason]()
    {
        DBG(dbgPrefix << "stateChanged" << ":  changed to " 
                << deviceStateString(newState) 
                << ", reason=" << deviceStateReasonString(reason));

        // Find any access point associated with the state change:
        LibNM::Thread::Module* nmThread 
                = getSiblingModule<LibNM::Thread::Module>();
        APList::Module* apList = getSiblingModule<APList::Module>();
        AccessPoint lastActiveAP;
        LibNM::AccessPoint activeNMAP 
                = nmThread->getWifiDevice().getActiveAccessPoint();
        if(!activeNMAP.isNull())
        {
            lastActiveAP = apList->getAccessPoint(activeNMAP.generateHash());
        }

        // Notify the Connection::Record::Module if the state change is from a
        // notable connection event:
        Connection::Record::Module* connectionRecord
                = getSiblingModule<Connection::Record::Module>();

        using Wifi::Connection::EventType;
        using Wifi::Connection::Event;
        
        EventType eventType;
        switch (newState)
        {
            case NM_DEVICE_STATE_ACTIVATED:
                eventType = EventType::connected;
                break;
            case NM_DEVICE_STATE_PREPARE:
            case NM_DEVICE_STATE_CONFIG:
            case NM_DEVICE_STATE_IP_CONFIG:
            case NM_DEVICE_STATE_IP_CHECK:
            case NM_DEVICE_STATE_SECONDARIES:
            case NM_DEVICE_STATE_NEED_AUTH:
                eventType = EventType::startedConnecting;
                break;
            case NM_DEVICE_STATE_DISCONNECTED:
                eventType = (oldState == NM_DEVICE_STATE_DEACTIVATING) ?
                        EventType::disconnected : EventType::connectionFailed;
                break;
            case NM_DEVICE_STATE_DEACTIVATING:
            case NM_DEVICE_STATE_FAILED:
                eventType = (reason == NM_DEVICE_STATE_REASON_NO_SECRETS) ?
                        EventType::connectionAuthFailed 
                        : EventType::disconnected;
            case NM_DEVICE_STATE_UNKNOWN:
            case NM_DEVICE_STATE_UNMANAGED:
            case NM_DEVICE_STATE_UNAVAILABLE:
            default:
                eventType = EventType::invalid;
        }
        if(lastActiveAP.isNull())
        {
            lastActiveAP = connectionRecord->getActiveAP();
        }
        
        if(eventType != EventType::invalid)
        {
            Event newEvent(lastActiveAP, eventType);
            connectionRecord->addEventIfNotDuplicate(newEvent);
        }

        // Send the update to the connection control module in case it needs it 
        // to complete a connection attempt:
        Connection::Control::Module* connectionController
                = getSiblingModule<Connection::Control::Module>();
        connectionController->signalWifiStateChange(newState, oldState, reason);
    });
}

/*
 * Updates the access point list whenever a new access point is detected.
 */
void Wifi::Signal::DeviceModule::accessPointAdded
(LibNM::AccessPoint addedAP)
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write,
            [this, addedAP]()
    {
        APModule* apHandler = getSiblingModule<APModule>();
        apHandler->connectAllSignals(addedAP);

        DBG(dbgPrefix << "accessPointAdded" << ": Added Wifi AP " 
                << addedAP.getSSIDText());

        Connection::Control::Module* connectionController
                = getSiblingModule<Connection::Control::Module>();
        connectionController->signalAPAdded(addedAP);

        APList::Module* apList = getSiblingModule<APList::Module>();
        apList->addAccessPoint(addedAP);
    });
}

/*
 * Updates the access point list whenever a previously seen access point is 
 * lost.
 */
void Wifi::Signal::DeviceModule::accessPointRemoved()
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write, [this]()
    {
        APList::Module* apList = getSiblingModule<APList::Module>();
        apList->removeInvalidatedAccessPoints();

#ifdef JUCE_DEBUG
        static int lastAPCount = 0;
        LibNM::Thread::Module* nmThread 
                = getSiblingModule<LibNM::Thread::Module>();
        int apsRemaining = nmThread->getWifiDevice().getAccessPoints().size();
        if(apsRemaining != lastAPCount)
        {
            lastAPCount = apsRemaining;
            DBG(dbgPrefix << "accessPointRemoved" << ": " << lastAPCount 
                    << " AP(s) remaining.");
        }
#endif
    });
}

/*
 * Updates the connection record when the active network connection changes.
 */
void Wifi::Signal::DeviceModule::activeConnectionChanged
(LibNM::ActiveConnection activeConnection)
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write, 
            [this, activeConnection]()
    {
        using Wifi::Connection::EventType;
        using Wifi::Connection::Event;

        DBG(dbgPrefix << "activeConnectionChanged" 
                << ": active connection changed to " 
                << (activeConnection.isNull() ? 
                    "NULL" : activeConnection.getUUID()));

        Connection::Record::Module* connectionRecord
                = getSiblingModule<Connection::Record::Module>();
        Event lastEvent = connectionRecord->getLatestEvent();
        EventType lastEventType = lastEvent.getEventType();
        
        EventType updateType = EventType::invalid;        
        Wifi::AccessPoint connectionAP = lastEvent.getEventAP();
        if(activeConnection.isNull())
        {
            updateType = EventType::disconnected; 
            static_cast<APInterface::SavedConnection*>(&connectionAP)
                    ->setLastConnectionTime
                    (juce::Time::getCurrentTime().toMilliseconds());
        }
        else
        {
            LibNM::Thread::Module* nmThread 
                    = getSiblingModule<LibNM::Thread::Module>();
            const LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
            const LibNM::AccessPoint nmAP = wifiDevice.getAccessPoint(
                    activeConnection.getAccessPointPath());
            const APList::Module* apList 
                    = getConstSiblingModule<APList::Module>();
            connectionAP = apList->getAccessPoint(nmAP.generateHash());

            NMActiveConnectionState connectionState 
                    = activeConnection.getConnectionState();

            if(connectionState == NM_ACTIVE_CONNECTION_STATE_ACTIVATING)
            {
                updateType = EventType::startedConnecting;
            }
            else if (connectionState == NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
            {
                updateType = EventType::connected;
                APInterface::SavedConnection* apUpdateInterface
                        = static_cast<APInterface::SavedConnection*>(
                                &connectionAP);
                apUpdateInterface->setLastConnectionTime
                        (juce::Time::getCurrentTime().toMilliseconds());
                apUpdateInterface->setHasSavedConnection(true);
            }
        }

        // Only add a new event if the event type and access point are valid.
        // Do not add an event if its type and access point are identical to the
        // last recorded event.
        if(updateType != EventType::invalid && !connectionAP.isNull())
        {
            connectionRecord->addEventIfNotDuplicate(Event(connectionAP,
                        updateType));
        }
    });
}
