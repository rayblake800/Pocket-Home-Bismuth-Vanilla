#include "JuceHeader.h"
#include "Config_FileResource.h"

class FileResourceTest : public juce::UnitTest
{
public:
    FileResourceTest() : juce::UnitTest("Config::FileResource Testing",
            "Config") {}
    
    void runTest() override
    {
    }
};

static FileResourceTest test;
