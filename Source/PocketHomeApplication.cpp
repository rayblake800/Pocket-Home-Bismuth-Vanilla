#include <sys/types.h>
#include <sys/wait.h>
#include "JsonWifiInterface.h"
#include "LibNMInterface.h"
#include "Utils.h"
#include "Audio.h"
#include "PocketHomeApplication.h"

PocketHomeApplication* PocketHomeApplication::getInstance()
{
    return dynamic_cast<PocketHomeApplication *>
            (JUCEApplication::getInstance());
}

WifiStateManager& PocketHomeApplication::getWifiManager()
{
    return wifiManager;
}

BluetoothStatus* PocketHomeApplication::getBluetoothStatus()
{
    return bluetoothStatus;
}

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

    if (!args.contains("--noWifi"))
    {
        //Initialize wifi status thread
        if (args.contains("--fakewifi"))
        {
            wifiManager.setNetworkInterface(new JsonWifiInterface());
        }
        else
        {
#            ifdef JUCE_LINUX
            wifiManager.setNetworkInterface(new LibNMInterface());
#            else          
            wifiManager.setNetworkInterface(new JsonWifiInterface());
#            endif
        }
    }
    bluetoothStatus = new BluetoothStatus();
    LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);
    homeWindow = new PocketHomeWindow(getApplicationName());
}

void PocketHomeApplication::shutdown()
{
    homeWindow = nullptr;
}