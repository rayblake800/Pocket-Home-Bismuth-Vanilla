#define APPMENU_IMPLEMENTATION
#include "JuceHeader.h"
#include "AppMenu_InputHandler.h"

class InputHandlerTest : public juce::UnitTest
{
public:
    InputHandlerTest() : juce::UnitTest("AppMenu::InputHandler Testing",
            "AppMenu") {}
    
    void runTest() override
    {
    }
};

static InputHandlerTest test;
