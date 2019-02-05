#include "JuceHeader.h"
#include "ShutdownListener.h"

/**
 * @brief  A fake application class to trigger the shutdown notification. 
 */
class PocketHomeApplication
{
public:
    /**
     * @brief  Sends a fake shutdown broadcast message.
     */
    static void broadcastShutdown()
    {
        ShutdownBroadcaster::broadcastShutdown();
    }
};


/**
 * @brief  A basic implementation of ShutdownListener for testing.
 */
class TestListener : public ShutdownListener
{
private:
    /* Tracks the number of times that any ShutdownListener ran onShutdown()
       since the test began: */
    int& notifyCount;

    /**
     * @brief  Updates the notification count when the shutdown message is
     *         received.
     */
    virtual void onShutdown() override
    {
        notifyCount++;
    }

public:
    /**
     * @brief  Saves a reference to the shared notification count.
     *
     * @param notifyCount  The variable tracking notification count for all
     *                     TestListener objects.
     */
    TestListener(int& notifyCount) : notifyCount(notifyCount) { }
};

class ShutdownListenerTest : public juce::UnitTest
{
public:
    ShutdownListenerTest() : juce::UnitTest("ShutdownListener Testing",
            "Util") {}
    
    void runTest() override
    {
        juce::OwnedArray<TestListener> testListeners;
        int notifyCount = 0;
        beginTest("Shutdown notification testing");
        
        /* Make sure the shutdown notification runs safely if no listeners 
           exist: */
        PocketHomeApplication::broadcastShutdown();

        int listenerCount = 5;
        for(int i = 0; i < listenerCount; i++)
        {
            testListeners.add(new TestListener(notifyCount));
        }
        PocketHomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                "Notification count does not match listener count.");
        
        notifyCount = 0;
        listenerCount --;
        testListeners.removeLast();
        PocketHomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                juce::String("After removing listener, notification count does")
                + juce::String("not match listener count."));
        
        notifyCount = 0;
        listenerCount += 2;
        testListeners.add(new TestListener(notifyCount));
        testListeners.add(new TestListener(notifyCount));
        PocketHomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                juce::String("After adding listeners, notification count does")
                + juce::String("not match listener count."));
        
        notifyCount = 0;
        listenerCount = 0;
        testListeners.clear();
        PocketHomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                juce::String("After clearing listeners, notification count ")
                + juce::String("does not match listener count."));

    }
};

static ShutdownListenerTest test;
