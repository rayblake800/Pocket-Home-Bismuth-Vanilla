/**
 * @file MainConfigFile.h
 * 
 * MainConfigFile loads general application settings from
 * the config.json file.
 */
#pragma once
#include "ResourceManager.h"
#include "ConfigFile.h"

class MainConfigFile : public ResourceManager
{
public:
    MainConfigFile();

    virtual ~MainConfigFile() { }

    /**
     * Gets one of the values stored in the json configuration file.
     * @see ConfigFile.h
     */
    template<typename T > T getConfigValue(String key)
    {
        const ScopedLock readLock(configLock);
        MainJson* config = static_cast<MainJson*> (sharedResource.get());
        return config->getConfigValue<T>(key);
    }

    /**
     * Sets one of this ConfigFile's values, writing it to the config 
     * file if the value has changed.  
     * @see ConfigFile.h
     */
    template<typename T>
    void setConfigValue(String key, T newValue)
    {
        const ScopedLock readLock(configLock);
        MainJson* config = static_cast<MainJson*> (sharedResource.get());
        config->setConfigValue<T>(key, newValue);
    }

    /**
     * Add a listener to track setting changes.
     * @see ConfigFile.h
     */
    void addListener(ConfigFile::Listener* listener, StringArray trackedKeys);

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
    //Sets if the cursor should be shown
    static const String showCursorKey;
    //Sets if the HomePage clock should be shown
    static const String showClockKey;
    //Sets if the HomePage clock should use 12 hour or 24 hour time
    static const String use24HrModeKey;

    //All possible values of menuTypeKey
    static const StringArray menuTypes;

private:

    class MainJson : public ConfigFile
    {
    public:
        MainJson();

        virtual ~MainJson() { }

    private:
        /**
         * @return all keys tracked by this file, along with their corresponding
         *          data types.
         */
        std::vector<DataKey> getDataKeys() const;
    };

    //filename to use for reading/storing MainConfigFile data
    static constexpr const char* filenameConst = "config.json";

    //ResourceManager shared object and lock;
    static ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static CriticalSection configLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainConfigFile)
};
