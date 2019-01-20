#pragma once
#include "JuceHeader.h"
#include "Theme_LookAndFeel.h"
#include "Config_MainFile.h"
#include "Layout_Component_ConfigFile.h"
#include "Theme_Colour_ConfigFile.h"
#include "Theme_Image_ConfigFile.h"
#include "GLib_DBusThreadRunner.h"
#include "Wifi_Manager.h"

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
    
    /* Application resource objects: 
     *  These objects remain allocated as long as one instance of them exists
     * somewhere. Declaring them here ensures that they will remain allocated
     * as long as the application is running.
     */

    /* Holds general user-set program configuration data: */
    Config::MainFile mainConfig;
    
    /* Holds UI component layout data: */
    Layout::Component::ConfigFile layoutConfig;
    
    /* Holds UI colour settings: */
    Theme::Colour::ConfigFile colourConfig;

    /* These resources are dynamically allocated because they should be created 
     * in the order they are listed here. */

    /* Handles inter-process communication over DBus: */
    std::unique_ptr<GLib::DBusThreadRunner> dBusThread;

    /* Manages wifi network operations: */
    std::unique_ptr<Wifi::Manager> wifiManager;

    /* The program appearance manager: */
    std::unique_ptr<Theme::LookAndFeel> lookAndFeel = nullptr;
    
    /* The single program window: */
    std::unique_ptr<juce::DocumentWindow> homeWindow = nullptr;
};
