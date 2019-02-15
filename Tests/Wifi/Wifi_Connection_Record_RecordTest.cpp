/**
 * @file  Wifi_Test_ConnectionTest.cpp
 *
 * @brief  Tests the Wifi resources's ability to control Wifi connections.
 */
#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_Handler.h"
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_Connection_Record_Writer.h"
#include "Wifi_Connection_Record_Listener.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Device_Controller.h"
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

/* Number of milliseconds to wait for access points to load: */
static const constexpr int apLoadingPeriod = 12000;

/* Frequency in milliseconds to check access points when waiting for them to 
 * load: */
static const constexpr int apCheckFrequency = 1000;

/* Number of milliseconds to wait for connection events to occur: */
static const constexpr int connectionWaitPeriod = 6500;

/* Frequency in milliseconds to check if an expected connection event has 
 * occurred: */
static const constexpr int connectionCheckFrequency = 100;

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
        DBG("Wifi::Test::Listener::updateLastEvent: New Event:" 
                << lastEvent.toString());
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
        updateLastEvent(connectedAP, EventType::connected);
    }

    /* Update the last event when a connection is closed: */
    virtual void disconnected
    (const Wifi::AccessPoint disconnectedAP) override
    {
        updateLastEvent(disconnectedAP, EventType::disconnected);
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

/**
 * @brief  Tests the Wifi::Connection::Control module.
 */
class Wifi::Test::ConnectionTest : public juce::UnitTest
{
public:
    ConnectionTest() : juce::UnitTest("Wifi::Connection Testing",
            "Wiffi") {}

    /**
     * @brief  Tests if the next Wifi event to occur has a specific event type
     *         and access point.
     * 
     *  If the last recorded connection event already matches the expected 
     * event right away, the tests will immediately pass. Otherwise, the 
     * function will wait for the next registered event, and tests will pass
     * only if that event occurs before the connection wait period ends, and it
     * has the correct event type and access point. 
     *
     * @param expectedEventType  The type of connection event that should occur.
     *
     * @param expectedEventAP    The access point that should be used by the
     *                           event.
     *
     * @param updateListener     The listener used to track event updates.
     */
    void checkForWifiEvent( Connection::EventType expectedEventType,
            AccessPoint expectedEventAP, Listener& updateListener)
    {
        using namespace Wifi::Connection;
        using juce::String;
        Record::Reader recordReader;
        Event lastEvent = updateListener.getLatestEvent();
        if(lastEvent.getEventType() == expectedEventType
                && lastEvent.getEventAP() == expectedEventAP)
        {
            // Event already happened, no need to wait
            return;
        }
        
        Event nextEvent;
        expect(DelayUtils::idleUntil([&lastEvent, &nextEvent, &updateListener]()
        {
            nextEvent = updateListener.getLatestEvent();
            return nextEvent != lastEvent;
        }, 100, 6500), 
                String("Waiting for event with EventType=")
                + eventTypeString(expectedEventType)
                + " but no new event occurred.");

        expect(nextEvent == updateListener.getLatestEvent(), 
                "Failed to add event to record resource properly!");
        expect(nextEvent.getEventType() == expectedEventType,
                String("Unexpected new event type. Expected:")
                + eventTypeString(expectedEventType) + ", event:"
                + nextEvent.toString());
        expect(nextEvent.getEventAP() == expectedEventAP,
                String("Unexpected new event access point. Expected: ") 
                + expectedEventAP.toString() + ", event:"
                + nextEvent.toString());
    }
    
    void runTest() override
    {
        using namespace Wifi::Connection;
        using juce::String;

        // Make sure Wifi is enabled and disconnected:
        Device::Controller deviceController;
        Control::Handler connectionController;
        Record::Reader recordReader;
        deviceController.setEnabled(true);
        connectionController.disconnect();
        DelayUtils::idleUntil([&recordReader]()
        {
            return !recordReader.isConnected();
        }, 200, 1000);
        
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

        beginTest("Valid saved connection test");
        Wifi::LibNM::APHash savedHash(firstSaved.hashString);

        Wifi::APList::Reader apListReader;
        Listener updateListener;
        expect(!recordReader.isConnected(), 
                "Failed to disconnect from Wifi connection before tests");

        Wifi::AccessPoint savedAP; 
        expect(DelayUtils::idleUntil(
                [this, &savedAP, &savedHash, &apListReader]()
        {
            logMessage("Scanning for matching access point...");
            savedAP = apListReader.getAccessPoint(savedHash);
            return !savedAP.isNull();

        }, 1000, 10000),
                String("Failed to find saved AP from hash string, ensure ")
                + testFilePath + " provides valid test hash values.");

        connectionController.connectToAccessPoint(savedAP);
        waitForWifiEvent(EventType::startedConnecting, savedAP, updateListener);
        waitForWifiEvent(EventType::connected, savedAP, updateListener);
        connectionController.disconnect();
        waitForWifiEvent(EventType::disconnected, savedAP, updateListener);
    }
};

static Wifi::Test::ConnectionTest connectionTest;

