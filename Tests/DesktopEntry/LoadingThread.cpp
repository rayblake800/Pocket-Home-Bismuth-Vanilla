#define DESKTOP_ENTRY_IMPLEMENTATION
#include "JuceHeader.h"
#include "DesktopEntry_LoadingThread.h"

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
