/**
 * @file  Wifi_APList_ListTest.h
 *
 * @brief  Tests the Wifi resource's APList module.
 */

#define WIFI_IMPLEMENTATION
#include "JuceHeader.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_APList_NMReader.h"
#include "Wifi_APList_Writer.h"
#include "Wifi_Device_Controller.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_TestUtils_Waiting.h"
#include "SharedResource_Handler.h"
#include "DelayUtils.h"

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

        beginTest("APList LibNM Reading Test");
        Wifi::LibNM::Thread::Handler threadHandler;
        threadHandler.call([this, &visibleAPs]()
        {
            APList::NMReader nmReader;
            for(AccessPoint& accessPoint : visibleAPs)
            {
            Wifi::LibNM::AccessPoint strongestAP 
                        = nmReader.getStrongestNMAccessPoint(accessPoint);
                expect(!strongestAP.isNull(), 
                        juce::String("Couldn't find nmAP for access point ")
                        + accessPoint.getSSID().toString());

                int maxStrength = strongestAP.getSignalStrength();
                juce::Array<Wifi::LibNM::AccessPoint> nmAPList
                        = nmReader.getNMAccessPoints(accessPoint);
                expect(!nmAPList.isEmpty(),
                        juce::String("Empty nmAP list for access point ")
                        + accessPoint.getSSID().toString());

                logMessage(juce::String("AP ") 
                        + accessPoint.getSSID().toString()
                        + ": Found " + juce::String(nmAPList.size())
                        + " LibNM access point(s).");

                for(Wifi::LibNM::AccessPoint& nmAP : nmAPList)
                {
                    expectEquals(accessPoint.getSSID().toString(),
                            nmAP.getSSIDText(),
                            "Matching NMAP list contains non-matching AP");
                    expectLessOrEqual((int) nmAP.getSignalStrength(),
                            maxStrength, 
                            "Found stronger NM access point!");
                }
            }
        });
    }
};

static Wifi::APList::ListTest listTest;

