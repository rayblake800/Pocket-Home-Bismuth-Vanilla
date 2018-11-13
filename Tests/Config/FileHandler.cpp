#include "JuceHeader.h"
#include "Config/FileHandler.h"

class FileHandlerTest : public juce::UnitTest
{
public:
    FileHandlerTest() : juce::UnitTest("Config::FileHandler Testing",
            "Config") {}
    
    void runTest() override
    {
    }
};

static FileHandlerTest test;
