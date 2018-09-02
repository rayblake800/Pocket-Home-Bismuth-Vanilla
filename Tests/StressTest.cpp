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
                    int threadNum = abs(getRandom().nextInt()) % threads.size();
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
    const ScopedLock threadLock(threads.getLock());
    endTime = Time::currentTimeMillis() + (100 * testDuration);
    while(threads.size() < minThreads)
    {
        threads.add(new TestThread(*this));
        threads.getLast()->startThread();
    }
    const ScopedUnlock threadUnlock(threads.getLock());
    while(!threads.isEmpty())
    {
        Thread::sleep(testDuration * 1000);
        for(int i = 0; i < threads.size(); i++)
        {
            if(threads[i] == nullptr || !threads[i]->isThreadRunning())
            {
                threads.remove(i);
                i--;
            }
        }
    }
    for(int i = 0; i < threadActionCounts.size(); i++)
    {
        
        String log;
        log << "Thread " << i << " ran " 
            << threadActionCounts[i] << " actions.";
        logMessage(log);
    }
    for(int i = 0; i < actionRunCounts.size(); i++)
    {
        String log;
        log << "Action " << i << " executed " 
            << actionRunCounts[i] << " times.";
        logMessage(log);
    }
    threadActionCounts.clear();
    actionRunCounts.clear();
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
        int threadNum = testSource.threads.indexOf(this);
        testSource.threadActionCounts.set
            (threadNum, testSource.threadActionCounts[threadNum] + 1);

        int actionNum = abs(Random::getSystemRandom().nextInt())
            % testSource.testActions.size();
        testSource.actionRunCounts.set
            (actionNum, testSource.actionRunCounts[actionNum] + 1);
        
        testSource.testActions[actionNum]();
        sleep(testSource.getRandom().nextInt() % testSource.actionFreq);
    }

}
