/**
 * @file PocketHomeWindow.h
 * 
 * PocketHomeWindow is the application window object and the root component in 
 * the component display tree. 
 */

#pragma once
#include "LoginPage.h"
#include "PageFactory.h"
#include "PokeLookAndFeel.h"
#include "MainConfigFile.h"
#include "ComponentConfigFile.h"
#include "PageStackComponent.h"
#include "WindowFocus.h"

class PocketHomeWindow : public WindowFocus::BroadcastWindow
{
public:
    /**
     * @param windowName         Sets the text of the window title bar.
     * 
     * @param fakeWifi           If true, wifi-using components in this window
     *                            will get all their wifi data from a simulated 
     *                            wifi device.
     * 
     * @param mainConfig         Holds shared configuration data.
     * 
     * @param componentConfig    Holds shared UI component configuration data.
     */
    PocketHomeWindow(
            String windowName, 
            bool fakeWifi,
            MainConfigFile& mainConfig,
            ComponentConfigFile& componentConfig);

    virtual ~PocketHomeWindow() { }

private:
    /**
     * closes the application normally.
     */
    void closeButtonPressed() override;

    /**
     * Resize page content to match window size.
     */
    void resized() override;
   
    //Loads config data from the config.json file
    MainConfigFile& mainConfig;
    
    //Loads component settings from the components.json file
    ComponentConfigFile& componentConfig;

    //Factory object for creating all UI pages
    PageFactory pageFactory;

    //Holds all page components, besides the login page
    PageStackComponent pageStack;

    //LoginPage will be initially shown instead of the pageStack if a password
    //has been set.
    ScopedPointer<LoginPage> loginPage;

    //Controls default component appearances.
    PokeLookAndFeel lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PocketHomeWindow)
};
