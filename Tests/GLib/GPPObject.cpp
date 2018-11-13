#include "JuceHeader.h"
#include "gtest_object.h"
#include "../StressTest.h"
#include "GPPTestObject.h"

static const constexpr int minThreadCount = 30;
static const constexpr int maxThreadCount = 50;
static const constexpr int threadSleepMS = 300;
static const constexpr int testSeconds = 10;

class GPPObjectTest : public StressTest
{
private:
    //Holds all GPPObjects created during the stress test
    juce::OwnedArray<GPPTestObject> testObjects;
    //Holds all object listeners created during the stress test
    juce::OwnedArray<GPPTestObject::Listener> listeners;
    //GPPTestObjects and OwnedArrays are not threadsafe, prevent concurrent
    //access.
    juce::CriticalSection dataLock;

    //Provides a less verbose way to test reference count changes.
    class RefTest
    {
    public:
        /**
         * Initializes a GObject reference change test.
         *
         * @param testObject  The object with the reference count being tested.
         *
         * @param test        The unit test that created this RefTest.
         */
        RefTest(const GPPTestObject& testObject, GPPObjectTest& test) :
            testObject(testObject),
            test(test),
            initialRefCount(testObject.getReferenceCount()),
            nullObject(testObject.isNull()) { }

        virtual ~RefTest() { }


        /**
         * Checks if the GPPTestObject's references changed as expected.
         *
         * @param changeExpected  The expected difference between the current
         *                        reference count and the original count.  This
         *                        is rounded up to zero if it is negative and
         *                        the original reference count is zero.
         *
         * @param errorMsg        The message to print along with the expected
         *                        and actual values if the test fails.
         */
        void check(int changeExpected, juce::String errorMsg) const
        {
            int expectedCount = initialRefCount + changeExpected;
            if(expectedCount < 0)
            {
                expectedCount = 0;
            }
            test.expectEquals(testObject.getReferenceCount(), expectedCount,
                    errorMsg);
        }
    private:
        const GPPTestObject& testObject;
        GPPObjectTest& test;
        const bool nullObject;
        const int initialRefCount;
    };

    template<class ArrayType>
    int randomIndex(ArrayType& array)
    {
        using namespace juce;
        int index = -1;
        return abs(Random::getSystemRandom().nextInt())
                % testObjects.size();
        return index;
    }

    GPPTestObject randomObject()
    {
        using namespace juce;
        const ScopedLock objectLock(dataLock);
        if(testObjects.isEmpty())
        {
            return GPPTestObject();
        }
        int index = randomIndex(testObjects);
        return GPPTestObject(*testObjects[index]);
    }

    bool listenersEmpty()
    {
        using namespace juce;
        if(listeners.isEmpty())
        {
            return true;
        }
        for(GPPTestObject::Listener* l : listeners)
        {
            if(l != nullptr)
            {
                return false;
            }
        }
        return true;
    }

    GPPTestObject::Listener* randomListener()
    {
        using namespace juce;
        if(listenersEmpty())
        {
            return nullptr;
        }
        int index = -1;
        while(index < 0 || listeners[index] == nullptr)
        {
            index = randomIndex(listeners);
        }
        return listeners[index];
    }
public:
    GPPObjectTest() : StressTest("GTestObject testing", minThreadCount,
            maxThreadCount, threadSleepMS, testSeconds) 
    {
        using namespace juce;
        //add a new test object
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    testObjects.add(new GPPTestObject());
                    expectEquals(testObjects.getLast()->getReferenceCount(), 1,
                            "New object should have a reference count of 1!");
                });
        //remove a random test object
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    if(testObjects.size() > 1)
                    {
                        int index = randomIndex(testObjects);
                        GPPTestObject temp(*testObjects[index]);
                        RefTest refTest(temp, *this);
                        testObjects.remove(index);
                        refTest.check(-1,
                                "reference count error after deleting object");
                    }
                });
        //create a new listener, and add it to a random test object
        addAction([this]()
                {
                    GPPTestObject tempObject = randomObject();

                    const ScopedLock objectLock(dataLock);
                    GPPTestObject::Listener* listener =
                            new GPPTestObject::Listener();
                    RefTest refTest(tempObject, *this);
                    tempObject.addListener(*listener);
                    listeners.add(listener);
                    refTest.check((tempObject.isNull() ? 0 : 1), 
                            "Reference count error after adding new listener");
                });

        //add a random listener to a random test object
        addAction([this]()
                {
                    GPPTestObject tempObject = randomObject();
                    const ScopedLock objectLock(dataLock);
                    GPPTestObject::Listener* listener = randomListener();
                    if(listener != nullptr 
                            && !listener->isListening(tempObject))
                    {
                        RefTest refTest(tempObject, *this);
                        tempObject.addListener(*listener);
                        refTest.check((tempObject.isNull() ? 0 : 1), 
                                "Reference count error after adding listener");
                    }
                });


        //remove a random listener from a random test object
        addAction([this]()
                 {
                    GPPTestObject tempObject = randomObject();
                    
                    const ScopedLock objectLock(dataLock);
                    GPPTestObject::Listener* listener = randomListener();
                    if(listener != nullptr && listener->isListening(tempObject))
                    {
                        
                        RefTest refTest(tempObject, *this);
                        expect(tempObject.removeListener(*listener),
                                "Listener was not removed from signal source!");
                        refTest.check(-1,
                                "Removing listener did not release reference!");
                    }
                });

        //destroy a random listener
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    GPPTestObject::Listener* listener = randomListener();
                    if(listeners.size() < 2 || listener == nullptr)
                    {
                        return;
                    }
                    OwnedArray<RefTest> refTests;
                    for(const GPPTestObject* object : testObjects)
                    {
                        if(listener->isListening(*object))
                        {
                            refTests.add(new RefTest(*object, *this));
                        }
                    }
                    listeners.removeObject(listener);
                    for(const RefTest* test : refTests)
                    {
                        test->check(-1,
                                "Deleting listener didn't release reference!");
                    }
                });
        //set a random test object equal to another random test object
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    if(testObjects.size() < 2)
                    {
                        return;
                    }
                    int obj1 = randomIndex(testObjects);
                    int obj2 = randomIndex(testObjects);

                    GPPTestObject copy1(*testObjects[obj1]);

                    int expectedRef1 = testObjects[obj1]->getReferenceCount();
                    int expectedRef2 = testObjects[obj2]->getReferenceCount();

                    int expectedCopyRef1 = expectedRef1;
                    if(testObjects[obj1]->isNull() 
                            && testObjects[obj2]->isNull())
                    {
                        expectedRef1 = 0;
                        expectedRef2 = 0;
                        expectedCopyRef1 = 0;
                    }
                    else if(testObjects[obj1]->isNull())
                    {
                        expectedRef1 = expectedRef2 + 1;
                        expectedRef2 = expectedRef1;
                        expectedCopyRef1 = 0;
                    }
                    else if(testObjects[obj2]->isNull())
                    {
                        expectedRef1 = 0;
                        expectedRef2 = 0;
                        expectedCopyRef1--;
                    }
                    else if(*testObjects[obj1] == *testObjects[obj2])
                    {
                        static int n = 0;
                        n++;
                    }
                    else
                    {
                        expectedRef1 = expectedRef2 + 1;
                        expectedRef2 = expectedRef1;
                        expectedCopyRef1--;
                    }
                      
                    *testObjects[obj1] = *testObjects[obj2];
                    String assignmentText = 
                        String::toHexString((unsigned long) testObjects[obj1]);
                    if(testObjects[obj1]->isNull())
                    {
                        assignmentText += "(null)";
                    }
                    assignmentText += " = "; 
                    assignmentText += 
                        String::toHexString((unsigned long) testObjects[obj2]);
                    if(testObjects[obj2]->isNull())
                    {
                        assignmentText += "(null)";
                    }
                    expectEquals(testObjects[obj1]->getReferenceCount(),
                            expectedRef1,
                            assignmentText + 
                            ": First GPPObject ref count is wrong");
                    expectEquals( 
                            testObjects[obj2]->getReferenceCount(),
                            expectedRef2,
                            assignmentText + 
                            ": Second GPPObject ref count is wrong");
                    expectEquals(copy1.getReferenceCount(),
                            expectedCopyRef1,
                            assignmentText + 
                            ": Original first GObject ref count is wrong");
                });

        //set a random test object equal to null
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    int numObj = randomIndex(testObjects);
                    jassert(testObjects[numObj] != nullptr);
                    GPPTestObject temp(*testObjects[numObj]);
                    RefTest refTest(temp, *this);
                    *testObjects[numObj] = NULL;
                    refTest.check(-1,
                            "Ref count error after setting object to null.");
                });

        //update a random test object's string property
        addAction([this]()
                {
                    GPPTestObject tempObject = randomObject();
                    const ScopedLock objectLock(dataLock);
                    juce::String currentString = tempObject.getTestString();
                    currentString = currentString + "b";
                    tempObject.setTestString(currentString);
                });

        //update a random test object's int property
        addAction([this]()
                {
                    GPPTestObject tempObject = randomObject();
                    const ScopedLock objectLock(dataLock);
                    int currentInt = tempObject.getTestInt();
                    currentInt = currentInt + 1;
                    tempObject.setTestInt(currentInt);
                });
    }
    
    void runTest() override
    {
        using namespace juce;
        beginTest("object count test");
        expectEquals(gtest_object_count(), 0);
        GPPTestObject test1;
        GPPTestObject test2;
        
        String testString = "asdfasdf";
        test1.setTestString(testString);
        expectEquals(testString, test1.getTestString());
        test1.setTestInt(5);
        expectEquals(5, test1.getTestInt());
        
        expectEquals(gtest_object_count(), 2);
        expectEquals(test1.getReferenceCount(), 1);
        test2 = nullptr;
        expectEquals(gtest_object_count(), 1);
        test1 = nullptr;
        expectEquals(gtest_object_count(), 0);
        test1 = GPPTestObject();
        expectEquals(gtest_object_count(), 1);
        test2 = test1;
        expectEquals(gtest_object_count(), 1);
        expectEquals(test1.getReferenceCount(), 2);
        test1 = nullptr;
        expectEquals(gtest_object_count(), 1);
        test2 = nullptr;
        expectEquals(gtest_object_count(), 0);

        

        beginTest("threading stress test");
        testObjects.add(new GPPTestObject());
        listeners.add(new GPPTestObject::Listener());
        runThreads();
        testObjects.clear();
        listeners.clear();
        expectEquals(gtest_object_count(), 0,
                "GObjects not destroyed properly!");
    }
};

static GPPObjectTest test;
