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
 * only one ConfigFile object should exist for each json configuration file.  
 * ConfigFile objects should be thread safe.
 */
#pragma once
#include <map>
#include "JuceHeader.h"

#define CONFIG_PATH "~/.pocket-home/"

class ConfigFile
{
public:

    /**
     * Writes any pending changes to the file before destruction.
     */
    virtual ~ConfigFile();

    /**
     * Gets one of the values stored in the json configuration file.
     * 
     * @param   key The key string that maps to the desired value.
     * 
     * @throws  std::out_of_range if key does not map to a value of type T
     *           in this config file
     * 
     * @return  the value read from the config file.
     */
    template<typename T>
    T getConfigValue(String key)
    {
        std::map<String, T>& fileDataMap = getMapReference<T>();
        const ScopedLock readLock(configLock);
        try
        {
            return fileDataMap.at(key);
        }
        catch (std::out_of_range e)
        {
            DBG("ConfigFile::" << __func__ << ": key \"" << key
                    << "\" is not value of this type stored in " << filename);
            throw e;
        }
    }

    /**
     * Sets one of this ConfigFile's values, writing it to the config 
     * file if the value has changed.  
     * 
     * @param key        The key string that maps to the value being updated.
     * 
     * @param newValue   The new value to save to the file.
     * 
     * @throws           std::out_of_range if key does not map to a value of 
     *                    type T in this config file
     */
    template<typename T>
    void setConfigValue(String key, T newValue)
    {
        std::map<String, T>& fileDataMap = getMapReference<T>();
        const ScopedLock writeLock(configLock);
        try
        {
            if (fileDataMap.at(key) != newValue)
            {
                fileChangesPending = true;
                fileDataMap[key] = newValue;
                writeChanges();

                const ScopedUnlock allowDataAccess(configLock);
                notifyListeners(key);
            }
        }
        catch (std::out_of_range e)
        {
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

    /**
     * Listeners receive updates whenever key values they track are updated in
     * a config file.  Listeners may track any number of keys in any number of
     * ConfigFile objects.
     */
    class Listener
    {
    protected:
        friend class ConfigFile;

        Listener() { }
        
        /**
         * Listeners safely remove themselves from all tracked ConfigFiles when
         * they are destroyed.
         */
        virtual ~Listener();
        
        /**
         * Calls configValueChanged() for every key tracked by this listener.
         */
        void loadAllConfigProperties();
        
    private:
        /**
         * Called whenever a key tracked by this listener changes in the config
         * file, listeners should override this to react to config changes.
         * 
         * @param config        A reference to the updated ConfigFile object.
         * 
         * @param propertyKey   Passes in the updated value's key.
         */
        virtual void configValueChanged
        (ConfigFile* config, String propertyKey) = 0;
        
        //holds references to all ConfigFiles this listener follows.
        Array<ConfigFile*> configFiles; 
    };
    
    /**
     * Adds a listener to the list of objects to notify when config values
     * change.
     * 
     * @param listener      The object that will be notified.
     * 
     * @param trackedKeys   The set of keys that the listener wants to track.
     */
    void addListener(Listener* listener, StringArray trackedKeys);
    
    /**
     * Removes a listener from this ConfigFile.
     * 
     * @param listener  This will no longer receive updates on any changes to
     *                   this ConfigFile.
     */
    void removeListener(Listener* listener);
    
    /**
     * Announce new changes to each object tracking a particular key.
     * 
     * @param key maps to a value that has changed in this ConfigFile. 
     * 
     * @pre make sure the lock is not held when calling this, so that
     *       the Configurable objects can read the property changes.
     */
    void notifyListeners(String key);

protected:

    /**
     * Defines the basic data types that can be stored in all ConfigFile
     * objects.
     */
    enum SupportedDataType
    {
        stringType,
        intType,
        boolType
    };

    /**
     * Used for tracking which data type is associated with a key.
     */
    struct DataKey
    {
        String keyString;
        SupportedDataType dataType;
    };

    /**
     * This constructor should only be called when constructing ConfigFile
     * subclasses.
     * 
     * @param configFilename The name of a json file to be read or created in
     *                        ~/.pocket-home/. There should be a file with the 
     *                        same name in the asset folder filled with default 
     *                        values.
     */
    ConfigFile(String configFilename);

    /**
     * @return the keys to all variables tracked in this config file.
     */
    virtual std::vector<DataKey> getDataKeys() const = 0;

    /**
     * Read in this object's data from a json config object
     * 
     * @param config         This should pass in json configuration data from 
     *                        ~/.pocket-home/<filename>.json.
     * 
     * @param defaultConfig  This should be either json data from the default 
     *                        config file in the assets folder, or a void var. 
     *                        If defaultConfig is null and data is missing from 
     *                        the configuration file, this method will open 
     *                        <filename>.json and load all data into 
     *                        defaultConfig. 
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
     * Marks this ConfigFile as containing changes that need to be written to
     * the underlying .json file.
     */
    void markPendingChanges();

    /**
     * Re-writes all data back to the config file, as long as there are
     * changes to write.
     * 
     * @pre any code calling this function is expected to have already
     * acquired the ConfigFile's lock
     */
    void writeChanges();

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
    template <class T> void initMapProperty(String key, T newValue)
    {
        std::map<String, T>& fileDataMap = getMapReference<T>();
        const ScopedLock writeLock(configLock);
        fileDataMap[key] = newValue;
    }

    CriticalSection configLock;
     
    String filename;
    
    
    bool fileChangesPending = false;
    std::map<String, int> intValues;
    std::map<String, String> stringValues;
    std::map<String, bool> boolValues;
    std::map<Listener*, StringArray> listenerKeys;
    std::map<String, Array<Listener*>> keyListeners;

};