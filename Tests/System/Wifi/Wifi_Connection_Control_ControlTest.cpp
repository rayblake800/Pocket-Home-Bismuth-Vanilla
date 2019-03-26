/**
 * @file  Wifi_Test_ConnectionTest.cpp
 *
 * @brief  Tests the Wifi resources's ability to control Wifi connections.
 */
#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_Handler.h"
#include "Wifi_Connection_Record_Handler.h"
#include "Wifi_Connection_Saved_Deleter.h"
#include "Wifi_TestUtils_Waiting.h"
#include "Wifi_TestUtils_ConnectionListener.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Device_Controller.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_AccessPoint.h"
#include "Assets_JSONFile.h"
#include "Testing_DelayUtils.h"
#include "JuceHeader.h"

namespace Wifi 
{ 
    namespace Connection 
    { 
        namespace Control 
        { 
            class ControlTest;
        } 
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
 * @brief  Tests the Wifi::Connection::Control module.
 *
 *   This test is far from complete, lots of unexpected results from 
 * NetworkManager keep turning up, and differences in NetworkManager behavior
 * between different versions are not handled appropriately.
 */
class Wifi::Connection::Control::ControlTest : public juce::UnitTest
{
private:
    /* Tracks Wifi events that are expected to occur.  The event times in this
     * queue's events should be ignored. */
    juce::Array<Wifi::Connection::Event> expectedEventQueue;

    /**
     * @brief  Listens for new Wifi events, and tests if they match the next
     *         expected event.
     */
    class EventWatcher : public Wifi::TestUtils::ConnectionListener
    {
    private:
        ControlTest& controlTest;

    public:
        EventWatcher(ControlTest& controlTest) : controlTest(controlTest) { }

        /**
         * @brief  Whenever a new connection event is added, tests if that
         *         event was expected.
         *
         * @param newEvent  The latest event added to the Record::Resource.
         */
        virtual void eventAdded(const Wifi::Connection::Event newEvent)
        {
            using juce::String;
            controlTest.expect(
                    !controlTest.expectedEventQueue.isEmpty(),
                    String("Unexpected event ") + newEvent.toString());

            if(!controlTest.expectedEventQueue.isEmpty())
            {
                Event expectedEvent = controlTest.expectedEventQueue.getLast();
                controlTest.expectedEventQueue.removeLast();
                controlTest.expect((expectedEvent.getEventType() 
                            == newEvent.getEventType())
                        && (expectedEvent.getEventAP() 
                            == newEvent.getEventAP()),
                        String("Unexpected new event. Expected: ")
                        + expectedEvent.toString() + ", found: "
                        + newEvent.toString());
            }
        }
    };

    /**
     * @brief  Adds a new expected Wifi event to end of the list of expected
     *         events.
     *
     * @param eventType  The type of event that should occur.
     *
     * @param eventAP    The access point that should be associated with the
     *                   event.
     */
    void expectEvent(const EventType eventType, const AccessPoint eventAP)
    {
        expectedEventQueue.insert(0, Event(eventAP, eventType));
    }

    /**
     * @brief  Test that all expected wifi events occur within a specific time
     *         period.
     *
     * @param millisToWait  Maximum number of milliseconds to wait for events to
     *                      occur.
     */
    void waitForAllExpected(int millisToWait)
    {
        expect(Testing::DelayUtils::idleUntil([this]()
        {
            return expectedEventQueue.isEmpty();
        }, 200, millisToWait),
                juce::String(expectedEventQueue.size()) 
                + juce::String(" expected connection events did not occur."));
    }

public:
    ControlTest() : juce::UnitTest("Wifi::Connection::Control Testing",
            "Wifi") {}

    void runTest() override
    {
        using namespace Wifi::Connection;
        using juce::String;

        Device::Controller deviceController;
        Control::Handler connectionController;
        Record::Handler recordHandler;

        // If Wifi is connected, save the current connected AP to restore after
        // tests:
        LibNM::APHash initialAPHash;
        if(recordHandler.isConnected())
        {
            AccessPoint initialAP = recordHandler.getActiveAP();
            initialAPHash = initialAP.getHashValue();
            beginTest(String("Saving and disconnecting existing connection ")
                    + initialAP.toString());
            connectionController.disconnect();
            /* The initial AP connection event may or may not make it through
             * before the disconnect event, so an EventWatcher and 
             * waitForAllExpected() aren't usable here. */
            juce::MessageManager::getInstance()->runDispatchLoopUntil(1000);
        }
        else
        {
            // Make sure Wifi is enabled:
            deviceController.setEnabled(true);
        }
        expect(!recordHandler.isConnected(),
                "Failed to close existing connection.");
        EventWatcher connectionEventWatcher(*this);
        
        
        /* Read in test data from the test JSON file: */
        APTestData firstAP;
        APTestData secondAP;
        APTestData thirdAP;
        try
        {
            Assets::JSONFile testValues(testFilePath);
            firstAP.hashString = testValues.getProperty<String>
                    (TestKeys::FirstSavedAP::hash);
            firstAP.invalidPSK = testValues.getProperty<String>
                    (TestKeys::FirstSavedAP::badPSK);
            firstAP.validPSK = testValues.getProperty<String>
                    (TestKeys::FirstSavedAP::goodPSK);

            secondAP.hashString = testValues.getProperty<String>
                    (TestKeys::SecondSavedAP::hash);
            secondAP.invalidPSK = testValues.getProperty<String>
                    (TestKeys::SecondSavedAP::badPSK);
            secondAP.validPSK = testValues.getProperty<String>
                    (TestKeys::SecondSavedAP::goodPSK);

            thirdAP.hashString = testValues.getProperty<String>
                    (TestKeys::UnsavedAP::hash);
            thirdAP.invalidPSK = testValues.getProperty<String>
                    (TestKeys::UnsavedAP::badPSK);
        }
        catch(Assets::JSONFile::FileException e)
        {
            logMessage(juce::String("Failed to open and parse test file ")
                    + e.getFilePath());
            logMessage(juce::String("This file is not included with this ")
                    + "project, and must be created as a JSON file defining "
                    + "valid access point test values as described at the start"
                    + " of Wifi_Connection_ControlTest.cpp");
            jassertfalse;
            return;
        }
        catch(Assets::JSONFile::TypeException e)
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

        /* First main connection test: */
        beginTest("Valid saved connection test");
        expect(!recordHandler.isConnected(), 
                "Failed to disconnect from Wifi connection before tests");

        LibNM::APHash savedAPHash(firstAP.hashString);
        logMessage("Scanning for first test access point...");
        Wifi::AccessPoint savedAP 
                = TestUtils::Waiting::waitForAccessPoint(savedAPHash); 
        expect(!savedAP.isNull(),
                String("Failed to find saved AP from hash string, ensure ")
                + testFilePath + " provides valid test hash values.");

        expectEvent(EventType::connectionRequested, savedAP);
        expectEvent(EventType::startedConnecting, savedAP);
        expectEvent(EventType::connected, savedAP);
        connectionController.connectToAccessPoint(savedAP);
        waitForAllExpected(15000);
        expectEvent(EventType::disconnected, savedAP);
        connectionController.disconnect();
        waitForAllExpected(1000);
        
        /* Second main connection test: */
        beginTest("Valid unsaved connection test");
        LibNM::APHash unsavedAPHash = secondAP.hashString;
        logMessage("Scanning for second test access point..."); 
        Wifi::AccessPoint unsavedAP 
                = TestUtils::Waiting::waitForAccessPoint(unsavedAPHash); 
        Saved::Deleter savedConnectionDeleter;
        savedConnectionDeleter.removeSavedConnection(unsavedAP);

        logMessage("Attempting to connect without a saved PSK");
        connectionController.connectToAccessPoint(unsavedAP);
        // Wait briefly for an event that shouldn't occur:
        juce::MessageManager::getInstance()->runDispatchLoopUntil
            (2000);
        
        logMessage("Attempting to connect with an invalid saved PSK");
        expectEvent(EventType::connectionRequested, unsavedAP);
        expectEvent(EventType::startedConnecting, unsavedAP);
        //expectEvent(EventType::connectionAuthFailed, unsavedAP);
        expectEvent(EventType::connectionFailed, unsavedAP);
        connectionController.connectToAccessPoint(unsavedAP,
                secondAP.invalidPSK);
        waitForAllExpected(15000);
        
        logMessage("Attempting to connect with a valid saved PSK");
        expectEvent(EventType::connectionRequested, unsavedAP);
        expectEvent(EventType::startedConnecting, unsavedAP);
        expectEvent(EventType::connected, unsavedAP);
        connectionController.connectToAccessPoint(unsavedAP, secondAP.validPSK);
        waitForAllExpected(15000);

        /* Third main connection test: */
        beginTest("Invalid unsaved connection test");
        LibNM::APHash invalidAPHash = thirdAP.hashString;
        logMessage("Scanning for third test access point...");
        Wifi::AccessPoint invalidAP
                = TestUtils::Waiting::waitForAccessPoint(invalidAPHash);

        logMessage("Attempting to connect without a saved PSK");
        connectionController.connectToAccessPoint(invalidAP);
        // Wait briefly for an event that shouldn't occur:
        juce::MessageManager::getInstance()->runDispatchLoopUntil
            (2000);
        
        logMessage("Attempting to connect with an invalid saved PSK");
        expectEvent(EventType::connectionRequested, invalidAP);
        expectEvent(EventType::startedConnecting, invalidAP);
        //expectEvent(EventType::connectionAuthFailed, invalidAP);
        expectEvent(EventType::connectionFailed, invalidAP);
        connectionController.connectToAccessPoint(invalidAP,
                thirdAP.invalidPSK);
        waitForAllExpected(15000);

        if(!initialAPHash.isNull())
        {
            beginTest("Restoring original Wifi connection");
            Wifi::AccessPoint initialAP 
                    = TestUtils::Waiting::waitForAccessPoint(initialAPHash); 
            expectEvent(EventType::connectionRequested, initialAP);
            expectEvent(EventType::startedConnecting, initialAP);
            expectEvent(EventType::connected, initialAP);
            connectionController.connectToAccessPoint(initialAP);
            waitForAllExpected(15000);
        }
    }
};

static Wifi::Connection::Control::ControlTest controlTest;

