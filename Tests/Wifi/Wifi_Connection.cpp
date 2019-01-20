#define WIFI_IMPLEMENTATION
#include "Wifi_Manager.h"
#include "Wifi_Connection_Controller.h"
#include "Wifi_Connection_RecordReader.h"
#include "Wifi_Connection_RecordWriter.h"
#include "Wifi_Connection_Listener.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_AccessPoint.h"
#include "Files/JSONFile.h"
#include "DelayUtils.h"
#include "JuceHeader.h"

namespace Test { class Connection; }
namespace Test { class Listener; }

/* Path to the JSON test data file in the program's assets folder: */
static const constexpr char* testFilePath = "testing/connectionTest.json";
/* APHash string identifying a secured, visible AP with a saved connection: */
static const constexpr char* savedAPHashKey = "Saved connection hash";
/* APHash string identifying a secured, visible AP with no saved connection: */
static const constexpr char* unsavedAPHashKey = "Unsaved connection hash";

/**
 * @brief  Listens for connection events to make sure they appear in the right
 *         time and order.
 */
class Test::Listener : public Wifi::Connection::Listener
{
private:
    typedef Wifi::Connection::EventType EventType;

    /**
     * @brief  Updates the last recorded connection event when updates are
     *         received.
     *
     * @param eventAP    The access point associated with the event.
     *
     * @param eventType  The type of event that occured.
     */
    void updateLastEvent(const Wifi::AccessPoint eventAP,
            const EventType eventType)
    {
        const juce::ScopedLock eventLock(eventControl);
        lastEvent = Wifi::Connection::Event(eventAP, eventType);
        DBG("New Event:" << lastEvent.toString());
    }

public:
    Listener() { }
    virtual ~Listener() { }

    /* Update the last event when a new connection starts opening: */
    virtual void startedConnecting
    (const Wifi::AccessPoint connectingAP) override
    {
        updateLastEvent(connectingAP, EventType::startedConnecting);
    }

    /* Update the last event when connection authentication fails: */
    virtual void connectionAuthFailed
    (const Wifi::AccessPoint connectingAP) override
    {
        updateLastEvent(connectingAP, EventType::connectionAuthFailed);
    }

    /* Update the last event when a connection is fully opened: */
    virtual void connected
    (const Wifi::AccessPoint connectedAP) override
    {
        const juce::ScopedLock eventLock(eventControl);
    }

    /* Update the last event when a connection is closed: */
    virtual void disconnected
    (const Wifi::AccessPoint disconnectedAP) override
    {
        const juce::ScopedLock eventLock(eventControl);
    }

    /**
     * @brief  Gets the last event recorded by this listener.
     *
     * @return  The last recorded connection event.
     */
    Wifi::Connection::Event getLastEvent() const
    {
        const juce::ScopedLock eventLock(eventControl);
        return lastEvent;
    }

private:
    juce::CriticalSection eventControl;
    Wifi::Connection::Event lastEvent;
};

class Test::Connection : public juce::UnitTest
{
public:
    Connection() : juce::UnitTest("Wifi::Connection Testing",
            "Wifi") {}
    
    void runTest() override
    {
        using namespace Wifi::Connection;
        using juce::String;
        Wifi::Manager wifiManager;
        Test::Listener updateListener;
        Controller connectionController;
        RecordReader recordReader;
        Wifi::APList::Reader apListReader;
        
        beginTest("Valid saved connection control test");
        JSONFile testValues(testFilePath);
        String savedHashStr;
        expectDoesNotThrow(savedHashStr = testValues.getProperty<String>
                (savedAPHashKey));
        LibNM::APHash savedHash(savedHashStr);

        Wifi::AccessPoint savedAP = apListReader.getAccessPoint(savedHash);
        expect(!savedAP.isNull(), 
                String("Failed to find saved AP from hash string, check ")
                + testFilePath);

        if(recordReader.isConnected())
        {
            connectionController.disconnect();
            DelayUtils::idleUntil([&recordReader]()->bool
            {
                return !recordReader.isConnected();
            }, 100, 1500);
        }
        expect(!recordReader.isConnected(), "Failed to disconnect!");

        juce::int64 beforeConnection = juce::Time::currentTimeMillis();
        expect(recordReader.getLastEvent().getEventTime().toMilliseconds()
                < beforeConnection, "Found invalid future connection event!");
        connectionController.connectToAccessPoint(savedAP);
        DelayUtils::idleUntil([&updateListener, &beforeConnection]()->bool
        {
            return updateListener.getLastEvent().getEventTime().toMilliseconds()
                    > beforeConnection;
        }, 100, 1500);

        expect(updateListener.getLastEvent().getEventTime().toMilliseconds()
                    > beforeConnection, "No event recorded!");
        Event lastEvent = recordReader.getLastEvent();
        expect(lastEvent == updateListener.getLastEvent(), 
                "Failed to add event to record resource properly!");
        expect(lastEvent.getEventType() == EventType::connected,
                "Last event type was not a connection event!");
        expect(lastEvent.getEventAP() == savedAP,
                String("Last event had unexpected AP ") 
                + lastEvent.getEventAP().toString());
    }
};

static Test::Connection connectionTest;

