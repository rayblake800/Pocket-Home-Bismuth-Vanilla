#include "JuceHeader.h"
#include "AppMenu/Control/InputHandler.h"

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
