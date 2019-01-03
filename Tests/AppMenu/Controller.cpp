#define APPMENU_IMPLEMENTATION
#include "JuceHeader.h"
#include "AppMenu_Controller.h"

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
