#include "JuceHeader.h"
#include "DesktopEntry/LoadingThread.h"

class LoadingThreadTest : public juce::UnitTest
{
public:
    LoadingThreadTest() : juce::UnitTest("DesktopEntry::LoadingThread Testing",
            "DesktopEntry") {}
    
    void runTest() override
    {
    }
};

static LoadingThreadTest test;
