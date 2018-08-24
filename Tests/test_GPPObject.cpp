#include "JuceHeader.h"
#include "GPPObject.h"

class GPPObjectTest : public juce::UnitTest
{
public:
    GPPObjectTest() : juce::UnitTest("GPPObject testing") {}
    
    void runTest() override
    {
        GWeakRef objectRef;
    }
};

static GPPObjectTest test;
