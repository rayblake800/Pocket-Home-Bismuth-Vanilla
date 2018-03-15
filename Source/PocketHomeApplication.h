/**
 * @file PocketHomeApplication.h
 * 
 * PocketHomeApplication is the base application class that initializes and
 * shuts down the program
 */

#pragma once
#include "JuceHeader.h"
#include "PokeLookAndFeel.h"
#include "PocketHomeWindow.h"
#include "WifiStatus.h"
#include "BluetoothStatus.h"

class PocketHomeApplication : public JUCEApplication
{
public:
    PocketHomeApplication() { }
    virtual ~PocketHomeApplication() { }
    
    static PocketHomeApplication* getInstance();
    
    /**
     * @return a pointer to the wifi status monitoring object. 
     */
    WifiStatus* getWifiStatus();
    
    /**
     * @return a pointer to the bluetooth status monitoring object. 
     */
    BluetoothStatus* getBluetoothStatus();
    
private:
    
    void initialise(const String &commandLine) override;

    void shutdown() override;

    const String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }

    const String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed() override
    {
        return false;
    }

    PokeLookAndFeel lookAndFeel;

    ScopedPointer<PocketHomeWindow> homeWindow;
    ScopedPointer<WifiStatus> wifiStatus;
    ScopedPointer<BluetoothStatus> bluetoothStatus;
};