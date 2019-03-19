/**
 * @file  Wifi_APList_ListTest.h
 *
 * @brief  Tests the Wifi resource's APList module.
 */

#define WIFI_IMPLEMENTATION
#include "JuceHeader.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_Device_Controller.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_TestUtils_Waiting.h"
#include "SharedResource_Handler.h"

namespace Wifi { namespace APList { class ListTest; } }

/**
 * @brief  Tests the Wifi::APList::Module class.
 */
class Wifi::APList::ListTest : public juce::UnitTest
{
public:
    ListTest() : juce::UnitTest("Wifi::APList Testing",
            "Wifi") {}
    
    void runTest() override
    {
        beginTest("APList Reading Test");
        // Make sure Wifi is turned on:
        Device::Controller wifiController;
        wifiController.setEnabled(true);

        logMessage("Waiting for access points to load...");
        expect(TestUtils::Waiting::waitForAccessPoints(),
                "Failed to find multiple access points.");

        APList::Reader listReader;
        juce::Array<AccessPoint> visibleAPs = listReader.getAccessPoints();
        logMessage(juce::String("Found ") + juce::String(visibleAPs.size())
                + " access points.");

        for(AccessPoint& visibleAP : visibleAPs)
        {
            DBG(visibleAP.toString());
            expect(!visibleAP.isNull(), "Received null access point!");

            Wifi::LibNM::APHash hashValue = visibleAP.getHashValue();
            expect(!hashValue.isNull(), "Failed to generate hash value!");

            AccessPoint matchingAP = listReader.getAccessPoint(hashValue);
            expect(!matchingAP.isNull(),
                    "Failed to look up access point by hash!");
            expect(hashValue == matchingAP.getHashValue(),
                    "Searching by hash value returned AP with wrong hash!");
            expect(hashValue == visibleAP.getHashValue(),
                    "Original AP's hash value changed!");
            expect(visibleAP == matchingAP,
                    "Searching for AP by hash returns a different AP!");
        }
    }
};

static Wifi::APList::ListTest listTest;

