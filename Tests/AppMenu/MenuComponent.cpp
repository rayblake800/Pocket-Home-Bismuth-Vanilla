#define APPMENU_IMPLEMENTATION
#include "JuceHeader.h"
#include "AppMenu_MenuComponent.h"

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
