#include "StressTest.h"

TestAction::TestAction(
        const juce::String testName,
        const std::function<bool()> testAction,
        const std::function<bool()> allowAction) :
testName(testName),
testAction(testAction),
allowAction(allowAction),
testsPassed(0),
testsRan(0) { }

const juce::String& TestAction::getTestName() const
{
    return testName;
}

bool TestAction::canRunAction()
{
    return allowAction();
}

void TestAction::runTestAction()
{
    jassert(allowAction);
    jassert(testAction);
    if(canRunAction())
    {
       const bool testPassed = testAction();
       jassert(testPassed);
       if(testPassed)
       {
           testsPassed.operator++();
       }
       else
       {
            DBG("Failed " << testName);
       }
       testsRan.operator++();
    }
    else
    {
        DBG("Couldn't run " << testName);
    }
}

int TestAction::getActionCount() const
{
    return testsRan.get();
}

int TestAction::getSuccessfulActionCount() const
{
    return testsPassed.get();
}

/*
 * Initialize a new StressTest.
 */
StressTest::StressTest(
        const juce::String testName,
        const juce::String testCategory,
        const int minThreads,
        const int maxThreads,
        const int actionFreq, 
        const int testDuration) :
juce::UnitTest(testName, testCategory),
minThreads(minThreads),
maxThreads(maxThreads),
actionFreq(actionFreq),
testDuration(testDuration)
{
    using juce::ScopedLock;

    TestAction addThread("Create new test thread", [this]()
    {
        const ScopedLock threadLock(threads.getLock());
        if(threads.size() < this->maxThreads)
        {
            threads.add(new TestThread(*this));
            threads.getLast()->startThread();
        }
        return true;
    },[this]()
    {
        const ScopedLock threadLock(threads.getLock());
        return threads.size() < this->maxThreads;
    });

    TestAction removeThread("Destroy test thread", [this]()
    {
        const ScopedLock threadLock(threads.getLock());
        if(threads.size() > this->minThreads)
        {
            int threadNum = abs(getRandom().nextInt()) % threads.size();
            threads[threadNum]->signalThreadShouldExit();
        }
        return true;
    },[this]()
    {
        const ScopedLock threadLock(threads.getLock());
        return threads.size() > this->minThreads;
    });
    addAction(addThread);
    addAction(removeThread);
}

/*
 * Adds a new action for threads to randomly perform while testing. 
 */
void StressTest::addAction(TestAction testAction)
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
    for(int i = 0; i < testActions.size(); i++)
    {
        String log;
        expectEquals(testActions[i].getSuccessfulActionCount(),
                testActions[i].getActionCount(),
                testActions[i].getTestName() + ": not all tests passed.");

        log << "Test \"" << testActions[i].getTestName() << "\": " 
            << juce::String(testActions[i].getSuccessfulActionCount()) << "/"
            << juce::String(testActions[i].getActionCount()) 
            << " tests passed.";
        logMessage(log);
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
        juce::Array<int> validTestIndices;
        for(int i = 0; i < testSource.testActions.size(); i++)
        {
            validTestIndices.add(i);
        }

        while(!validTestIndices.isEmpty())
        {
            int metaIndex = abs(Random::getSystemRandom().nextInt())
                % validTestIndices.size();
            int index = validTestIndices[metaIndex];
            validTestIndices.remove(metaIndex);
            if(testSource.testActions[index].canRunAction())
            {
                TestAction& testAction 
                        = testSource.testActions.getReference(index);
                testAction.runTestAction();
                break;
            }
        }
        sleep(testSource.getRandom().nextInt() % testSource.actionFreq);
    }

}
