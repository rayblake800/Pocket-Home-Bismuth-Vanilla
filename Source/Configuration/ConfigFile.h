/**
 * @file ConfigFile.h
 * 
 * ConfigFile reads and writes data from a json configuration file. ConfigFile
 * itself is abstract, each configuration file should have its own ConfigFile
 * subclass.  Each ConfigFile should provide a set of public, static, constant 
 * key String objects to use for accessing its data. 
 *
 * Along with reading and writing data, ConfigFile objects allow Configurable
 * objects to register to receive notification whenever particular data keys
 * are changed.
 * 
 * Because ConfigFiles manage access to a shared resource, each json file should
 * have only have one ConfigFile object. ConfigFile reads from the json file on
 * construction only, so any external changes to the file that occur after that
 * will be ignored and overwritten.
 * @see Configurable.h
 */
#pragma once
#include <map>
#include "../../JuceLibraryCode/JuceHeader.h"



class Configurable;

class ConfigFile {
public:

    /**
     * Writes any pending changes to the file before destruction.
     */
    virtual ~ConfigFile();

    /**
     * Register an object as tracking configuration changes. That object
     * is notified whenever any data it tracks is changed.
     * 
     * @param keys defines all data keys that configurable is tracking.
     * For each String key in keys, this ConfigFile will call 
     * configurable->configChanged(key) every time it changes the value 
     * of the data mapped to key.
     */
    void registerConfigurable(Configurable * configurable,
            Array<String> keys);

    /**
     * Removes an object from the list of objects to notify when configuration
     * changes.
     * 
     * @param configurable a configurable object to be unregistered.  If the
     * object wasn't actually registered, nothing will happen.
     * @param keys all keys that the configurable object will no longer track.
     * configurable will not be unregistered from any keys not contained in
     * this list.
     */
    void unregisterConfigurable(Configurable * configurable,
            Array<String> keys);
      
    //######################### Integer Data ###################################

    /**
     * Gets one of the integer values stored in the json configuration file
     * 
     * @throws std::out_of_range if intKey is not a value in this ConfigFile
     *  already, or if it's the key to a non-integer value. 
     * @return the integer value from the config file
     */
    int getConfigInt(String intKey);

    /**
     * Sets one of this ConfigFile's integer values, writing it to the config 
     * file if the value has changed.  
     * 
     * @throws std::out_of_range if intKey is not a value in this ConfigFile
     *  already, or if it's the key to a non-integer value. 
     */
    void setConfigInt(String intKey, int newValue);

    //######################### String Data ####################################

    /**
     * Gets one of the string values stored in the json configuration file
     * 
     * @throws std::out_of_range if stringKey is not a value in this ConfigFile
     *  already, or if it's the key to a non-String value. 
     * @return the string value from the config file
     */
    String getConfigString(String stringKey);

    /**
     * Sets one of this ConfigFile's string values, writing it to the config 
     * file if the value has changed.  
     * 
     * @throws std::out_of_range if stringKey is not a value in this ConfigFile
     *  already, or if it's the key to a non-String value. 
     */
    void setConfigString(String stringKey, String newValue);

    //######################### Boolean Data ###################################
    /**
     * Gets one of the boolean values stored in the json configuration file.
     * 
     * @throws std::out_of_range if boolKey is not a value in this ConfigFile
     *  already, or if it's the key to a non-boolean value. 
     * @return the boolean value from the config file
     */
    bool getConfigBool(String boolKey);

    /**
     * Sets one of this ConfigFile's boolean values, writing it to the config 
     * file if the value has changed.  
     * 
     * @throws std::out_of_range if boolKey is not a value in this ConfigFile
     *  already, or if it's the key to a non-boolean value. 
     */
    void setConfigBool(String boolKey, bool newValue);

    /**
     * @return true iff this ConfigFile and rhs have the same filename.
     */
    bool operator==(const ConfigFile& rhs) const;

protected:
    /**
     * This constructor should only be called when constructing ConfigFile
     * subclasses.
     * 
     * @param configFilename the name of a json file to be read or created in
     * ~/.pocket-home/. There should be a file with the same name in the asset
     * folder filled with default values.
     */
    ConfigFile(String configFilename);

    
    /**
     * @return the keys to all integer variables tracked in this config file.
     */
    virtual Array<String> getIntKeys() const;

    /**
     * @return the keys to all string variables tracked in this config file.
     */
    virtual Array<String> getStringKeys() const;

    /**
     * @return the keys to all boolean variables tracked in this config file.
     */
    virtual Array<String> getBoolKeys() const;

    /**
     * Read in this object's data from a json config object
     * 
     * @param config should pass in json configuration data from 
     * ~/.pocket-home/<filename>.json
     * 
     * @param defaultConfig should be either json data from the default config 
     * file in the assets folder, or a var object set to var::null. 
     * If defaultConfig is null and data is missing from the configuration file,
     * this method will open <filename>.json and load all data into 
     * defaultConfig. 
     */
    virtual void readDataFromJson(var& config, var& defaultConfig);

    /**
     * Copy all config data to a json object.
     * 
     * @param jsonObj a dynamicObject that can later be
     * written to a json file.
     * 
     * @pre any code calling this function is expected to have already
     * acquired the object's lock.
     */
    virtual void copyDataToJson(DynamicObject::Ptr jsonObj);

    /**
     * Checks if a property exists in a config data object loaded from a json
     * file.
     * 
     * @return true iff propertyKey has a value in config
     */
    bool propertyExists(var& config, String propertyKey);


    /**
     * Gets a property from json configuration data, or from default
     * configuration data if necessary
     * 
     * @param config should pass in json configuration data from 
     * ~/.pocket-home/<filename>.json
     * 
     * @param defaultConfig should be either json data from the default config 
     * file in the assets folder, or a var object set to var::null. 
     * If defaultConfig is null and data is missing from the configuration file,
     * this method will open <filename>.json and load all data into 
     * defaultConfig. 
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
     * acquired the ConfigFile's lock
     */
    void writeChanges();

    /**
     * Announce new changes to each object tracking a particular key.
     * 
     * @param key maps to a value that has changed in this ConfigFile. 
     * 
     * @pre make sure the lock is not held when calling this, so that
     * the Configurable objects can read the property changes.
     */
    void notifyConfigurables(String key);

    String filename;
    bool changesPending = false;
    std::map<String, int> intValues;
    std::map<String, String> stringValues;
    std::map<String, bool> boolValues;
    CriticalSection lock;
    static constexpr const char* CONFIG_PATH = "/.pocket-home/";

private:
    std::map<String, Array<Configurable*>> configured;

};