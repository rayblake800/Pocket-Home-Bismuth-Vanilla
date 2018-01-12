/*
  ==============================================================================

    ConfigFile.h
    Created: 26 Dec 2017 10:31:17am
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include <map>
#include "../../JuceLibraryCode/JuceHeader.h"

class Configurable;

//Component type keys
static const String appMenuButtonKey;
static const String appMenuKey;
static const String menuFrameKey;
static const String batteryKey;
static const String batteryPercentKey;
static const String clockKey;
static const String wifiKey;
static const String powerKey;
static const String settingsKey;

/**
 * Maps all configurable component types.
 */
enum ComponentType {
    APP_MENU_BUTTON,
    APP_MENU,
    MENU_FRAME,
    BATTERY,
    BATTERY_PERCENT,
    CLOCK,
    WIFI,
    POWER,
    SETTINGS
};

class ConfigFile {
public:

    /**
     * @param configFilename the name of a json file to be read or created in
     * ~/.pocket-home/. There should be a file with the same name in assets
     * filled with default values.
     */
    ConfigFile(String configFilename);

    /**
     * Writes any pending changes to the file before destruction.
     */
    virtual ~ConfigFile();

    /**
     * Register an object as tracking configuration changes. That object
     * is notified whenever any data it tracks is changed.
     * 
     * @param configurable an object that uses data stored in this ConfigFile 
     * @param keys all data keys that the object needs to track.
     */
    void registerConfigurable(Configurable * configurable,
            Array<String> keys);

    /**
     * Removes an object from the list of objects to notify when configuration
     * changes.
     * @param configurable a configurable object to be unregistered.  If the
     * object wasn't actually registered, nothing will happen.
     * @param keys all keys that the configurable object will no longer track.
     */
    void unregisterConfigurable(Configurable * configurable,
            Array<String> keys);

    //######################### String Data ####################################

    /**
     * Gets a string value
     * @param stringKey the key for the string value you need
     * @return the string value from the config file, or empty string if
     * nothing is found
     */
    String getConfigString(String stringKey);

    /**
     *Sets a string value, writing it to the config file if the value has
     * changed
     * @param configString the key of the string value to set
     * @param newValue the new value for the string
     */
    void setConfigString(String stringKey, String newValue);
    //######################### Boolean Data ###################################
    /**
     * Gets a boolean value
     * @param boolKey key string for the boolean value you need
     * @return the bool value from the config file
     */
    bool getConfigBool(String boolKey);

    /**
     *Sets a boolean value, writing it to the config file if the value has
     * changed
     * @param boolKey key of the boolean variable to access
     * @param newValue the new value for the bool
     */
    void setConfigBool(String boolKey, bool newValue);
    
    //ConfigFiles are equal if they have the same filename.
    bool operator==(const ConfigFile& rhs) const;
protected:
    /**
     * Read in this object's data from a json config object
     * 
     * @param config json data from ~/.pocket-home/filename.json
     * 
     * @param defaultConfig default json config data from the filename.json
     * in assets. If this value is var::null and default data is needed, this 
     * method will open it as the appropriate default config file from assets
     */
    virtual void readDataFromJson(var& config, var& defaultConfig);

    /**
     * Copy all config data to a json object.
     * 
     * @param jsonObj a dynamicObject that can later be
     * written to a json file
     * 
     * @pre any code calling this function is expected to have already
     * acquired the object's lock
     */
    virtual void copyDataToJson(DynamicObject::Ptr jsonObj);

    /**
     * Checks if a property exists in config data
     * @param config json configuration loaded from the file
     * @param propertyKey the key of some data member
     * @return true iff propertyKey has a value in config
     */
    bool propertyExists(var& config, String propertyKey);


    /**
     * Gets a property from json configuration data, or from default
     * configuration data if necessary
     * 
     * @param config json configuration data for this object.
     * 
     * @param defaultConfig backup configuration data source. If this value is
     * var::null and default data is needed, this method will open it as the 
     * appropriate default config file from assets
     * 
     * @param key the key value for some main property in the json config
     * 
     * @return the value read from config/defaultConfig at [key], or var::null
     * if nothing was found in either var object.
     */
    var getProperty(var& config, var& defaultConfig, String key);

    /**
     * Re-writes all data back to the config file, as long as there are
     * changes to write.
     * 
     * @pre any code calling this function is expected to have already
     * acquired the object's lock
     */
    void writeChanges();

    /**
     * Announce new changes to each object tracking a particular key.
     * @param key the value that has changed in this ConfigFile. 
     * @pre make sure the lock is not held when calling this, so that
     * the Configurable objects can read the property changes.
     */
    void notifyConfigurables(String key);

    /**
     * @return the keys to all string variables tracked in this config file.
     */
    virtual Array<String> getStringKeys() const = 0;

    /**
     * @return the keys to all boolean variables tracked in this config file.
     */
    virtual Array<String> getBoolKeys() const = 0;
    String filename;
    bool changesPending = false;
    std::map<String, String> stringValues;
    std::map<String, bool> boolValues;
    CriticalSection lock;

    //constants and default values: 
    static constexpr const char* CONFIG_PATH = "/.pocket-home/";

private:
    std::map<String, Array<Configurable*>> configured;

};