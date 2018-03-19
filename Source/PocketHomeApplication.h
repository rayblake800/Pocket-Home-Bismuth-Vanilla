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
#include "WifiStateManager.h"
#include "BluetoothStatus.h"

class PocketHomeApplication : public JUCEApplication
{
public:
    PocketHomeApplication() { }
    virtual ~PocketHomeApplication() { }
    
    static PocketHomeApplication* getInstance();
    
    /**
     * @return a reference to the wifi manager.
     */
    WifiStateManager& getWifiManager();
    
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
    WifiStateManager wifiManager;

    ScopedPointer<PocketHomeWindow> homeWindow;
    ScopedPointer<BluetoothStatus> bluetoothStatus;
};