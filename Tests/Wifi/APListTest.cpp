#define WIFI_IMPLEMENTATION
#include "JuceHeader.h"
#include "Wifi_Manager.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_APList_NMReader.h"
#include "Wifi_APList_Writer.h"
#include "Wifi_AccessPoint.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "GLib_DBusThreadRunner.h"
#include "SharedResource_Handler.h"

class APListTest : public juce::UnitTest
{
public:
    APListTest() : juce::UnitTest("Wifi::APListTest Testing",
            "Wifi") {}
    
    void runTest() override
    {
        using namespace Wifi;
        Manager wifiManager;

        {
            beginTest("APList Reading Test");
            APList::Reader listReader;
            juce::Array<AccessPoint> visibleAPs = listReader.getAccessPoints();
            logMessage(juce::String("Found ") + juce::String(visibleAPs.size())
                    + " access points.");
            for(AccessPoint& visibleAP : visibleAPs)
            {
                DBG(visibleAP.toString());
                expect(!visibleAP.isNull(), "Received null access point!");

                LibNM::APHash hashValue = visibleAP.getHashValue();
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
            LibNM::ThreadHandler threadHandler;
            threadHandler.call([this, &visibleAPs]()
            {
                APList::NMReader nmReader;
                for(AccessPoint& accessPoint : visibleAPs)
                {
                    LibNM::AccessPoint strongestAP 
                            = nmReader.getStrongestNMAccessPoint(accessPoint);
                    expect(!strongestAP.isNull(), 
                            juce::String("Couldn't find nmAP for access point ")
                            + accessPoint.getSSID().toString());

                    int maxStrength = strongestAP.getSignalStrength();
                    juce::Array<LibNM::AccessPoint> nmAPList
                            = nmReader.getNMAccessPoints(accessPoint);
                    expect(!nmAPList.isEmpty(),
                            juce::String("Empty nmAP list for access point ")
                            + accessPoint.getSSID().toString());

                    logMessage(juce::String("AP ") 
                            + accessPoint.getSSID().toString()
                            + ": Found " + juce::String(nmAPList.size())
                            + " LibNM access point(s).");

                    for(LibNM::AccessPoint& nmAP : nmAPList)
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
    }
};

static APListTest listTest;

