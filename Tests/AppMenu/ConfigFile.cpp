#define APPMENU_IMPLEMENTATION
#include "AppMenu_MenuFile.h"
#include "AppMenu_MenuItem.h"
#include "JuceHeader.h"

class AppMenuTest : public juce::UnitTest
{
public:
    AppMenuTest() : juce::UnitTest("AppMenu::MenuFile testing") {}
    
    void runTest() override
    {
        using namespace AppMenu;
        beginTest("AppMenu::MenuFile test");
        AppMenu::MenuFile appConfig;
        MenuItem rootItem = appConfig.getRootFolderItem();
        expect(!rootItem.isNull(), "Root folder item was null!");
        expect(rootItem.isFolder(), "Root item was not a folder!");
        expectGreaterThan(rootItem.getFolderSize(), 0,
                "Root folder item was empty!");
    }
};

static AppMenuTest test;
