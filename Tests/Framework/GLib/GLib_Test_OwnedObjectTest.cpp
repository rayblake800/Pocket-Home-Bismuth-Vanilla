/**
 * @file  GLib_Test_OwnedObjectTest.cpp
 *
 * @brief  Tests the GLib::Owned::Object class.
 */

#include "JuceHeader.h"
#include "gtest_object.h"
#include "Testing_StressTest.h"
#include "GLib_Test_OwnedObject.h"

static const constexpr int minThreadCount = 30;
static const constexpr int maxThreadCount = 50;
static const constexpr int threadSleepMS = 300;
static const constexpr int testSeconds = 10;

namespace GLib { namespace Test { class OwnedObjectTest; } }

/**
 * @brief  Tests the GLib::Owned::Object class, checking that reference counts
 *         and signal handlers behave correctly even when used heavily by
 *         multiple threads.
 */
class GLib::Test::OwnedObjectTest : public Testing::StressTest
{
private:
    //Holds all GPPObjects created during the stress test
    juce::OwnedArray<OwnedObject> testObjects;
    //Holds all object listeners created during the stress test
    juce::OwnedArray<OwnedObject::Listener> listeners;
    //OwnedObjects and OwnedArrays are not threadsafe, prevent concurrent
    //access.
    juce::CriticalSection dataLock;

    /**
     * @brief  Provides a less verbose way to test reference count changes.
     */
    class RefTest
    {
    public:
        /**
         * @brief  Initializes a GObject reference change test.
         *
         * @param testObject  The object with the reference count being tested.
         *
         * @param test        The unit test that created this RefTest.
         */
        RefTest(const OwnedObject& testObject, OwnedObjectTest& test) :
            testObject(testObject),
            test(test),
            initialRefCount(testObject.getReferenceCount()),
            nullObject(testObject.isNull()) { }

        virtual ~RefTest() { }

        /**
         * @brief  Checks if the OwnedObject's references changed as expected.
         *
         * @param changeExpected  The expected difference between the current
         *                        reference count and the original count. This
         *                        is rounded up to zero if it is negative and
         *                        the original reference count is zero.
         *
         * @return                Whether the actual change matched the expected
         *                        change in reference count.
         */
        bool check(int changeExpected) const
        {
            int expectedCount = initialRefCount + changeExpected;
            if (expectedCount < 0)
            {
                expectedCount = 0;
            }
            return testObject.getReferenceCount() == expectedCount;
        }

    private:
        // The GLib::Owned::Object checked by this test object:
        const OwnedObject& testObject;
        // The UnitTest class that owns this test object:
        OwnedObjectTest& test;
        // Whether the testObject was initially null:
        const bool nullObject;
        // The testObject's initial GObject reference count.
        const int initialRefCount;
    };

    /**
     * @brief  Gets a random valid index for any juce::Array type.
     *
     * @tparam ArrayType  The type of whatever Array class the function needs to
     *                    access.
     *
     * @param array       Any juce::Array object.
     *
     * @return            A random index value between zero and the maximum
     *                    index used in the array.
     */
    template<class ArrayType>
    int randomIndex(ArrayType& array)
    {
        int index = -1;
        return abs(juce::Random::getSystemRandom().nextInt())
                % testObjects.size();
        return index;
    }

    /**
     * @brief  Selects a random OwnedObject from the list of tested objects.
     *
     * @return  A random saved object, or a null object if no OwnedObject has
     *          been initialized.
     */
    OwnedObject randomObject()
    {
        if (testObjects.isEmpty())
        {
            return OwnedObject();
        }
        int index = randomIndex(testObjects);
        return OwnedObject(*testObjects[index]);
    }

    /**
     * @brief  Checks if any non-null listener objects are stored by the test.
     *
     * @return  Whether the listener list is currently empty, or only holds
     *          null values.
     */
    bool listenersEmpty()
    {
        if (listeners.isEmpty())
        {
            return true;
        }
        for (OwnedObject::Listener* l : listeners)
        {
            if (l != nullptr)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief  Gets a random non-null listener object from the list of tested
     *         OwnedObject listeners.
     *
     * @return  The random listener, or nullptr if the listener list is empty.
     */
    OwnedObject::Listener* randomListener()
    {
        if (listenersEmpty())
        {
            return nullptr;
        }
        int index = -1;
        while (index < 0 || listeners[index] == nullptr)
        {
            index = randomIndex(listeners);
        }
        return listeners[index];
    }

public:
    OwnedObjectTest() : StressTest("GLib::Owned::Object testing", "GLib",
            minThreadCount, maxThreadCount, threadSleepMS, testSeconds)
    {
        using juce::ScopedLock;
        // Initialize random stress test actions:

        // Add a new test object.
        addAction(Testing::Action("Creating a new test Object",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            testObjects.add(new OwnedObject());

            return testObjects.getLast()->getReferenceCount() == 1;
        }));

        // Remove a random test object.
        addAction(Testing::Action("Removing a random test object",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            if (testObjects.size() > 1)
            {
                int index = randomIndex(testObjects);
                OwnedObject temp(*testObjects[index]);
                RefTest refTest(temp, *this);
                testObjects.remove(index);
                return refTest.check(-1);
            }
            return true;
        },
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            return testObjects.size() > 1;
        }));

        // Create a new listener, and add it to a random test object.
        addAction(Testing::Action(
        "Adding a new signal handler to a random Object",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            OwnedObject tempObject = randomObject();
            OwnedObject::Listener* listener =
                    new OwnedObject::Listener();
            RefTest refTest(tempObject, *this);
            tempObject.addListener(*listener);
            listeners.add(listener);
            return refTest.check( (tempObject.isNull() ? 0 : 2));
        }));

        // Add a random listener to a random test object.
        addAction(Testing::Action(
        "Adding a random signal handler to a random Object",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            OwnedObject tempObject = randomObject();
            OwnedObject::Listener* listener = randomListener();
            if (listener != nullptr && !listener->isConnected(tempObject))
            {
                RefTest refTest(tempObject, *this);
                tempObject.addListener(*listener);
                return refTest.check( (tempObject.isNull() ? 0 : 2));
            }
            return true;
        }));


        // Remove a random listener from a random test object.
        addAction(Testing::Action(
        "Removing a random signal handler from a random test Object",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            OwnedObject tempObject = randomObject();
            OwnedObject::Listener* listener = randomListener();
            if (listener != nullptr && listener->isConnected(tempObject))
            {
                RefTest refTest(tempObject, *this);
                if (!tempObject.removeListener(*listener))
                {
                    return false;
                }
                return refTest.check(-2);
            }
            return true;
        }));

        // Destroy a random listener.
        addAction(Testing::Action("Destroying a random signal handler",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            OwnedObject::Listener* listener = randomListener();
            if (listeners.size() < 2 || listener == nullptr)
            {
                return true;
            }
            juce::OwnedArray<RefTest> refTests;
            for (OwnedObject* object : testObjects)
            {
                if (listener->isConnected(*object))
                {
                    refTests.add(new RefTest(*object, *this));
                }
            }
            listeners.removeObject(listener);
            for (const RefTest* test : refTests)
            {
                return test->check(-2);
            }
            return true;
        }));

        // Set a random test object equal to another random test object.
        addAction(Testing::Action(
        "Setting one random test Object equal to another",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            if (testObjects.size() < 2)
            {
                return true;
            }
            int obj1 = randomIndex(testObjects);
            int obj2 = randomIndex(testObjects);

            OwnedObject copy1(*testObjects[obj1]);

            int expectedRef1 = testObjects[obj1]->getReferenceCount();
            int expectedRef2 = testObjects[obj2]->getReferenceCount();

            int expectedCopyRef1 = expectedRef1;
            if (testObjects[obj1]->isNull()
                    && testObjects[obj2]->isNull())
            {
                expectedRef1 = 0;
                expectedRef2 = 0;
                expectedCopyRef1 = 0;
            }
            else if (testObjects[obj1]->isNull())
            {
                expectedRef1 = expectedRef2 + 1;
                expectedRef2 = expectedRef1;
                expectedCopyRef1 = 0;
            }
            else if (testObjects[obj2]->isNull())
            {
                expectedRef1 = 0;
                expectedRef2 = 0;
                expectedCopyRef1 -= 1;
            }
            else if (*testObjects[obj1] == *testObjects[obj2])
            {
                static int n = 0;
                n++;
            }
            else
            {
                expectedRef1 = expectedRef2 + 1;
                expectedRef2 = expectedRef1;
                expectedCopyRef1 -= 1;
            }

             *testObjects[obj1] = *testObjects[obj2];
            juce::String assignmentText = juce::String::toHexString
                    ( (unsigned long) testObjects[obj1]);
            if (testObjects[obj1]->isNull())
            {
                assignmentText += " (null)";
            }
            assignmentText += " = ";
            assignmentText += juce::String::toHexString
                    ( (unsigned long) testObjects[obj2]);
            if (testObjects[obj2]->isNull())
            {
                assignmentText += " (null)";
            }

            if (testObjects[obj1]->getReferenceCount() != expectedRef1)
            {
                logMessage(assignmentText +
                        ": First GLib::Object ref count is wrong");
                return false;
            }
            if (testObjects[obj2]->getReferenceCount() != expectedRef2)
            {
                logMessage(assignmentText +
                        ": Second GLib::Object ref count is wrong");
                return false;
            }
            if (copy1.getReferenceCount() != expectedCopyRef1)
            {
                logMessage(assignmentText +
                        ": Original first GObject ref count is wrong");
                return false;
            }
            return true;
        }));

        // Set a random test object equal to null.
        addAction(Testing::Action("Setting a random test Object to null",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            int numObj = randomIndex(testObjects);
            jassert(testObjects[numObj] != nullptr);
            OwnedObject temp(*testObjects[numObj]);
            RefTest refTest(temp, *this);
             *testObjects[numObj] = NULL;
            return refTest.check(-1);
        }));

        // Update a random test object's string property.
        addAction(Testing::Action(
        "Updating a random test Object's string property",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            OwnedObject tempObject = randomObject();
            if (tempObject.isNull())
            {
                return true;
            }
            juce::String currentString = tempObject.getTestString();
            currentString = currentString + "b";
            tempObject.setTestString(currentString);
            return currentString == tempObject.getTestString();
        }));

        // Update a random test object's int property.
        addAction(Testing::Action(
        "Updating a random test Object's int property",
        [this]()
        {
            const ScopedLock objectLock(dataLock);
            OwnedObject tempObject = randomObject();
            if (tempObject.isNull())
            {
                return true;
            }
            int currentInt = tempObject.getTestInt();
            currentInt = currentInt + 1;
            tempObject.setTestInt(currentInt);
            return tempObject.getTestInt() == currentInt;
        }));
    }

    /**
     * @brief  Runs all Owned::Object tests.
     */
    void runTest() override
    {
        using namespace juce;
        beginTest("object count test");
        expectEquals(gtest_object_count(), 0);
        OwnedObject test1;
        OwnedObject test2;

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
        test1 = OwnedObject();
        expectEquals(gtest_object_count(), 1);
        test2 = test1;
        expectEquals(gtest_object_count(), 1);
        expectEquals(test1.getReferenceCount(), 2);
        test1 = nullptr;
        expectEquals(gtest_object_count(), 1);
        test2 = nullptr;
        expectEquals(gtest_object_count(), 0);

        beginTest("Threading stress test");
        testObjects.add(new OwnedObject());
        listeners.add(new OwnedObject::Listener());
        runThreads();
        testObjects.clear();
        listeners.clear();
        expectEquals(gtest_object_count(), 0,
                "GObjects not destroyed properly!");
    }
};

static GLib::Test::OwnedObjectTest test;
