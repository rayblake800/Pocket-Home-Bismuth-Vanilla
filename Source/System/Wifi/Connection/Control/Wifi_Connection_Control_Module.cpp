#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Connection_Saved_Module.h"
#include "Wifi_APList_Module.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_LibNM_SSID.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_Connection.h"
#include "SharedResource_Thread_ScopedWriteLock.h"
#include "GLib_ErrorPtr.h"

#ifdef JUCE_DEBUG
#include "Wifi_DebugOutput.h"
// Print the complete class name before all debug output.
static const constexpr char* dbgPrefix = "Wifi::Connection::Control::Module::";
#endif

namespace WifiConnect = Wifi::Connection;

// Maximum number of milliseconds to wait before abandoning a connection
// attempt:
static const constexpr int connectionTimeout = 300000;

// Additional pending connection data:

// These items are defined here to reduce the number of dependencies that need
// to be defined in the header file. This approach is only valid because
// SharedResource::Module classes are singletons, and Control::Module will
// never have any subclasses.

// The access point used to establish the connection:
static Wifi::AccessPoint connectingAP;

// The last recorded connection event:
static Wifi::Connection::Event lastEvent;

// Saved connection objects to try and use to connect:
static juce::Array<Wifi::LibNM::DBus::SavedConnection>
        potentialSavedConnections;
// Index of the saved connection object to try next:
static int savedConnectionIdx = -1;

// LibNM access point objects to try and use to connect:
static juce::Array<Wifi::LibNM::AccessPoint> potentialNMAPs;
// Index of the LibNM::AccessPoint object to try next:
static int nmAPIdx = -1;
// List of access point MAC addresses tried, used to prevent a NMAP from being 
// continually reused if LibNM recreates access point objects:
static juce::StringArray attemptedBSSIDs;

namespace Wifi { class ConnectionComparator; }

/**
 * @brief  Provides comparator functions needed to sort access point and
 *         connection data.
 */
class Wifi::ConnectionComparator
{
public:
    /**
     * @brief  Compares LibNM access points by signal strength.
     *
     * @param firstAP   The first of two access points to compare.
     *
     * @param secondAP  The second of two access points to compare.
     *
     * @return          A negative value if firstAP is stronger, a positive
     *                  value if secondAP is stronger, or zero if both access
     *                  points are equally strong.
     */
    static int compareElements(LibNM::AccessPoint firstAP,
            LibNM::AccessPoint secondAP)
    {
        return (int) secondAP.getSignalStrength()
                - firstAP.getSignalStrength();
    }

    /**
     * @brief  Compares saved connections based on last connection time.
     *
     * @param firstSaved   The first of two saved connections to compare.
     *
     * @param secondSaved  The second of two saved connections to compare.
     *
     * @return             A negative value if firstSaved has the most recent
     *                     connection time, a positive value if secondSaved has
     *                     the most recent connection time, or zero if both
     *                     have the same last connection time.
     */
    static int compareElements(LibNM::DBus::SavedConnection firstSaved,
            LibNM::DBus::SavedConnection secondSaved)
    {
        return secondSaved.lastConnectionTime().toMilliseconds()
                - firstSaved.lastConnectionTime().toMilliseconds();
    }
};

// Connects the module to its Resource.
WifiConnect::Control::Module::Module(Resource& wifiResource) :
    Wifi::Module(wifiResource) { }


// Ensures all pending connection data is removed.
WifiConnect::Control::Module::~Module()
{
    if (connectionStarted)
    {
        DBG(dbgPrefix << __func__
                << ": Destroying module despite pending connection to "
                << connectingAP.toString());
    }
    clearPendingConnectionData();
}


// Checks if the Control::Module is currently attempting to open a Wifi
// connection.
bool WifiConnect::Control::Module::tryingToConnect() const
{
    return connectionStarted;
}


// Attempts to open a Wifi network connection using a nearby access point.
void WifiConnect::Control::Module::connectToAccessPoint
(const AccessPoint toConnect, juce::String securityKey)
{
    if (toConnect.isNull())
    {
        DBG(dbgPrefix << __func__
                << ": Can't connect, selected access point is null.");
        return;
    }

    if (!toConnect.hasSavedConnection()
            && !toConnect.isValidKeyFormat(securityKey))
    {
        DBG(dbgPrefix << __func__
                << ": Can't connect, missing valid security key.");
        return;
    }

    Record::Module* connectionRecord = getSiblingModule<Record::Module>();
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();

    if (connectionRecord->isConnected() || connectionRecord->isConnecting())
    {
        if (toConnect == connectionRecord->getActiveAP())
        {
            DBG(dbgPrefix << __func__ << ": Cancelling connection, access point"
                    " is already connected or connecting.");
            return;
        }
        else
        {
            DBG(dbgPrefix << __func__
                    << ": Closing previous active connection.");
            nmThread->call([this]() { internalDisconnect(); });
        }
    }
    if (connectionStarted)
    {
        jassert(connectingAP != toConnect);
        DBG(dbgPrefix << __func__ << ": Cancelling connection with "
                << connectingAP.toString() << " to connect with "
                << toConnect.toString());
        nmThread->call([this]() { cancelPendingConnection(); });
    }

    connectionStarted = true;
    connectionActivating = false;
    pendingPSK = securityKey;
    connectingAP = toConnect;
    potentialSavedConnections.clear();
    potentialNMAPs.clear();
    savedConnectionIdx = -1;
    nmAPIdx = -1;
    lastEvent = Event(connectingAP, EventType::connectionRequested);
    connectionRecord->addEventIfNotDuplicate(lastEvent);
    startTimer(connectionTimeout);

    nmThread->call([this, nmThread]()
    {
        // Cancel if Wifi has been disabled:
        if (!nmThread->getClient().wirelessEnabled())
        {
            DBG(dbgPrefix << __func__
                    << ": Cancelling connection, Wifi was disabled.");
            cancelPendingConnection();
            return;
        }
        // Load matching LibNM access points
        APList::Module* apList = getSiblingModule<APList::Module>();
        potentialNMAPs = apList->getNMAccessPoints(connectingAP);
        // Sort by signal strength
        ConnectionComparator nmComparator;
        potentialNMAPs.sort(nmComparator);
        if (potentialNMAPs.isEmpty())
        {
            DBG(dbgPrefix << __func__ << ": no visible matching LibNM access"
                    << " points, requesting scan.");
            LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
            // It shouldn't be possible for the Wifi device to be null at this
            // point, make sure that assumption is correct.
            jassert(!wifiDevice.isNull());
            wifiDevice.requestScan();
            return;
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": Found " << potentialNMAPs.size()
                    << " LibNM access points to try.");
        }
        continueConnectionAttempt();
    });
}


// Continues the connection attempt if a pending connection is currently in
// progress.
void Wifi::Connection::Control::Module::continueConnectionAttempt()
{
    ASSERT_NM_CONTEXT;

    // Only continue if Wifi is still connecting and is not currently activating
    // a connection:
    Record::Module* connectionRecord = getSiblingModule<Record::Module>();
    if (! connectionStarted || connectionActivating 
            || connectionRecord->isConnected())
    {
        DBG(dbgPrefix << __func__ << ": No action needed, exiting function.");
        return;
    }

    DBG(dbgPrefix << __func__ << ": Trying to create a connection using "
            << connectingAP.toString());

    // If the current LibNM AccessPoint is null or it has failed to connect
    // with either a saved connection or a new connection, advance to the next
    // access point.
    while (potentialNMAPs[nmAPIdx].isNull()
            || savedConnectionIdx > potentialSavedConnections.size())
    {
        nmAPIdx++;
        savedConnectionIdx = -1;
        if (nmAPIdx >= potentialNMAPs.size())
        {
            DBG(dbgPrefix << __func__ << ": all " << potentialNMAPs.size()
                    << " potential LibNM access point(s) failed, "
                    << "cancelling connection attempt.");
            EventType lastEventType = lastEvent.getEventType();
            if (lastEventType != EventType::connectionFailed
                    && lastEventType != EventType::connectionAuthFailed)
            {
                // Treat the event as an authentication failure if the access
                // point is secured and at least one of its NMAccessPoints is
                // still valid.
                lastEventType = EventType::connectionAuthFailed;
                if (connectingAP.getSecurityType()
                        == LibNM::SecurityType::unsecured)
                {
                    lastEventType = EventType::connectionFailed;
                }
                else
                {
                    for (LibNM::AccessPoint& nmAP : potentialNMAPs)
                    {
                        if (!nmAP.isNull())
                        {
                            lastEventType = EventType::connectionFailed;
                            break;
                        }
                    }
                }
                lastEvent = Event(connectingAP, lastEventType);
            }

            // If any new saved connections were created, they are probably
            // invalid and should be deleted here.
            WifiConnect::Saved::Module* savedConnectionModule
                    = getSiblingModule<WifiConnect::Saved::Module>();
            if (potentialSavedConnections.isEmpty())
            {
                savedConnectionModule->removeSavedConnection(connectingAP);
            }
            else
            {
                juce::Array<LibNM::DBus::SavedConnection> updatedSavedList 
                        = std::move(savedConnectionModule
                        ->getMatchingConnections(connectingAP));
                for (LibNM::DBus::SavedConnection& saved : updatedSavedList)
                {
                    if (! potentialSavedConnections.contains(saved))
                    {
                        saved.deleteConnection();
                    }
                }
            }
            cancelPendingConnection();
            connectionRecord->addEventIfNotDuplicate(lastEvent);
            return;
        }
    }
    LibNM::AccessPoint nmAccessPoint = potentialNMAPs[nmAPIdx];

    if (connectingAP.hasSavedConnection() && savedConnectionIdx < 0)
    {
        // Load the list of saved connections for the current LibNM
        // AccessPoint.
        Saved::Module* savedConnectionLoader
                = getSiblingModule<Saved::Module>();
        potentialSavedConnections
                = savedConnectionLoader->getMatchingConnections(nmAccessPoint);
        ConnectionComparator savedConnectionComparator;
        potentialSavedConnections.sort(savedConnectionComparator);
    }
    savedConnectionIdx = 0;

    LibNM::Connection newConnection;
    if (savedConnectionIdx < potentialSavedConnections.size())
    {
        // Attempt to use the next saved connection.
        DBG(dbgPrefix << __func__ << " Attempting connection with LibNM AP "
                << (nmAPIdx + 1) << " of " << potentialNMAPs.size()
                << ", saved connection " << (savedConnectionIdx + 1) << " of "
                << potentialSavedConnections.size());
        creatingNewConnection = false;
        LibNM::DBus::SavedConnection savedConnection
                = potentialSavedConnections[savedConnectionIdx];
        newConnection = savedConnection.getNMConnection();
    }
    else
    {
        // Try creating a new connection.
        DBG(dbgPrefix << __func__ << " Attempting connection with LibNM AP "
                << (nmAPIdx + 1) << " of " << potentialNMAPs.size()
                << " and no saved connections.");
        creatingNewConnection = true;
        newConnection.addWifiSettings(connectingAP.getSSID());
        // Add security settings if necessary.
        using WifiSecurity = LibNM::SecurityType;
        LibNM::SecurityType securityType = connectingAP.getSecurityType();
        if (connectingAP.isValidKeyFormat(pendingPSK)
                && securityType != WifiSecurity::unsecured)
        {
            if (securityType == WifiSecurity::securedWEP)
            {
                newConnection.addWEPSettings(pendingPSK);
            }
            else
            {
                newConnection.addWPASettings(pendingPSK);
            }
        }
    }
    savedConnectionIdx++;

    // Check if the new connection object is valid:
    GLib::ErrorPtr gError;
    if (!newConnection.verify(gError.getAddress()))
    {
        DBG(dbgPrefix << __func__
                << ": Verifying connection resulted in error:");
        gError.handleError();
        DBG(dbgPrefix << __func__
                << ": Trying connection despite failing to verify.");
    }

    // Save the AP BSSID to use, and have the Client activate the connection:
    attemptedBSSIDs.add(nmAccessPoint.getBSSID());
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    LibNM::Client networkClient = nmThread->getClient();
    networkClient.activateConnection(
            newConnection,
            nmThread->getWifiDevice(),
            nmAccessPoint,
            this,
            !creatingNewConnection);
    connectionActivating = true;
}


// Asynchronously closes the active Wifi connection.
void WifiConnect::Control::Module::disconnect()
{
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->callAsync([this]()
    {
        internalDisconnect();
    });
}


// Notifies the Control::Module that a new access point was spotted, just in
// case it is needed to establish a connection.
void WifiConnect::Control::Module::signalAPAdded(LibNM::AccessPoint newAP)
{
    ASSERT_NM_CONTEXT;
    juce::String apBSSID(newAP.getBSSID());
    if (connectionStarted && connectingAP == newAP 
            && ! attemptedBSSIDs.contains(apBSSID))
    {
        potentialNMAPs.add(newAP);
        DBG(dbgPrefix << __func__ << ": Found new compatible LibNM AP, "
                << potentialNMAPs.size() << " potential APs now tracked.");
        if (! connectionActivating)
        {
            continueConnectionAttempt();
        }
    }
}


// Notifies the Control::Module that wireless networking was disabled.
void WifiConnect::Control::Module::signalWifiDisabled()
{
    ASSERT_NM_CONTEXT;
    if (connectionStarted)
    {
        DBG(dbgPrefix << __func__
                << ": Wifi disabled, stopping connection attempt.");
        clearPendingConnectionData();
        stopTimer();
    }
}


// Notifies the Control::Module when a new connection event is recorded.
void WifiConnect::Control::Module::wifiEventRecorded(const Event newEvent)
{
    ASSERT_NM_CONTEXT;
    if (newEvent == lastEvent)
    {
        return;
    }
    DBG(dbgPrefix << __func__ << ": Received new event "
            << newEvent.toString());
    lastEvent = newEvent;
    if (!connectionStarted)
    {
        return;
    }
    switch(lastEvent.getEventType())
    {
        case EventType::connected:
            DBG(dbgPrefix << __func__ << ": Connection activated.");
            if (potentialSavedConnections.isEmpty())
            {
                DBG(dbgPrefix << __func__ 
                        << ": Updating AP saved connections.");
                WifiConnect::Saved::Module* savedConnectionModule
                        = getSiblingModule<WifiConnect::Saved::Module>();
                savedConnectionModule->updateSavedAPData(
                        lastEvent.getEventAP());
            }

            clearPendingConnectionData();
            return;
        case EventType::disconnected:
            // When trying to connect, treat disconnect events like failed
            // connection events if they're not for a different non-null AP
            if (! connectionStarted || (lastEvent.getEventAP() != connectingAP
                        && ! lastEvent.getEventAP().isNull()))
            {
                return;
            }
        case EventType::connectionAuthFailed:
        case EventType::connectionFailed:
            DBG(dbgPrefix << __func__ << ": Failed to open connection,"
                    << " continuing connection attempt.");
            connectionActivating = false;
            continueConnectionAttempt();
            return;
        case EventType::connectionRequested:
        case EventType::startedConnecting:
            // No action needed even if a connection is in progress.
            return;
        case EventType::invalid:
            DBG(dbgPrefix << __func__
                    << ": Received invalid connection event!");
    }
}


// Clears all saved data being used for an ongoing Wifi connection attempt.
void WifiConnect::Control::Module::clearPendingConnectionData()
{
    connectionStarted = false;
    pendingPSK = juce::String();
    potentialSavedConnections.clear();
    potentialNMAPs.clear();
    nmAPIdx = 0;
    attemptedBSSIDs.clear();
    savedConnectionIdx = -1;
    creatingNewConnection = false;
}


// Cancels any pending connection attempt.
void WifiConnect::Control::Module::cancelPendingConnection()
{
    ASSERT_NM_CONTEXT;
    DBG(dbgPrefix << __func__ << ": cancelling connection.");
    stopTimer();
    clearPendingConnectionData();

    Record::Module* record = getSiblingModule<Record::Module>();
    if (record->isConnected())
    {
        DBG(dbgPrefix << "cancelPendingConnection: connection completed, "
                << "no need to cancel.");
    }
    else if (record->isConnecting())
    {
        record->addEventIfNotDuplicate(Event(connectingAP,
                    EventType::connectionFailed));
    }
    connectingAP = Wifi::AccessPoint();
}


// Closes the active Wifi connection. Only call this within the LibNM thread.
void WifiConnect::Control::Module::internalDisconnect()
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    DBG(dbgPrefix << __func__
            << ": Closing any connection on the managed Wifi device.");
    LibNM::DeviceWifi wifiDevice = nmThread->getWifiDevice();
    if (! wifiDevice.getActiveConnection().isNull())
    {
        wifiDevice.disconnect();
    }
}


// Cancels a pending connection event if it doesn't finish within a timeout
// period.
void WifiConnect::Control::Module::timerCallback()
{
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, &nmThread]()
    {
        SharedResource::Thread::ScopedWriteLock timeoutLock(
                 *nmThread->getThreadLock());
        DBG(dbgPrefix << "timerCallback"
                << ": Connection attempt timed out, cancelling:");
        cancelPendingConnection();
    });
}


// Signals that a connection is being opened.
void WifiConnect::Control::Module::openingConnection
(LibNM::ActiveConnection connection)
{
    ASSERT_NM_CONTEXT;
    // Asynchronous callbacks must lock the Wifi Resource.
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write,
            [this, connection]()
    {
        Record::Module* record = getSiblingModule<Record::Module>();
        Event latestEvent = record->getLatestEvent();
        if (latestEvent.getEventAP() == connectingAP
                && latestEvent.getEventType() == EventType::startedConnecting)
        {
            DBG(dbgPrefix << "openingConnection: Wifi device signal handler "
                    << "already recorded connection attempt");
        }
        else
        {
            DBG(dbgPrefix << "openingConnection"
                    << ": Recording new connection attempt.");
            record->addEventIfNotDuplicate(Event(connectingAP,
                        EventType::startedConnecting));
        }
    });
}


// Signals that an attempt to open a connection failed.
void WifiConnect::Control::Module::openingConnectionFailed
(LibNM::ActiveConnection connection, GError* error)
{
    ASSERT_NM_CONTEXT;
    // Asynchronous callbacks must lock the Wifi Resource.
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write,
            [this, connection, error]()
    {
        DBG(dbgPrefix << __func__ << ": Error " << error->code << ":"
                << error->message);

        Saved::Module* savedConnectionLoader
                = getSiblingModule<Saved::Module>();

        if (savedConnectionIdx >= potentialSavedConnections.size())
        {
            LibNM::AccessPoint connectingNMAP = potentialNMAPs[nmAPIdx];
            juce::Array<LibNM::DBus::SavedConnection> newSavedConnections
                    = savedConnectionLoader->getMatchingConnections(
                            connectingNMAP);
            for (LibNM::DBus::SavedConnection& savedConnection
                    : newSavedConnections)
            {
                if (!potentialSavedConnections.contains(savedConnection))
                {
                    DBG(dbgPrefix << __func__
                            << ": Deleting failed new saved connection "
                            << connection.getID());
                    savedConnection.deleteConnection();
                    break;
                }
            }
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": Previously established connection "
                    << connection.getID() << " failed to activate fully.");
        }

        // Continue trying other compatible LibNM APs or saved connections:
        connectionActivating = false;
        continueConnectionAttempt();
    });
}
