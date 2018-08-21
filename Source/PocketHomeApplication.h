#pragma once
#include "JuceHeader.h"
#include "PocketHomeWindow.h"
#include "PokeLookAndFeel.h"
#include "MainConfigFile.h"
#include "ComponentConfigFile.h"
#include "ColourConfigFile.h"
#include "GLibSignalThread.h"

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
    MainConfigFile mainConfig;
    
    //Holds user-set UI component configuration data.
    ComponentConfigFile componentConfig;
    
    //Holds UI colour settings
    ColourConfigFile colourConfig;

    //Runs the GLib event loop
    GLibSignalThread gLibThread;
    
    //The program appearance manager.  This is dynamically allocated because it
    //should be created after/destroyed before all of the above resources.
    juce::ScopedPointer<PokeLookAndFeel> lookAndFeel = nullptr;
    
    //The single program window.  This is dynamically allocated because it
    //should be created after/destroyed before all of the above resources.
    juce::ScopedPointer<PocketHomeWindow> homeWindow = nullptr;
};
