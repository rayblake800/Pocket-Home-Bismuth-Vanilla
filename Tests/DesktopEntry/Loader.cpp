#define DESKTOP_ENTRY_IMPLEMENTATION
#include "JuceHeader.h"
#include "DesktopEntry/Loader.h"

class LoaderTest : public juce::UnitTest
{
public:
    LoaderTest() : juce::UnitTest("DesktopEntry::Loader Testing",
            "DesktopEntry") {}
    
    void runTest() override
    {
    }
};

static LoaderTest test;
