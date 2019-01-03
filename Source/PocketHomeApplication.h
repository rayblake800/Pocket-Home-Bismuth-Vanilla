#pragma once
#include "JuceHeader.h"
#include "PokeLookAndFeel.h"
#include "Config/MainFile.h"
#include "ComponentConfigFile.h"
#include "ColourConfigFile.h"
#include "Wifi/Manager.h"

/**
 * @file PocketHomeApplication.h
 * 
 * @brief The base class that initializes and shuts down the program.
 */

class PocketHomeApplication : public juce::JUCEApplication
{
public:
    PocketHomeApplication() { }
    
    virtual ~PocketHomeApplication() { }
    
private:
    
    /**
     * This is called by the Juce framework to initialize the program.
     * This will initialize audio and create the program window.
     * 
     * @param commandLine  This will contain the full command used to launch
     *                      the program, so that any command line parameters
     *                      can be processed.
     */
    void initialise(const juce::String &commandLine) override;

    /**
     * This is called by the Juce framework to safely shut down the program.
     */
    void shutdown() override;

    /**
     * @return the application name set in Juce project files.
     */
    const juce::String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }

    /**
     * @return the application version set in Juce project files.
     */
    const juce::String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }

    /**
     * @return false, it is very unlikely that anyone actually has reason to
     *          run multiple instances of this application.
     */
    bool moreThanOneInstanceAllowed() override
    {
        return false;
    }
    
    //global resource objects:
    //These objects remain allocated as long as one instance of them exists
    //somewhere.  Declaring them here ensures that they will remain allocated
    //as long as the application is running.

    //Holds general user-set program configuration data.
    Config::MainFile mainConfig;
    
    //Holds user-set UI component configuration data.
    ComponentConfigFile componentConfig;
    
    //Holds UI colour settings
    ColourConfigFile colourConfig;

    /* These resources are dynamically allocated because they should be created 
     * in the order they are listed here. */

    //Manages wifi network operations.
    std::unique_ptr<Wifi::Manager> wifiManager;

    //The program appearance manager.
    std::unique_ptr<PokeLookAndFeel> lookAndFeel = nullptr;
    
    //The single program window.
    std::unique_ptr<juce::DocumentWindow> homeWindow = nullptr;
};
