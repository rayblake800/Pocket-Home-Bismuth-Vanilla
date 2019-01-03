#define DESKTOP_ENTRY_IMPLEMENTATION
#include "JuceHeader.h"
#include "DesktopEntry/FileUtils.h"

class FileUtilsTest : public juce::UnitTest
{
public:
    FileUtilsTest() : juce::UnitTest("DesktopEntry::FileUtils Testing",
            "DesktopEntry") {}
    
    void runTest() override
    {
    }
};

static FileUtilsTest test;
