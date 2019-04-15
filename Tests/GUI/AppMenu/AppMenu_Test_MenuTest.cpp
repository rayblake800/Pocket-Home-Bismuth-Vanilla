#include "AppMenu.h"
#include "AppMenu_Format.h"
#include "AppMenu_ConfigFile.h"
#include "JuceHeader.h"
#include "Icon_Loader.h"
#include "DesktopEntry_Loader.h"
#include "Testing_Window.h"
#include "Testing_DelayUtils.h"

// Test window bounds:
static const constexpr int winX = 5;
static const constexpr int winY = 5;
static const constexpr int winWidth = 480;
static const constexpr int winHeight = 272;

namespace AppMenu { namespace Test { class MenuTest; } }

/**
 * @brief  Tests AppMenu creation, format changes, and destruction.
 */
class AppMenu::Test::MenuTest : public juce::UnitTest
{
public:
    MenuTest() : juce::UnitTest("AppMenu::AppMenu Testing",
            "AppMenu") {}

    /**
     * @brief  Call within the runTest method to test changes to the menu
     *         format.
     */
    void formatChangeTest()
    {
        using juce::String;
        using namespace AppMenu;
        ConfigFile config;
        Format initial = config.getMenuFormat();
        logMessage(String("Initial menu format:")
                + config.formatToString(initial));

        expectNotEquals( (int) initial, (int) Format::Invalid,
                "Initial format was invalid!");

        config.setMenuFormat(Format::Invalid);
        expectEquals( (int) initial, (int) config.getMenuFormat(),
                "Trying to set an invalid format should be ignored");

        for (int i = 0; i < (int) Format::Invalid; i++)
        {
            config.setMenuFormat( (Format) i);
            expectEquals(i, (int) config.getMenuFormat(),
                    "Failed to change menu format!");
        }
        config.setMenuFormat(initial);
        expectEquals( (int) config.getMenuFormat(), (int) initial,
                "Failed to restore initial menu format!");
    }

    /**
     * @brief  Call within the runTest method to test changes to other menu
     *         settings.
     */
    void otherSettingsTest()
    {
        using juce::String;
        using namespace AppMenu;
        ConfigFile config;

        const constexpr int numSettings = 3;
        juce::StringArray settingNames =
        {
            "PagedMenuColumns",
            "PagedMenuRows",
            "ScrollingMenuRows"
        };
        juce::Array<std::function<int()>> getSettings =
        {
            [&config](){ return config.getPagedMenuColumns(); },
            [&config](){ return config.getPagedMenuRows(); },
            [&config](){ return config.getScrollingMenuRows(); }
        };
        juce::Array<std::function<void(int)>> setSettings =
        {
            [&config] (int val){ config.setPagedMenuColumns(val); },
            [&config] (int val){ config.setPagedMenuRows(val); },
            [&config] (int val){ config.setScrollingMenuRows(val); }
        };
        juce::Array<int> testValues = { -999, -1, 0, 1, 5, 999};
        for (int settingNum = 0; settingNum < numSettings; settingNum++)
        {
            int initial = getSettings[settingNum]();
            expectGreaterThan(initial, 0, String("Invalid initial value for ")
                    + settingNames[settingNum]);
            for (int tVal : testValues)
            {
                setSettings[settingNum] (tVal);
                if (tVal < 1)
                {
                    expectEquals(getSettings[settingNum](), 1,
                            String("Invalid value for ")
                            + settingNames[settingNum]
                            + String(" not rounded to 1"));
                }
                else
                {
                    expectEquals(getSettings[settingNum](), tVal,
                            String("Failed to set ")
                            + settingNames[settingNum]);
                }
            }
            setSettings[settingNum] (initial);
            expectEquals(getSettings[settingNum](), initial,
                    String("Failed to restore initial setting for ")
                    + settingNames[settingNum]);
        }
    }

    void runTest() override
    {
        using juce::String;
        using namespace AppMenu;
        //Create AppMenu::ConfigFile to avoid pointlessly recreating and
        //destroying JSON data whenever another ConfigFile instance is
        //destroyed.
        ConfigFile config;

        beginTest("Initial AppMenu format tests");
        // Test format strings:
        const int numFormats = 1 + (int) Format::Invalid;
        juce::StringArray formatStrings;
        for (int i = 0; i < numFormats; i++)
        {
            String formatString = config.formatToString( (Format) i);
            expect(formatString.isNotEmpty(),
                    String("Missing string for format number ") + String(i));
            if (i != (int) Format::Invalid)
            {
                expectNotEquals(formatString,
                        config.formatToString(Format::Invalid),
                        String("Format number ") + String(i)
                        + String(" treated as invalid!"));
            }
        }
        juce::StringArray invalidFormats = { "", "paged", " Scrolling", "lsk" };
        for (const String& invalidStr : invalidFormats)
        {
            expectEquals( (int) config.stringToFormat(invalidStr),
                    (int) Format::Invalid,
                    "Invalid format not handled correctly");
        }

        // Test format changes before creating the menu
        beginTest("Initial format change test");
        formatChangeTest();
        //Test other changes to menu settings
        beginTest("Initial test of other menu settings");
        otherSettingsTest();


        beginTest("AppMenu creation");
        juce::Component* appMenu = AppMenu::createAppMenu();
        Testing::Window menuWindow("AppMenu", appMenu, winX, winY, winWidth,
                winHeight);

        expect(Testing::DelayUtils::idleUntil([&menuWindow]()
                    {
                        return menuWindow.isShowing();
                    },
                    200, 2000), "AppMenu window was never shown!");
        expect(!appMenu->getBounds().isEmpty(), "AppMenu bounds are empty!");

        // Test format changes after creating the menu
        beginTest("Active menu format change test");
        formatChangeTest();
        //Test other changes to menu settings
        beginTest("Active menu test of other menu settings");
        otherSettingsTest();

        // Test menu destruction
        menuWindow.clearContentComponent();
        menuWindow.removeFromDesktop();

        // Test format changes after creating the menu
        beginTest("Destroyed menu format change test");
        formatChangeTest();
        //Test other changes to menu settings
        beginTest("Destroyed menu test of other menu settings");
        otherSettingsTest();
    }
};

static AppMenu::Test::MenuTest test;
