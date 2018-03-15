/**
 * @file ConfigFile.h
 * 
 * ConfigFile reads and writes data from a json configuration file. ConfigFile
 * itself is abstract, each configuration file should have its own ConfigFile
 * subclass.  Each ConfigFile should provide access to a set of key Strings
 * for accessing its specific data. 
 *
 * Along with reading and writing data, ConfigFile objects allow Configurable
 * objects to register to receive notification whenever particular data keys
 * are changed.
 * 
 * ConfigFile reads from each json file only once per program instance, so any 
 * external changes to the file that occur after that will be ignored and 
 * overwritten.  To reduce file IO and prevent concurrent file modification,
 * ConfigFile data is shared between ConfigFile instances.  ConfigFile objects
 * should be threadsafe.
 * @see Configurable.h
 */
#pragma once
#include <map>
#include "JuceHeader.h"

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
     * @param configurable
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
     * 
     * @param keys all keys that the configurable object will no longer track.
     * configurable will not be unregistered from any keys not contained in
     * this list.
     */
    void unregisterConfigurable(Configurable * configurable,
            Array<String> keys);

    /**
     * Gets one of the values stored in the json configuration file
     * 
     * @param key
     * @throws std::out_of_range if key does not map to a value of type T
     * in this config file
     * 
     * @return the value from the config file
     */
    template<typename T>
    T getConfigValue(String key) {
        std::map<String, T>& fileDataMap = getMapReference<T>();
        const ScopedLock readLock(getFileLock());
        try {
            return fileDataMap.at(key);
        } catch (std::out_of_range e) {
            DBG("ConfigFile::" << __func__ << ": key \"" << key
                    << "\" is not value of this type stored in " << filename);
            throw e;
        }
    }

    /**
     * Sets one of this ConfigFile's values, writing it to the config 
     * file if the value has changed.  
     * 
     * @param key
     * @param newValue
     * @throws std::out_of_range if key does not map to a value of type T
     * in this config file
     */
    template<typename T>
    void setConfigValue(String key, T newValue) {
        std::map<String, T>& fileDataMap = getMapReference<T>();
        const ScopedLock writeLock(getFileLock());
        try {
            if (fileDataMap.at(key) != newValue) {
                fileChangesPending[filename] = true;
                fileDataMap[key] = newValue;
                writeChanges();

                const ScopedUnlock allowDataAccess(getFileLock());
                notifyConfigurables(key);
            }
        } catch (std::out_of_range e) {
            DBG(__PRETTY_FUNCTION__ << ":");
            DBG("\tkey " << key << " is not a value of this type stored in " 
                    << filename);
            throw e;
        }
    }
    
    /**
     * @return true iff this ConfigFile and rhs have the same filename.
     */
    bool operator==(const ConfigFile& rhs) const;

protected:
    
    /**
     * Defines the basic data types that can be stored in all ConfigFile
     * objects.
     */
    enum SupportedDataType{
        stringType,
        intType,
        boolType
    };
    
    struct DataKey{
      String keyString;  
      SupportedDataType dataType;
    };
    
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
     * @return the CriticalSection shared by all ConfigFile objects that
     * access the same file as this one.
     */
    CriticalSection& getFileLock();

    /**
     * Opens and reads data from this ConfigFile's json file.  This will mark
     * the file as opened, so that ConfigFiles can avoid reading in file data
     * more than once.
     * @return the file's json data packaged as a var object, or a void var
     * if the file was opened already.
     */
    var openFile();

    /**
     * Check to see if this ConfigFile has already read data from its json file.
     * @return true iff the file has been read.
     */
    bool fileOpened();

    /**
     * Marks the ConfigFile as containing changes that need to be written
     * back to the object's json file.
     */
    void markPendingChanges();

    /**
     * @return the keys to all integer variables tracked in this config file.
     */
    virtual std::vector<DataKey> getDataKeys() const = 0;

    /**
     * Read in this object's data from a json config object
     * 
     * @param config should pass in json configuration data from 
     * ~/.pocket-home/<filename>.json
     * 
     * @param defaultConfig should be either json data from the default config 
     * file in the assets folder, or a void var. 
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
     * file in the assets folder, or a void var. 
     * If defaultConfig is null and data is missing from the configuration file,
     * this method will open <filename>.json and load all data into 
     * defaultConfig. 
     * 
     * @return the value read from config/defaultConfig at [key], or a void var
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
    static constexpr const char* CONFIG_PATH = "/.pocket-home/";
private:

    /**
     * @return a reference to the map that stores data values of type T for
     * ConfigFiles with this object's filename.
     */
    template <class T> std::map<String, T>& getMapReference();
    
    /**
     * Sets a property in the appropriate data map.  This does not notify
     * Configurables or mark the value as a change waiting to be written to the
     * file.
     * 
     * @param key
     * @param newValue
     */
    template <class T> void initMapProperty(String key, T newValue) {
        std::map<String, T>& fileDataMap = getMapReference<T>();
        const ScopedLock writeLock(getFileLock());
        fileDataMap[key] = newValue;
    }
    

    static CriticalSection configLock;
    static std::map<String, CriticalSection> fileLocks;
    static std::map<String, bool> openFileMap;
    static std::map<String, bool> fileChangesPending;
    static std::map<String, std::map<String, int>> intValues;
    static std::map<String, std::map<String, String>> stringValues;
    static std::map<String, std::map<String, bool>> boolValues;
    static std::map<String, std::map<String, Array<Configurable*>>> configured;

};