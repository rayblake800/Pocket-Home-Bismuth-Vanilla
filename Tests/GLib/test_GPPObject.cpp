#include "JuceHeader.h"
#include "gtest_object.h"
#include "GPPTestObject.h"

class GPPObjectTest : public juce::UnitTest
{
public:
    GPPObjectTest() : juce::UnitTest("GTestObject testing") {}
    
    void runTest() override
    {
        using namespace juce;
        beginTest("object count test");
        expectEquals(gtestInstances(), 0);
        GPPTestObject test1;
        GPPTestObject test2;
        GPPTestObject test3;
        expectEquals(gtestInstances(), 3);
        test3 = nullptr;
        expectEquals(gtestInstances(), 2);
        test2 = nullptr;
        expectEquals(gtestInstances(), 1);
        test1 = nullptr;
        expectEquals(gtestInstances(), 0);
        test1 = GPPTestObject();
        expectEquals(gtestInstances(), 1);
        test2 = test1;
        expectEquals(gtestInstances(), 1);
        test1 = nullptr;
        expectEquals(gtestInstances(), 1);
        test2 = nullptr;
        expectEquals(gtestInstances(), 0);
    }
};

static GPPObjectTest test;
