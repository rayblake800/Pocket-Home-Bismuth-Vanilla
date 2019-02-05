#pragma once
/**
 * @file StressTest.h
 *
 * @brief Tests for thread safety by performing many simultaneous actions in
 *        many threads.
 */

#include <map>
#include "JuceHeader.h"

class TestAction
{
public:
    TestAction(
            const juce::String testName,
            const std::function<bool()> testAction,
            std::function<bool()> allowAction = []() { return true; });

    TestAction() { }

    virtual ~TestAction() { }

    const juce::String& getTestName() const;

    bool canRunAction();

    void runTestAction();

    int getActionCount() const;

    int getSuccessfulActionCount() const;

private:
    const juce::String testName;
    std::function<bool()> testAction;
    std::function<bool()> allowAction;
    juce::Atomic<int> testsRan;
    juce::Atomic<int> testsPassed;
};

class StressTest : public juce::UnitTest 
{
public:
    /**
     * Initialize a new StressTest.
     *
     * @param testName      A name identifying this StressTest.
     *
     * @param testCategory  The test's category name.
     *
     * @param minThreads    Minimum number of testing threads to run at once.
     *
     * @param maxThreads    Maximum number of testing threads to run at once.
     *
     * @param actionFreq    Longest amount of time (in milliseconds) that
     *                      threads should sleep between actions.
     *
     * @param testDuration  Duration to run tests, in seconds.
     */
    StressTest(
            const juce::String testName, 
            const juce::String testCategory,
            const int minThreads,
            const int maxThreads,
            const int actionFreq,
            const int testDuration);
            
protected:
    /**
     * Adds a new action for threads to randomly perform while testing. 
     *
     * @param testAction  A function that threads may randomly select to run
     *                    while testing.
     */
    void addAction(TestAction testAction);

    /**
     * Starts the test.  This creates the minimum number of threads, and allows
     * them to perform random actions for <testDuration> seconds.  This should
     * be called once in the runTest() method of StressTest unit tests.
     */
    void runThreads();
    
private:
    /**
     * Randomly selects and performs actions for the duration of a test. 
     */
    class TestThread : public juce::Thread
    {
    public:
        /**
         * Initializes a new test thread
         *
         * @param testSource  The test object creating this thread.
         */
        TestThread(StressTest& testSource);

        virtual ~TestThread() { }

    private:
        /**
         * Runs a random function from testSource.testActions, then sleeps for
         * a random amount of time less than testSource.actionFreq millseconds.
         */
        virtual void run() override;
        
        /**
         * The UnitTest that created the thread, used to access actions and
         * frequency.
         */
        StressTest& testSource;
    };

    //All actions that TestThreads may select while testing.
    juce::Array<TestAction> testActions;
    //Minimum number of TestThreads to run at once.
    const int minThreads;
    //Maximum number of TestThreads to run at once.
    const int maxThreads;
    //Maximum amount of time for threads to sleep between actions, in 
    //milliseconds.
    const int actionFreq;
    //Amount of time to allow TestThreads to act, in seconds.
    const int testDuration;
    //Saves the time an active test should end, in milliseconds since the epoch.
    juce::uint64 endTime;
    //Holds all active test threads.
    juce::OwnedArray<TestThread, juce::CriticalSection> threads;
};
