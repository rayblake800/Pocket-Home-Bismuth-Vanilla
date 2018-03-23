#include <sys/types.h>
#include <sys/wait.h>
#include "JsonWifiInterface.h"
#include "LibNMInterface.h"
#include "Utils.h"
#include "Audio.h"
#include "PocketHomeApplication.h"

void PocketHomeApplication::initialise(const String &commandLine)
{
    StringArray args;
    args.addTokens(commandLine, true);

    if (args.contains("--help"))
    {
        std::cerr << "arguments:" << std::endl;
        std::cerr << "  --help:	Print usage help" << std::endl;
        std::cerr << "  --fakewifi:	Use fake WifiStatus" << std::endl;
        quit();
    }

    // open sound handle
    if (!Audio::initAudio())
    {
        DBG("PocketHomeApplication::" << __func__
                << ": Sound failed to initialize");
    }

    
    homeWindow = new PocketHomeWindow(getApplicationName(), 
            args.contains("--fakewifi"),
            mainConfig,
            componentConfig);
}

void PocketHomeApplication::shutdown()
{
    homeWindow = nullptr;
}