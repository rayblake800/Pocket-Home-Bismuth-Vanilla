/**
 * @file  Wifi_Test_ConnectionTest.cpp
 *
 * @brief  Tests the Wifi module's ability to control Wifi connections.
 */
#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_Handler.h"
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_Connection_Record_Writer.h"
#include "Wifi_Connection_Record_Listener.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_AccessPoint.h"
#include "Files/JSONFile.h"
#include "DelayUtils.h"
#include "JuceHeader.h"

namespace Wifi
{
    namespace Test
    {
        class ConnectionTest;
        class Listener;
    }
}

/* Path to the JSON test data file in the program's assets folder: */
static const constexpr char* testFilePath = "testing/connectionTest.json";

/* Test data keys: */
namespace TestKeys
{
    namespace FirstSavedAP
    {
        static const constexpr char* hash = "First saved connection AP hash";
        static const constexpr char* badPSK = "First connection invalid PSK";
        static const constexpr char* goodPSK = "First connection valid PSK";
    }
    namespace SecondSavedAP
    {
        static const constexpr char* hash = "Second saved connection AP hash";
        static const constexpr char* badPSK = "Second connection invalid PSK";
        static const constexpr char* goodPSK = "Second connection valid PSK";
    }
    namespace UnsavedAP
    {
        static const constexpr char* hash = "Unsaved connection AP hash";
        static const constexpr char* badPSK = "Unsaved connection invalid PSK";
    }
}

/* Holds access point test data: */
struct APTestData
{
    juce::String hashString;
    juce::String invalidPSK;
    juce::String validPSK;
};

/**
 * @brief  Listens for connection events to make sure they appear in the right
 *         time and order.
 */
class Wifi::Test::Listener : public Wifi::Connection::Record::Listener
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
    Wifi::Connection::Event getLatestEvent() const
    {
        const juce::ScopedLock eventLock(eventControl);
        return lastEvent;
    }

private:
    juce::CriticalSection eventControl;
    Wifi::Connection::Event lastEvent;
};

class Wifi::Test::ConnectionTest : public juce::UnitTest
{
public:
    ConnectionTest() : juce::UnitTest("Wifi::Connection Testing",
            "Wifi") {}
    
    void runTest() override
    {
        using namespace Wifi::Connection;
        using juce::String;
        
        /* Read in test data from the test JSON file: */
        APTestData firstSaved;
        APTestData secondSaved;
        APTestData unsaved;
        try
        {
            JSONFile testValues(testFilePath);
            firstSaved.hashString = testValues.getProperty<String>
                    (TestKeys::FirstSavedAP::hash);
            firstSaved.invalidPSK = testValues.getProperty<String>
                    (TestKeys::FirstSavedAP::badPSK);
            firstSaved.validPSK = testValues.getProperty<String>
                    (TestKeys::FirstSavedAP::goodPSK);

            secondSaved.hashString = testValues.getProperty<String>
                    (TestKeys::SecondSavedAP::hash);
            secondSaved.invalidPSK = testValues.getProperty<String>
                    (TestKeys::SecondSavedAP::badPSK);
            secondSaved.validPSK = testValues.getProperty<String>
                    (TestKeys::SecondSavedAP::goodPSK);

            unsaved.hashString = testValues.getProperty<String>
                    (TestKeys::UnsavedAP::hash);
            unsaved.invalidPSK = testValues.getProperty<String>
                    (TestKeys::UnsavedAP::badPSK);
        }
        catch(JSONFile::FileException e)
        {
            logMessage(juce::String("Failed to open and parse test file ")
                    + e.getFilePath());
            logMessage(juce::String("This file is not included with this ")
                    + "project, and must be created as a JSON file defining "
                    + "valid access point test values as described at the start"
                    + " of Wifi_Test_Connection_Test.cpp.");
            jassertfalse;
            return;
        }
        catch(JSONFile::TypeException e)
        {
            logMessage(juce::String("Failed to find a test string value with ")
                    + "key \"" + e.getPropertyKey() + "\"");
            logMessage(juce::String("Make sure the JSON file at ") 
                    + testFilePath + " provides valid data at this key.");
            logMessage("Visible access point values:");
            Wifi::APList::Reader apListReader;
            juce::Array<AccessPoint> visibleAPs 
                    = apListReader.getAccessPoints();
            for(const AccessPoint& ap : visibleAPs)
            {
                logMessage(ap.toString());
            }

            jassertfalse;
            return;
        }

        Test::Listener updateListener;
        Control::Handler connectionController;
        Record::Reader recordReader;
        Wifi::APList::Reader apListReader;
        
        beginTest("Valid saved connection control test");
        Wifi::LibNM::APHash savedHash(firstSaved.hashString);

        Wifi::AccessPoint savedAP = apListReader.getAccessPoint(savedHash);
        expect(!savedAP.isNull(), 
                String("Failed to find saved AP from hash string, ensure ")
                + testFilePath + " provides valid test hash values.");

        if(recordReader.isConnected())
        {
            connectionController.disconnect();
            DelayUtils::idleUntil([&recordReader]()->bool
            {
                return !recordReader.isConnected();
            }, 100, 6500);
        }
        expect(!recordReader.isConnected(), "Failed to disconnect!");

        juce::int64 beforeConnection = juce::Time::currentTimeMillis();
        expect(recordReader.getLatestEvent().getEventTime().toMilliseconds()
                < beforeConnection, "Found invalid future connection event!");
        connectionController.connectToAccessPoint(savedAP);
        DelayUtils::idleUntil([&updateListener, &beforeConnection]()->bool
        {
            return updateListener.getLatestEvent().getEventTime()
                    .toMilliseconds() > beforeConnection;
        }, 100, 6500);

        expect(updateListener.getLatestEvent().getEventTime().toMilliseconds()
                    > beforeConnection, "No event recorded!");
        Event lastEvent = recordReader.getLatestEvent();
        expect(lastEvent == updateListener.getLatestEvent(), 
                "Failed to add event to record resource properly!");
        expect(lastEvent.getEventType() == EventType::connected,
                "Last event type was not a connection event!");
        expect(lastEvent.getEventAP() == savedAP,
                String("Last event had unexpected AP ") 
                + lastEvent.getEventAP().toString());
    }
};

static Wifi::Test::ConnectionTest connectionTest;

