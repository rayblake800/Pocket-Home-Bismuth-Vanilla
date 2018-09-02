#include "StressTest.h"
#include "GLibSignalThread.h"
#include "WifiStateManager.h"
#include "LibNMInterface.h"

static const constexpr int minThreadCount = 5;
static const constexpr int maxThreadCount = 15;
static const constexpr int threadSleepMS = 300;
static const constexpr int testSeconds = 10;

static const constexpr int resourceInitCount = 50;

class WifiStateManagerTest : public StressTest
{
private:
    class TestListener : public WifiStateManager::Listener
    {
    public:
        TestListener(WifiStateManagerTest& test) : test(test) { }
    private:
        virtual void wifiStateChanged(WifiStateManager::WifiState state) 
        override
        {
            test.expectNotEquals(state, WifiStateManager::missingNetworkDevice);
        }

        virtual void accessPointAdded(const WifiAccessPoint& addedAP) override
        {
            test.expect(!addedAP.isNull(), "Added null access point!");
            test.expect(addedAP.getSSID().isNotEmpty(),
                    "Access point has null SSID!");
        }

        virtual void accessPointRemoved(const WifiAccessPoint& removedAP)
        override
        {
            test.expect(!removedAP.isNull(), "Added null access point!");
            test.expect(removedAP.getSSID().isNotEmpty(),
                    "Access point has null SSID!");
        }
        
        WifiStateManagerTest& test;
    };

    juce::OwnedArray<TestListener, juce::CriticalSection> listeners;
public:
    WifiStateManagerTest() : StressTest("WifiStateManager testing", 
            minThreadCount, maxThreadCount, threadSleepMS, testSeconds) 
    {
        /*
        //enable wifi
        addAction([this]()
                {
                    WifiStateManager wifiManager;
                    wifiManager.enableWifi();
                });
        //disable wifi
        addAction([this]()
                {
                    WifiStateManager wifiManager;
                    wifiManager.disableWifi();
                });
        //create and add new listener
        addAction([this]()
                {
                    WifiStateManager wifiManager;
                    TestListener * listener = new TestListener(*this);
                    wifiManager.addListener(listener);
                    const juce::ScopedLock arrayLock(listeners.getLock());
                    listeners.add(listener);
                });
        //create a new listener, without adding
        addAction([this]()
                {
                    WifiStateManager wifiManager;
                    TestListener * listener = new TestListener(*this);
                    const juce::ScopedLock arrayLock(listeners.getLock());
                    listeners.add(listener);
                }); 
                */
        //create and add a new listener, then let it go out of scope.
        addAction([this]()
                {
                    WifiStateManager wifiManager;
                    TestListener listener(*this);
                    wifiManager.addListener(&listener);
                });
        //destroy a listener
        addAction([this]()
                {
                    WifiStateManager wifiManager;
                    const juce::ScopedLock arrayLock(listeners.getLock());
                    if(!listeners.isEmpty())
                    {
                        listeners.remove(abs(getRandom().nextInt())
                                % listeners.size());
                    }
                });
    }

    virtual ~WifiStateManagerTest() { }

    void runTest() override
    {
        using namespace juce;
        //initialize shared resources
        GLibSignalThread signalThread;
        
        ScopedPointer<WifiStateManager>  wifiManager;
        beginTest("Repeated initialization/destruction test");

        for(int i = 0; i < resourceInitCount; i++)
        {
            wifiManager = new WifiStateManager([]
            (juce::ReadWriteLock& lock)->WifiStateManager::NetworkInterface*
            {
                return new LibNMInterface(lock);
            });
            if(i < resourceInitCount - 1)
            {
                wifiManager = nullptr;
            }
        }
        //If we haven't segfaulted by now, that's enough to pass.
        //TODO: check memory use for leaks
        expect(true);

        beginTest("threading stress test");
        runThreads();
        listeners.clear();
        wifiManager->enableWifi();
        wifiManager = nullptr;
    }
};

static WifiStateManagerTest test;
