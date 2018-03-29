/**
 * @file PocketHomeApplication.h
 * 
 * PocketHomeApplication is the base application class that initializes and
 * shuts down the program.
 */

#pragma once
#include "JuceHeader.h"
#include "PocketHomeWindow.h"
#include "MainConfigFile.h"
#include "ComponentConfigFile.h"
#include "ComponentConfigFile.h"

class PocketHomeApplication : public JUCEApplication
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
    void initialise(const String &commandLine) override;

    /**
     * This is called by the Juce framework to safely shut down the program.
     */
    void shutdown() override;

    /**
     * @return the application name set in Juce project files.
     */
    const String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }

    /**
     * @return the application version set in Juce project files.
     */
    const String getApplicationVersion() override
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
    
    //Holds the single application window.
    ScopedPointer<PocketHomeWindow> homeWindow;
    
    //Holds general user-set program configuration data.  This should be the 
    //only MainConfigFile object, to be passed by reference to any objects that
    //depend on it.
    MainConfigFile mainConfig;
    
    //Holds user-set UI component configuration data.  This should be the 
    //only ComponentConfigFile object, to be passed by reference to any objects 
    //that depend on it.
    ComponentConfigFile componentConfig;
};