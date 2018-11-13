#include "JuceHeader.h"

class ScrollingMenuTest : public juce::UnitTest
{
public:
    ScrollingMenuTest() : juce::UnitTest("AppMenu::ScrollingMenu Testing",
            "AppMenu") {}
    
    void runTest() override
    {
    }
};

static ScrollingMenuTest test;
