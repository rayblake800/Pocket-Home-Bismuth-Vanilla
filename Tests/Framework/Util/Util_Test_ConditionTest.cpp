#include "Util_ConditionCheck.h"
#include "JuceHeader.h"

namespace Util
{
    namespace Test
    {
        class ConditionTest;
    }
}

/**
 * @brief  Tests the ConditionCheck class.
 */
class Util::Test::ConditionTest : public juce::UnitTest
{
public:
    ConditionTest() : juce::UnitTest("ConditionCheck Testing",
            "Util") {}
    
    void runTest() override
    {
        ConditionCheck conditionTester;
        beginTest("Immediate condition success test");

        bool testPassed = false;
        // Set the check interval to an unreasonably large value to ensure it
        // will never finish:
        conditionTester.setCheckInterval(9999999);
        expect(conditionTester.startCheck( 
                    [](){return true;},
                    [&testPassed](){ testPassed = true; }, -1),
                "Condition check failed to start.");
        expect(!conditionTester.isChecking(),
                "Condition check failed to finish.");
        expect(testPassed, "Failed to run condition callback function.");
    }
};

static Util::Test::ConditionTest test;
