/**
 * @file  Wifi_Test_ConnectionTest.cpp
 *
 * @brief  Tests the Wifi resources's ability to control Wifi connections.
 */
#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Control_Handler.h"
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_TestUtils_Waiting.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Device_Controller.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_AccessPoint.h"
#include "Files/JSONFile.h"
#include "DelayUtils.h"
#include "JuceHeader.h"

namespace Wifi { namespace Connection { namespace Control { 
    class ControlTest; } } }

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
 */
class Wifi::Connection::Control::ControlTest : public juce::UnitTest
{
public:
    ControlTest() : juce::UnitTest("Wifi::Connection::Control Testing",
            "Wifi") {}

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
     * @param earliestTime       Wifi events occurring before this time will
     *                           be ignored.
     *
     * @return                   The time the matching event occurred, or
     *                           earliestTime if there was no match.
     */
    juce::Time checkForWifiEvent(Connection::EventType expectedEventType,
            AccessPoint expectedEventAP, juce::Time earliestTime)
    {
        using juce::String;
        Record::Reader recordReader;
        Event latestEvent = TestUtils::Waiting::waitForNextConnectionEvent
                (earliestTime);
        expect(!latestEvent.isNull(),
                String("Waiting for event with EventType=")
                + eventTypeString(expectedEventType)
                + " but no new event occurred.");

        expect(latestEvent.getEventType() == expectedEventType,
                String("Unexpected new event type. Expected:")
                + eventTypeString(expectedEventType) + ", event:"
                + latestEvent.toString());

        expect(latestEvent.getEventAP() == expectedEventAP,
                String("Unexpected new event access point. Expected: ") 
                + expectedEventAP.toString() + ", event:"
                + latestEvent.toString());

        if(latestEvent.getEventAP() == expectedEventAP
                && latestEvent.getEventType() == expectedEventType)\
        {
            return latestEvent.getEventTime();
        }
        return earliestTime;
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

        Wifi::APList::Reader apListReader;
        expect(!recordReader.isConnected(), 
                "Failed to disconnect from Wifi connection before tests");

        Wifi::LibNM::APHash savedHash(firstSaved.hashString);
        logMessage("Scanning for matching access point...");
        Wifi::AccessPoint savedAP 
                = TestUtils::Waiting::waitForAccessPoint(savedHash); 
        expect(!savedAP.isNull(),
                String("Failed to find saved AP from hash string, ensure ")
                + testFilePath + " provides valid test hash values.");

        juce::Time earliestTime = juce::Time::getCurrentTime();
        connectionController.connectToAccessPoint(savedAP);
        earliestTime = checkForWifiEvent(EventType::startedConnecting, savedAP,
                earliestTime);
        earliestTime = checkForWifiEvent(EventType::connected, savedAP, 
                earliestTime);
        connectionController.disconnect();
        earliestTime = checkForWifiEvent(EventType::disconnected, savedAP, 
                earliestTime);
    }
};

static Wifi::Connection::Control::ControlTest controlTest;

