#include "Audio.h"
#include "PocketHomeApplication.h"
#include "XWindowInterface.h"
#include "TempTimer.h"
#include "PocketHomeWindow.h"

/* Milliseconds to wait between window focus attempts: */
static const constexpr int focusWaitMs = 200;

/* Sets if tests should run after the window initializes. */
static bool runTests = false;
static bool verboseTesting = false;
/* Specific test categories to run: */
static juce::StringArray testCategories;

/**
 * Attempts to activate the application window and grab keyboard focus.  This
 * may fail if the window isn't fully initialized, so it will repeatedly call
 * itself on a timer until the action succeeds.
 */
static void focusAppWindow()
{
    using namespace juce;
    XWindowInterface xWindows;
    Window appWindow = xWindows.getPocketHomeWindow();
    xWindows.activateWindow(appWindow);
    Component * rootComponent = Desktop::getInstance().getComponent(0);
    rootComponent->grabKeyboardFocus();
    if(runTests && xWindows.isActiveWindow(appWindow))
    {
        UnitTestRunner tester;
        tester.setPassesAreLogged(verboseTesting);
        if(testCategories.isEmpty())
        {
            DBG("Running all pocket-home tests:");
            tester.runAllTests();
        }
        else
        {
            for(const juce::String& category : testCategories)
            {
                DBG("Running test category: " << category);
                tester.runTestsInCategory(category);
            }
        }
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
    else if(!WindowFocus::isFocused())
    {
        TempTimer::initTimer(focusWaitMs, focusAppWindow);
    }
    
}

/**
 * This is called by the Juce framework to initialize the program.
 * This will initialize audio and create the program window.
 */
void PocketHomeApplication::initialise(const juce::String &commandLine)
{
    using namespace juce;
    using std::cerr;
    StringArray args;
    args.addTokens(commandLine, true);

    if (args.contains("--help"))
    {
        cerr << "arguments:" << std::endl;
        cerr << "  --help	         Print usage help\n";
        cerr << "  --fakeWifi	     Use fake WifiStatus\n";
        cerr << "  --test 	         Run all program tests\n";
        cerr << "     -categories    Run tests within listed categories\n";
        cerr << "     -v 	         Verbose test output\n";
        quit();
    }


    lookAndFeel = new PokeLookAndFeel();
    LookAndFeel::setDefaultLookAndFeel(lookAndFeel);

    // open sound handle
    if (!Audio::chipAudioInit())
    {
        DBG("PocketHomeApplication::" << __func__
                << ": Sound failed to initialize");
    }
    runTests = args.contains("--test");
    if(!runTests)
    {
        gLibThread = new GLibSignalThread();
        homeWindow = new PocketHomeWindow
                (getApplicationName(), args.contains("--fakeWifi"));
    }
    else
    {
        verboseTesting = args.contains("-v");
        int categoryIndex = args.indexOf("-categories");
        if(categoryIndex != -1)
        {
            for(int i = categoryIndex + 1; i < args.size(); i++)
            {
                testCategories.add(args[i]);
            }
        }
        //Use an empty window and don't initialize signal thread when testing
        homeWindow = new DocumentWindow(getApplicationName(), Colours::dimgrey,
                DocumentWindow::allButtons);
        homeWindow->setBounds(0,0,50,50);
        homeWindow->setLookAndFeel(&LookAndFeel::getDefaultLookAndFeel());
        homeWindow->setUsingNativeTitleBar(true);
        homeWindow->setResizable(true, false);
        homeWindow->setVisible(true);
        homeWindow->addToDesktop();
    }           
    focusAppWindow();
}

/*
 * This is called by the Juce framework to safely shut down the application.
 */
void PocketHomeApplication::shutdown()
{
    using namespace juce;
    homeWindow = nullptr;
    LookAndFeel::setDefaultLookAndFeel(nullptr);
    lookAndFeel = nullptr;
}
