#include "StressTest.h"

/*
 * Initialize a new StressTest.
 */
StressTest::StressTest(juce::String testName, int minThreads, int maxThreads,
int actionFreq, int testDuration) :juce::UnitTest(testName),
    minThreads(minThreads), maxThreads(maxThreads),
    actionFreq(actionFreq), testDuration(testDuration)
{
    using namespace juce;
    addAction([this]()
            {
                const ScopedLock threadLock(threads.getLock());
                if(threads.size() < this->maxThreads)
                {
                    threads.add(new TestThread(*this));
                    threads.getLast()->startThread();
                }
            });
    
    addAction([this]()
            {
                const ScopedLock threadLock(threads.getLock());
                if(threads.size() > this->minThreads)
                {
                    int threadNum = getRandom().nextInt() % threads.size();
                    threads[threadNum]->signalThreadShouldExit();
                }
            });
}

/*
 * Adds a new action for threads to randomly perform while testing. 
 */
void StressTest::addAction(std::function<void()> testAction)
{
    testActions.add(testAction);
}

/*
 * Starts the test.  This creates the minimum number of threads, and allows
 * them to perform random actions for <testDuration> seconds.  This should
 * be called once in the StressTest::runTest() method of StressTest unit tests.
 */
void StressTest::runThreads()
{
    using namespace juce;
    endTime = Time::currentTimeMillis() + 1000 * testDuration;
    const ScopedLock threadLock(threads.getLock());
    while(threads.size() < minThreads)
    {
        threads.add(new TestThread(*this));
        threads.getLast()->startThread();
    }
    const ScopedUnlock threadUnlock(threads.getLock());
    while(!threads.isEmpty())
    {
        for(int i = 0; i < threads.size(); i++)
        {
            if(threads[i] == nullptr || !threads[i]->isThreadRunning())
            {
                threads.remove(i);
                i--;
            }
        }
    }
}

/*
 * Initializes a new test thread
 */
StressTest::TestThread::TestThread(StressTest& testSource) :
juce::Thread(testSource.getName() + " Thread"), testSource(testSource) { }

/*
 * Runs a random function from testSource.testActions, then sleeps for
 * a random amount of time less than testSource.actionFreq millseconds.
 */
void StressTest::TestThread::run()
{
    using namespace juce;
    while(!threadShouldExit() && Time::currentTimeMillis() < testSource.endTime)
    {
        testSource.testActions[testSource.getRandom().nextInt()
                % testSource.testActions.size()]();
        sleep(testSource.getRandom().nextInt() % testSource.actionFreq);
    }

}
