#include "Audio.h"
#include "PocketHomeApplication.h"

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

    
    if(args.contains("--test"))
    {
        UnitTestRunner tester;
        tester.setPassesAreLogged(args.contains("-v"));
        tester.runAllTests();
        JUCEApplicationBase::quit();
    }
    
    else
    {
        // open sound handle
        if (!Audio::initAudio())
        {
            DBG("PocketHomeApplication::" << __func__
                    << ": Sound failed to initialize");
        }
        homeWindow = new PocketHomeWindow
                (getApplicationName(), args.contains("--fakeWifi"));
    }
    
}

/**
 * This is called by the Juce framework to safely shut down the application.
 */
void PocketHomeApplication::shutdown()
{
    using namespace juce;
    homeWindow = nullptr;
    LookAndFeel::setDefaultLookAndFeel(nullptr);
    lookAndFeel = nullptr;
}
