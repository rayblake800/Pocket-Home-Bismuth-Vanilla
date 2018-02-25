/**
 * @file PocketHomeApplication.h
 * 
 * TODO: documentation, organization
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Configuration/MainConfigFile.h"
#include "Configuration/ComponentConfigFile.h"
#include "Wifi/WifiStatus.h"
#include "Wifi/WifiStatusJson.h"
#include "Wifi/WifiStatusNM.h"
#include "BluetoothStatus.h"
#include "PokeLookAndFeel.h"
#include "Utils.h"
#include "PocketHomeWindow.h"

class PocketHomeApplication : public JUCEApplication {
public:
    PocketHomeApplication();

    static PocketHomeApplication* getInstance();

    WifiStatus& getWifiStatus();

    BluetoothStatus& getBluetoothStatus();

private:
    bool initAudio();
    PokeLookAndFeel lookAndFeel;
#ifdef LINUX
    WifiStatusNM wifiStatusNM;
#else
    WifiStatusJson wifiStatusNM;
#endif //LINUX

    WifiStatusJson wifiStatusJson;
    WifiStatus *wifiStatus;

    BluetoothStatus bluetoothStatus;
    const String getApplicationName() override;
    const String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;

    void initialise(const String &commandLine) override;
    void shutdown() override;

private:
    ScopedPointer<PocketHomeWindow> homeWindow;
};