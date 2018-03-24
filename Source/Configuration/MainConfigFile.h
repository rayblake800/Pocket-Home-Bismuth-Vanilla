/**
 * @file MainConfigFile.h
 * 
 * MainConfigFile loads general application settings from
 * the config.json file.
 */
#pragma once
#include "ConfigFile.h"

class MainConfigFile : public ConfigFile {
public:
    MainConfigFile();
    
    virtual ~MainConfigFile() { }
 
    // ### Integer value keys: ###
    //sets the maximum number of AppMenu icon rows per screen
    static const String maxRowsKey;
    //sets the maximum number of AppMenu icon columns per screen
    static const String maxColumnsKey;
    
    // ### String value keys: ###
    //sets the HomePage background color or image.
    static const String backgroundKey;
    //sets the AppMenuComponent type to use on the home page.
    static const String menuTypeKey;
    
    //sets the command used to shut down the system on the power page.
    static const String shutdownCommandKey;
    //sets the command used to restart the system on the power page.
    static const String restartCommandKey;
    //Sets the command used to turn off the display from the power page.
    static const String sleepCommandKey;
    //Sets the string prefix to put before commands in order to launch them
    //from the terminal
    static const String termLaunchCommandKey;
    
    // ### Boolean value keys: ###
    //Sets if the cursor should be shown or hidden
    static const String showCursorKey;
    
    /**
     * @return all keys tracked by this file, along with their corresponding
     *          data types.
     */
    std::vector<DataKey> getDataKeys() const;
    
    //All possible values of menuTypeKey
    static const StringArray menuTypes;

private:
    //filename to use for reading/storing MainConfigFile data
    static constexpr const char* filenameConst = "config.json";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainConfigFile)
};
