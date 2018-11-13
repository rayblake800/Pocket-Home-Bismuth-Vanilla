#include "JuceHeader.h"
#include "Config/MainResource.h"

class MainResourceTest : public juce::UnitTest
{
public:
    MainResourceTest() : juce::UnitTest("Config::MainResource Testing",
            "Config") {}
    
    void runTest() override
    {
    }
};

static MainResourceTest test;
