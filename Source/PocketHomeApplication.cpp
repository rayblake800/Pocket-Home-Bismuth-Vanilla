#include "Audio.h"
#include "PocketHomeApplication.h"
#include "XWindowInterface.h"
#include "TempTimer.h"
#include "PocketHomeWindow.h"

//Milliseconds to wait between window focus attempts.
static const constexpr int focusWaitMs = 200;

//sets if tests should run after the window initializes
static bool runTests = false;
static bool verboseTesting = false;

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
        tester.runAllTests();
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
    if (!Audio::chipAudioInit())
    {
        DBG("PocketHomeApplication::" << __func__
                << ": Sound failed to initialize");
    }
    runTests = args.contains("--test");
    verboseTesting = args.contains("-v");
    if(!runTests)
    {
        gLibThread = new GLibSignalThread();
        homeWindow = new PocketHomeWindow
                (getApplicationName(), args.contains("--fakeWifi"));
    }
    else
    {
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
