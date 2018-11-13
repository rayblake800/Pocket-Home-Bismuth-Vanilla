#include "JuceHeader.h"

class PagedMenuTest : public juce::UnitTest
{
public:
    PagedMenuTest() : juce::UnitTest("AppMenu::PagedMenu Testing",
            "AppMenu") {}
    
    void runTest() override
    {
    }
};

static PagedMenuTest test;
