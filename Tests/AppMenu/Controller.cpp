#include "JuceHeader.h"
#include "AppMenu/Control/Controller.h"

class ControllerTest : public juce::UnitTest
{
public:
    ControllerTest() : juce::UnitTest("AppMenu::Controller Testing",
            "AppMenu") {}
    
    void runTest() override
    {
    }
};

static ControllerTest test;
