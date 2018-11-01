#define APPMENU_IMPLEMENTATION_ONLY
#include "JuceHeader.h"
#include "AppMenu/Data/ConfigFile.h"

class AppMenuTest : public juce::UnitTest
{
public:
    AppMenuTest() : juce::UnitTest("AppMenu testing") {}
    
    void runTest() override
    {
        using namespace AppMenu;
        beginTest("AppConfigFile test");
        AppMenu::ConfigFile appConfig;
        MenuItem rootItem = appConfig.getRootFolderItem();
        expect(!rootItem.isNull(), "Root folder item was null!");
        expect(rootItem.isFolder(), "Root item was not a folder!");
        expectGreaterThan(rootItem.getFolderSize(), 0,
                "Root folder item was empty!");
    }
};

static AppMenuTest test;
