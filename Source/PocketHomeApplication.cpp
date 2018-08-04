#include "Audio.h"
#include "PocketHomeApplication.h"
#include "XWindowInterface.h"
#include "TempTimer.h"

//Milliseconds to wait between window focus attempts.
static const constexpr int focusWaitMs = 200;

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
    if(!WindowFocus::isFocused())
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
    StringArray args;
    args.addTokens(commandLine, true);

    if (args.contains("--help"))
    {
        std::cerr << "arguments:" << std::endl;
        std::cerr << "  --help:	Print usage help" << std::endl;
        std::cerr << "  --fakeWifi:	Use fake WifiStatus"   << std::endl;
        std::cerr << "  --test: 	Run all program tests" << std::endl;
        std::cerr << "      -v: 	Verbose test output"   << std::endl;
        quit();
    }


    lookAndFeel = new PokeLookAndFeel();
    LookAndFeel::setDefaultLookAndFeel(lookAndFeel);

    // open sound handle
    if (!Audio::initAudio())
    {
        DBG("PocketHomeApplication::" << __func__
                << ": Sound failed to initialize");
    }
    homeWindow = new PocketHomeWindow
            (getApplicationName(), args.contains("--fakeWifi"));
    focusAppWindow();
    
    if(args.contains("--test"))
    {
        UnitTestRunner tester;
        tester.setPassesAreLogged(args.contains("-v"));
        tester.runAllTests();
        systemRequestedQuit();
    }
    
    
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
