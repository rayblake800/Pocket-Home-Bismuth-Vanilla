#include "JuceHeader.h"
#include "DesktopEntry/EntryFile.h"

class EntryFileTest : public juce::UnitTest
{
public:
    EntryFileTest() : juce::UnitTest("DesktopEntry::EntryFile Testing",
            "DesktopEntry") {}
    
    void runTest() override
    {
    }
};

static EntryFileTest test;
