#include "Audio.h"
#include "PocketHomeApplication.h"

/**
 * This is called by the Juce framework to initialize the program.
 * This will initialize audio and create the program window.
 */
void PocketHomeApplication::initialise(const String &commandLine)
{
    StringArray args;
    args.addTokens(commandLine, true);

    if (args.contains("--help"))
    {
        std::cerr << "arguments:" << std::endl;
        std::cerr << "  --help:	Print usage help" << std::endl;
        std::cerr << "  --fakeWifi:	Use fake WifiStatus" << std::endl;
        quit();
    }

    // open sound handle
    if (!Audio::initAudio())
    {
        DBG("PocketHomeApplication::" << __func__
                << ": Sound failed to initialize");
    }


    homeWindow = new PocketHomeWindow(getApplicationName(),
            args.contains("--fakeWifi"),
            mainConfig,
            componentConfig);
}

/**
 * This is called by the Juce framework to safely shut down the application.
 */
void PocketHomeApplication::shutdown()
{
    homeWindow = nullptr;
}