#define APPMENU_IMPLEMENTATION
#include "AppMenu_MenuFile.h"
#include "AppMenu_MenuItem.h"
#include "JuceHeader.h"

namespace AppMenu { namespace Test { class MenuFileTest; } }

/**
 * @brief  Tests that the AppMenu successfully loads the root menu folder from
 *         the AppMenu configuration file.
 */
class AppMenu::Test::MenuFileTest : public juce::UnitTest
{
public:
    MenuFileTest() : juce::UnitTest("AppMenu::MenuFile testing") {}

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

static AppMenu::Test::MenuFileTest test;
