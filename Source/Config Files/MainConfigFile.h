#pragma once
#include "ResourceManager.h"
#include "ConfigFile.h"

/**
 * @file MainConfigFile.h
 * 
 * @brief Loads general application settings from the config.json file.
 */

class MainConfigFile : public ResourceManager
{
public:
    MainConfigFile();

    virtual ~MainConfigFile() { }

    /**
     * Gets one of the values stored in the json configuration file.
     * 
     * @see ConfigFile.h
     */
    template<typename T > T getConfigValue(juce::String key)
    {
        const juce::ScopedReadLock readLock(configLock);
        MainJson* config = static_cast<MainJson*> (sharedResource.get());
        return config->getConfigValue<T>(key);
    }

    /**
     * Sets one of this ConfigFile's values, writing it to the config 
     * file if the value has changed.  
     * 
     * @see ConfigFile.h
     */
    template<typename T>
    void setConfigValue(juce::String key, T newValue)
    {
        const juce::ScopedWriteLock writeLock(configLock);
        MainJson* config = static_cast<MainJson*> (sharedResource.get());
        config->setConfigValue<T>(key, newValue);
    }

    /**
     * Add a listener to track setting changes.
     * 
     * @see ConfigFile.h
     */
    void addListener(ConfigFile::Listener* listener, juce::StringArray trackedKeys);

    // ### Integer value keys: ###
    //sets the maximum number of AppMenu icon rows per screen
    static const juce::String maxRowsKey;
    //sets the maximum number of AppMenu icon columns per screen
    static const juce::String maxColumnsKey;

    // ### String value keys: ###
    //sets the HomePage background color or image.
    static const juce::String backgroundKey;
    //sets the AppMenuComponent type to use on the home page.
    static const juce::String menuTypeKey;

    //sets the command used to shut down the system on the power page.
    static const juce::String shutdownCommandKey;
    //sets the command used to restart the system on the power page.
    static const juce::String restartCommandKey;
    //Sets the command used to turn off the display from the power page.
    static const juce::String sleepCommandKey;
    //Sets the string prefix to put before commands in order to launch them
    //from the terminal
    static const juce::String termLaunchCommandKey;
    
    //Sets the name of the wifi interface
    static const juce::String wifiInterfaceKey;

    // ### Boolean value keys: ###
    //Sets if the cursor should be shown
    static const juce::String showCursorKey;
    //Sets if the HomePage clock should be shown
    static const juce::String showClockKey;
    //Sets if the HomePage clock should use 12 hour or 24 hour time
    static const juce::String use24HrModeKey;

    //All possible values of menuTypeKey
    static const juce::StringArray menuTypes;

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
        const std::vector<DataKey>& getDataKeys() const;
    };

    //filename to use for reading/storing MainConfigFile data
    static constexpr const char* filenameConst = "config.json";

    //ResourceManager shared object and lock;
    static juce::ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static juce::ReadWriteLock configLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainConfigFile)
};
