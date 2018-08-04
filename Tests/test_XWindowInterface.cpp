#include "JuceHeader.h"
#include "WindowFocus.h"
#include "XWindowInterface.h"

class XWindowInterfaceTest : public juce::UnitTest
{
public:
    XWindowInterfaceTest() : juce::UnitTest("XWindowInterface testing") {}
    
    void runTest() override
    {
        using namespace juce;
        /* Test command summary:
         * beginTest("testName");
         * expect(condition);
         * expectEquals(item1, item2);
         *
         * see deps/JUCE/modules/juce_core/unit_tests
         */      
    }
};

static XWindowInterfaceTest test;