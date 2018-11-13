#include "JuceHeader.h"
#include "AppMenu/AppMenu.h"

class AppMenuTest : public juce::UnitTest
{
public:
    AppMenuTest() : juce::UnitTest("AppMenu::AppMenu Testing",
            "AppMenu") {}
    
    void runTest() override
    {
    }
};

static AppMenuTest test;
