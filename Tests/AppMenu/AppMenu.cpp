#include "JuceHeader.h"
#include "Icon/IconLoader.h"
#include "DesktopEntry/Loader.h"
#include "TestWindow.h"
#include "DelayUtils.h"
#include "AppMenu/AppMenu.h"

/* Test window bounds: */
static const constexpr int winX = 5;
static const constexpr int winY = 5;
static const constexpr int winWidth = 480;
static const constexpr int winHeight = 272;

class AppMenuTest : public juce::UnitTest
{
public:
    AppMenuTest() : juce::UnitTest("AppMenu::AppMenu Testing",
            "AppMenu") {}

    /**
     * @brief  Call within the runTest method to test changes to the menu
     *         format.
     */
    void formatChangeTest()
    {
        using juce::String;
        using namespace AppMenu;
        using namespace Settings;
        Format initial = getMenuFormat();
        logMessage(String("Initial menu format:") 
                + formatToString(initial));

        expectNotEquals((int) initial,(int) Format::Invalid,
                "Initial format was invalid!");

        setMenuFormat(Format::Invalid);
        expectEquals((int) initial,(int) getMenuFormat(), 
                "Trying to set an invalid format should be ignored");

        for(int i = 0; i < (int) Format::Invalid; i++)
        {
            setMenuFormat((Format) i);
            expectEquals(i, (int) getMenuFormat(),
                    "Failed to change menu format!");
        }
        setMenuFormat(initial);
        expectEquals((int) getMenuFormat(),(int) initial,
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
        using namespace Settings;

        const constexpr int numSettings = 3;
        juce::StringArray settingNames =
        {
            "PagedMenuColumns",
            "PagedMenuRows",
            "ScrollingMenuRows"
        };
        juce::Array<std::function<int()>> getSettings =
        {
            [](){ return getPagedMenuColumns(); },
            [](){ return getPagedMenuRows(); },
            [](){ return getScrollingMenuRows(); }
        };
        juce::Array<std::function<void(int)>> setSettings =
        {
            [](int val){ setPagedMenuColumns(val); },
            [](int val){ setPagedMenuRows(val); },
            [](int val){ setScrollingMenuRows(val); }
        };
        juce::Array<int> testValues = { -999, -1, 0, 1, 5, 999};
        for(int settingNum = 0; settingNum < numSettings; settingNum++)
        {
            int initial = getSettings[settingNum]();
            expectGreaterThan(initial, 0, String("Invalid initial value for ")
                    + settingNames[settingNum]);
            for(int tVal : testValues)
            {
                setSettings[settingNum](tVal);
                if(tVal < 1)
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
            setSettings[settingNum](initial);
            expectEquals(getSettings[settingNum](), initial,
                    String("Failed to restore initial setting for ")
                    + settingNames[settingNum]);
        }
    }
    
    void runTest() override
    {
        using juce::String;
        using namespace AppMenu;
        using namespace Settings;

        //Create resources that must be initialized before AppMenu can run:
        //TODO: AppMenu should be able to initialize these on its own if needed.
        //DesktopEntry::Loader entryLoader;
        //IconLoader iconLoader;
        
        beginTest("Initial AppMenu format tests");
        // Test format strings:
        const int numFormats = 1 + (int) Format::Invalid;
        juce::StringArray formatStrings;
        for(int i = 0; i < numFormats; i++)
        {
            String formatString = formatToString ((Format) i);
            expect(formatString.isNotEmpty(),
                    String("Missing string for format number ") + String(i));
            if(i != (int) Format::Invalid)
            {
                expectNotEquals(formatString, formatToString(Format::Invalid),
                        String("Format number ") + String(i)
                        + String(" treated as invalid!"));
            }
        }
        juce::StringArray invalidFormats = { "", "paged", " Scrolling", "lsk" };
        for(const String& invalidStr : invalidFormats)
        {
            expectEquals((int) stringToFormat(invalidStr),(int) Format::Invalid,
                    "Invalid format not handled correctly");
        }
       
        // TODO: interaction with settings before the menu is created causes
        //       problems with SharedResource deadlocks.  Fix bugs in 
        //       SharedResource destruction.
        // Test format changes before creating the menu
        //beginTest("Initial format change test");
        //formatChangeTest();
        //Test other changes to menu settings
        //beginTest("Initial test of other menu settings");
        //otherSettingsTest();


        beginTest("AppMenu creation");
        juce::Component* appMenu = AppMenu::createAppMenu();
        TestWindow menuWindow("AppMenu", appMenu, winX, winY, winWidth,
                winHeight);

        expect(DelayUtils::idleUntil([&menuWindow]()
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

        // Test format changes after creating the menu
        beginTest("Destroyed menu format change test");
        formatChangeTest();
        //Test other changes to menu settings
        beginTest("Destroyed menu test of other menu settings");
        otherSettingsTest();
    }
};

static AppMenuTest test;
