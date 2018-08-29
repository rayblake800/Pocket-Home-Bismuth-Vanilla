#include "JuceHeader.h"
#include "gtest_object.h"
#include "../StressTest.h"


#include "GPPTestObject.h"

class GPPObjectTest : public StressTest
{
private:
    juce::OwnedArray<GPPTestObject> testObjects;
    juce::OwnedArray<GPPTestObject::Listener> listeners;
    juce::CriticalSection dataLock;

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
    GPPObjectTest() : StressTest("GTestObject testing", 10, 20, 10, 15) 
    {
        using namespace juce;
        //add a new test object
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    testObjects.add(new GPPTestObject());
                });
        //remove a random test object
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    if(testObjects.size() > 1)
                    {
                        testObjects.remove(randomIndex(testObjects));
                    }
                });
        /*
        //create a new listener, and add it to a random test object
        addAction([this]()
                {
                    GPPTestObject tempObject = randomObject();

                    const ScopedLock objectLock(dataLock);
                    GPPTestObject::Listener* listener =
                            new GPPTestObject::Listener();
                    tempObject.addListener(*listener);
                    listeners.add(listener);
                });

        //add a random listener to a random test object
        addAction([this]()
                {
                    GPPTestObject tempObject = randomObject();
                    const ScopedLock objectLock(dataLock);
                    GPPTestObject::Listener* listener = randomListener();
                    if(listener != nullptr)
                    {
                        tempObject.addListener(*listener);
                    }
                });


        //remove a random listener from a random test object
        addAction([this]()
                 {
                    GPPTestObject tempObject = randomObject();
                    
                    const ScopedLock objectLock(dataLock);
                    GPPTestObject::Listener* listener = randomListener();
                    if(listener != nullptr)
                    {
                        tempObject.removeListener(*listener);
                    }
                });

        //destroy a random listener
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    if(listeners.size() > 1)
                    {
                        listeners.remove(randomIndex(listeners));
                    }
                });
        */
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
                    
                    if(obj1 == obj2)
                    {
                        static int n = 0;
                        n++;
                        DBG("Self=self: "<<n);
                    }
                    if(testObjects[obj1]->isNull() 
                            && testObjects[obj2]->isNull())
                    {
                        static int n = 0;
                        n++;
                        DBG("null=null: "<<n);
                    }
                    else if(testObjects[obj1]->isNull())
                    {
                        static int n = 0;
                        n++;
                        DBG("null=value: "<<n);
                    }
                    else if(testObjects[obj2]->isNull())
                    {
                        static int n = 0;
                        n++;
                        DBG("value=null: "<<n);
                    }
                    else if(*testObjects[obj1] == *testObjects[obj2])
                    {
                        static int n = 0;
                        n++;
                        DBG("v1=v1: " << n);
                        return;
                    }
                    else
                    {
                        static int n = 0;
                        n++;
                        DBG("v1=v2: " << n);
                        return;
                    }
                      
                    *testObjects[obj1] = *testObjects[obj2];
                    
                });

        //set a random test object equal to null
        addAction([this]()
                {
                    const ScopedLock objectLock(dataLock);
                    int numObj = randomIndex(testObjects);
                    jassert(testObjects[numObj] != nullptr);
                    *testObjects[numObj] = NULL;
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
