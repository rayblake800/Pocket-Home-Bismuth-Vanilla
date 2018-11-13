#include "JuceHeader.h"
#include "AppMenu/Components/MenuComponent.h"

class MenuComponentTest : public juce::UnitTest
{
public:
    MenuComponentTest() : juce::UnitTest("AppMenu::MenuComponent Testing",
            "AppMenu") {}
    
    void runTest() override
    {
    }
};

static MenuComponentTest test;
