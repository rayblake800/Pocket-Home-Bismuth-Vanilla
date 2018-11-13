#include "JuceHeader.h"
#include "Config/MainFile.h"

class MainFileTest : public juce::UnitTest
{
public:
    MainFileTest() : juce::UnitTest("Config::MainFile Testing",
            "Config") {}
    
    void runTest() override
    {
    }
};

static MainFileTest test;
