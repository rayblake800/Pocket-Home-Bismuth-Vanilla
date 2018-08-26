#include "JuceHeader.h"
/**
 * @file StressTest.h
 *
 * @brief Tests for thread safety by performing many simultaneous actions in
 *        many threads.
 */


class StressTest 
{
public:
    StressTest(int minThreads, int maxThreads, int testDuration);
            

    void addAction(std::function<void()> testAction;);

    void runTest() override;

    
private:
    juce::Array<std::function<void()> testActions;

    class TestThread : public juce::Thread
    {
    public:
        TestThread(StressTest& testSource);

        virtual ~TestThread();
    };


}
