#include "PocketHomeApplication.h"
#include "PocketHomeWindow.h"
#include "Windows_XInterface.h"
#include "Util_ShutdownListener.h"
#include "Util_TempTimer.h"

#ifdef CHIP_FEATURES
#include "Hardware_Audio.h"
#endif

#ifdef INCLUDE_TESTING
#include "Debug_ScopeTimerRecords.h"
#endif

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "PocketHomeApplication::";
#endif

// Milliseconds to wait between window focus attempts:
static const constexpr int focusWaitMs = 100;
// Each attempt to set and check window focus status should wait a little longer
// before the next test. Wait time multiplier:
static const constexpr float focusWaitMultiplier = 1.3;
// Milliseconds to wait before abandoning window focus attempts:
static const constexpr int focusTimeout = 20000;

#ifdef INCLUDE_TESTING
// Sets if tests should run after the window is created and focused.
static bool runTests = false;

// Sets if tests should print all output.
static bool verboseTesting = false;

// Specific test categories to run:
static juce::StringArray testCategories;
#endif


// Performs all required initialization when the application is first launched.
void PocketHomeApplication::initialise(const juce::String &commandLine)
{
    juce::StringArray args;
    args.addTokens(commandLine, true);

    if (args.contains("--help"))
    {
        using std::cerr;
        cerr << "arguments:" << std::endl;
        cerr << "  --help           Print this help text\n";
        #ifdef INCLUDE_TESTING
        cerr << "  --test           Run program tests\n";
        cerr << "     -categories   Run only tests within listed categories\n";
        cerr << "     -v            Verbose test output\n";
        #endif
        quit();
        return;
    }

    #ifdef CHIP_FEATURES
    if (!Hardware::Audio::chipAudioInit())
    {
        DBG("PocketHomeApplication::" << __func__
                << ": PocketC.H.I.P audio setup failed");
    }
    #endif

    lookAndFeel.reset(new Theme::LookAndFeel);
    juce::LookAndFeel::setDefaultLookAndFeel(lookAndFeel.get());

    #ifdef INCLUDE_TESTING
    runTests = args.contains("--test");
    if (runTests)
    {
        verboseTesting = args.contains("-v");
        int categoryIndex = args.indexOf("-categories");
        if (categoryIndex != -1)
        {
            for (int i = categoryIndex + 1; i < args.size(); i++)
            {
                testCategories.add(args[i]);
            }
        }

        // Use an empty window when testing.
        homeWindow.reset(new juce::DocumentWindow(getApplicationName(),
                juce::Colours::dimgrey, juce::DocumentWindow::allButtons));
        homeWindow->setBounds(0, 0, 50, 50);
        homeWindow->setLookAndFeel(
                &juce::LookAndFeel::getDefaultLookAndFeel());
        homeWindow->setUsingNativeTitleBar(true);
        homeWindow->setResizable(true, false);
        homeWindow->setVisible(true);
        homeWindow->addToDesktop();
    }
    else
    {
        homeWindow.reset(new PocketHomeWindow(getApplicationName()));
    }
    #else
    homeWindow.reset(new PocketHomeWindow(getApplicationName()));
    #endif

    focusChecker.setCheckInterval(focusWaitMs, focusWaitMultiplier);
    const auto onFocus = [this]()
    {
        DBG(dbgPrefix << __func__
                << ": Main window focused, enabling focus tracking:");
        static_cast<Windows::MainWindow*> (homeWindow.get())
                ->startFocusTracking();
        #ifdef INCLUDE_TESTING
        if (runTests)
        {
            runApplicationTests();
        }
        #endif
    };
    #if defined(JUCE_DEBUG) && defined(INCLUDE_TESTING)
    focusChecker.startCheck([this]() { return focusAppWindow(); },
            onFocus, focusTimeout,
            []()
            {
                DBG(dbgPrefix << __func__
                        << ": Window focus attempts timed out!");
            });
    #else
    focusChecker.startCheck([this]() { return focusAppWindow(); },
            onFocus, focusTimeout);
    #endif
}


// Performs all necessary cleanup steps before the application can be safely
// closed.
void PocketHomeApplication::shutdown()
{
    DBG(dbgPrefix << __func__ << ": Closing application resources.");
    Util::ShutdownBroadcaster::broadcastShutdown();
    homeWindow.reset(nullptr);
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    lookAndFeel.reset(nullptr);
    #ifdef INCLUDE_TESTING
    Debug::ScopeTimerRecords::printRecords();
    #endif
}


// Gets the name of this application.
const juce::String PocketHomeApplication::getApplicationName()
{
    return ProjectInfo::projectName;
}


// Gets the application's version string.
const juce::String PocketHomeApplication::getApplicationVersion()
{
    return ProjectInfo::versionString;
}


// Checks if multiple versions of this application may run simultaneously.
bool PocketHomeApplication::moreThanOneInstanceAllowed()
{
    return false;
}

#ifdef INCLUDE_TESTING
// Runs application tests and shuts down the application.
void PocketHomeApplication::runApplicationTests()
{
    juce::UnitTestRunner tester;
    tester.setPassesAreLogged(verboseTesting);
    if (testCategories.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": Running all pocket-home tests.");
        tester.runAllTests();
    }
    else
    {
        for (const juce::String& category : testCategories)
        {
            DBG(dbgPrefix << __func__ << ": Running test category: "
                    << category);
            tester.runTestsInCategory(category);
        }
    }
    DBG(dbgPrefix << __func__ << ": Finished running application tests.");
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
#endif

// Attempts to activate the application window and grab keyboard focus.
bool PocketHomeApplication::focusAppWindow()
{
    if (homeWindow == nullptr)
    {
        return false;
    }
    Windows::XInterface xWindows;
    Window appWindow = xWindows.getMainAppWindow();
    if (appWindow == BadWindow)
    {
        return false;
    }
    xWindows.activateWindow(appWindow);
    homeWindow->grabKeyboardFocus();
    return xWindows.isActiveWindow(appWindow)
            && homeWindow->hasKeyboardFocus(true);
}
