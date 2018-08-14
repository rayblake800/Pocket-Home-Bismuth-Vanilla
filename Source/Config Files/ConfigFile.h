#pragma once
#include <map>
#include "ResourceManager.h"
#include "JSONFile.h"
#include "JuceHeader.h"
#include "ConfigAlertWindows.h"

/**
 * @file ConfigFile.h
 * 
 * @brief Reads and writes data from a JSON configuration file. 
 * 
 *  ConfigFile provides an abstract basis for ResourceManager sharedResources 
 * that access JSON file data.  Each ConfigFile should provide access to a set 
 * of key strings for accessing its specific data.  A default version of each 
 * ConfigFile's JSON resource file should be placed in the configuration 
 * subdirectory of the asset folder.
 *
 * Along with reading and writing data, ConfigFile objects allow listener
 * objects to register to receive notification whenever particular data keys
 * are changed.
 * 
 * ConfigFile reads from each JSON file only once per program instance, so any 
 * external changes to the file that occur while the program is running will
 * most likely be ignored and may be overwritten.
 */

class ConfigFile : public ResourceManager::SharedResource
{
protected:
    /**
     * This constructor should only be called when constructing ConfigFile
     * subclasses.
     * 
     * @param configFilename  The name of a JSON file to read or create in
     *                        the config file directory. There should be a file
     *                        with the same name in the asset folder filled with
     *                        default values.
     */
    ConfigFile(juce::String configFilename);

public:
    /**
     * Writes any pending changes to the file before destruction.
     */
    virtual ~ConfigFile();

    /**
     * Gets one of the values stored in the json configuration file.
     * 
     * @param key  The key string that maps to the desired value.
     * 
     * @tparam T   The value's data type.
     * 
     * @return  The value read from the config file.
     * 
     * @throws BadKeyException           If the key parameter was not a valid 
     *                                   key string for this ConfigFile.
     * 
     * @throws JSONFile::TypeException   If the key does not map to a value of 
     *                                   type T in this config file.
     */
    template<typename T > T getConfigValue(juce::String key)
    {      
        using namespace juce;
        if(!isValidKey(key))
        {
            throw BadKeyException(key);
        }
        try
        {
            return configJson.getProperty<T>(key);
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigFile::" << __func__ 
                    << ": Failed to load key \"" 
                    << e.getPropertyKey()
                    <<"\" , expected type:" << e.getExpectedType()
                    <<", actual type: "  << e.getFoundType()
                    << ", error = " << e.getErrorMessage());
            throw e;
        }
        catch (JSONFile::FileException e)
        {
            //Failed to access .json file
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        return T();
    }

    /**
     * Sets one of this ConfigFile's values, notifying listeners and writing to
     * the JSON file if the value is changed.
     * 
     * @param key        The key string that maps to the value being updated.
     * 
     * @param newValue   The new value to save to the file.
     * 
     * @tparam T         The value data type.
     * 
     * 
     * @throws BadKeyException           If the key parameter was not a valid 
     *                                   key string for this ConfigFile.
     */
    template<typename T>
    void setConfigValue(juce::String key, T newValue)
    {
        using namespace juce;
        if(!isValidKey(key))
        {
            throw BadKeyException(key);
        }
        if(updateProperty<T>(key, newValue))
        {
            configJson.writeChanges();
            notifyListeners(key);
        }
    }
     
    /**
     * Signals an attempt to access an invalid config value in a ConfigFile.
     */
    struct BadKeyException : public std::exception
    {
    public:
        /**
         * @param invalidKey  The invalid key string that caused this exception.
         */
        BadKeyException(juce::String invalidKey) : invalidKey(invalidKey) { }
        
        /**
         * Gets the invalid key that caused the exception.
         * 
         * @return  The unexpected key value.
         */
        juce::String getInvalidKey()
        {
            return invalidKey;
        }
    private:
        const juce::String invalidKey;
    };

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
         * file. Listeners should implement this to handle config changes.
         * 
         * @param propertyKey   Passes in the updated value's key.
         */
        virtual void configValueChanged(juce::String propertyKey) = 0;

        //Prevents concurrent modification of tracked key values.
        juce::CriticalSection configKeyAccess; 
        //Holds references to all keys in ConfigFiles this listener follows.
        std::map<ConfigFile*, juce::StringArray> configKeyMap;
    };

    /**
     * Adds a listener to the list of objects to notify when config values
     * change.
     * 
     * @param listener      The object that will be notified.
     * 
     * @param trackedKeys   The set of keys that the listener wants to track.
     */
    void addListener(Listener* listener, juce::StringArray trackedKeys);

    /**
     * Removes a listener from this ConfigFile.
     * 
     * @param listener  This will no longer receive updates on any changes to
     *                  this ConfigFile.
     */
    void removeListener(Listener * listener);

protected:
    /**
     * Announce new changes to each object tracking a particular key.
     * 
     * @param key  Maps to a value that has changed in this ConfigFile. 
     */
    void notifyListeners(juce::String key);

    /**
     * Loads all initial configuration data from the JSON config file. This 
     * checks for all expected data keys, and replaces any missing or invalid
     * values with ones from the default config file. ConfigFile subclasses
     * should call this once, after they load any custom object or array data.
     */
    void loadJSONData();
    
    /**
     * Sets a configuration data value back to its default setting.  If this
     * changes the value, listeners will be notified and changes will be saved.
     * 
     * @param key       A key value defined in the config file.
     * 
     * @throws BadKeyException   If the key parameter was not a valid key string
     *                           for this ConfigFile.
     */
    virtual void restoreDefaultValue(juce::String key);

    /**
     * Restores all values in the configuration file to their defaults. All 
     * updated values will notify their Listeners and be written to the JSON
     * file.
     */
    virtual void restoreDefaultValues();
    
    /**
     * Checks if a key string is valid for this ConfigFile.
     * 
     * @param key  A key string value to check.
     * 
     * @return  True iff the key is valid for this file.
     */
    virtual bool isValidKey(const juce::String& key) const;
    
    /**
     * Defines the basic data types that can be stored in all ConfigFile
     * objects.
     */
    enum SupportedDataType
    {
        stringType,
        intType,
        boolType,
        doubleType
    };

    /**
     * Used for tracking which data type is associated with a key.
     */
    struct DataKey
    {
        juce::String keyString;
        SupportedDataType dataType;
    }; 
    
    /**
     * Get the set of all basic (non-array, non-object) properties tracked by
     * this ConfigFile.
     * 
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<DataKey>& getDataKeys() const = 0;

    
    /**
     * Checks for an expected property value in the JSON config data.  If the
     * value is not found or has an invalid type, the property will be copied
     * from the default config file.
     * 
     * @param key  The key string of a parameter that should be defined in this
     *             config file.
     * 
     * @tparam T   The expected type of the JSON property.
     * 
     * @return  The initialized property value.
     * 
     * @throws JSONFile::TypeException  If any data value is missing or has an
     *                                  incorrect type in both the config file 
     *                                  and the default config file.
     */
    template<typename T> T initProperty(const juce::String& key)
    {
        try
        {
            if(!configJson.propertyExists<T>(key))
            {
                DBG("ConfigFile::" << __func__ << ": Key \"" << key
                        << "\" not found in " << filename 
                        << ", checking default config file");
                if(!defaultJson.propertyExists<T>(key))
                {
                    DBG("ConfigFile::" << __func__ << ": Key \"" << key 
                            << "\" missing in config and default config!");
                }
                T property = defaultJson.getProperty<T>(key);
                configJson.setProperty<T>(key, property);
                return property;
            }
            return configJson.getProperty<T>(key);
        }
        catch (JSONFile::FileException e)
        {
            //Failed to read from .json file
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigFile::" << __func__ 
                    << ": Failed to load default value for " 
                    << e.getPropertyKey()
                    <<", expected type:" << e.getExpectedType()
                    <<", actual type: "  << e.getFoundType()
                    << ", error = " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        return T();
    }
    
    /**
     * Updates a property in the JSON config file.  This will not check
     * key validity, immediately write any changes, or notify listeners.
     * 
     * @param key        The key string that maps to the value being updated.
     * 
     * @param newValue   The new value to save to the file.
     * 
     * @tparam T         The value data type.
     * 
     * @return  True if JSON data changed, false if newValue was identical to
     *          the old value with the same key.
     */
    template<typename T>
    bool updateProperty(juce::String key, T newValue)
    {
        try
        {
            return configJson.setProperty<T>(key, newValue);
        }
        catch (JSONFile::FileException e)
        {
            //Failed to write to .json file
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        return false;
    }

    /**
     * Re-writes all data back to the config file, as long as there are
     * changes to write.
     * 
     * @pre Any code calling this function is expected to have already
     *      acquired the ConfigFile's lock
     */
    void writeChanges();

private:
    
    /**
     * If ConfigFiles define custom object or array data types, they must
     * override this method to write that custom data back to the JSONFile
     * object. 
     */
    virtual void writeDataToJSON() { }
    
    //The name of this JSON config file.
    juce::String filename;
    
    //The directory where all config files will be located
    static const constexpr char* configPath = "~/.pocket-home/";
    
    //The pocket-home asset folder subdirectory containing default config files.
    static const constexpr char* defaultAssetPath = "configuration/";
    
    //Holds configuration values read from the file.
    JSONFile configJson;
    
    //Holds default config file values
    JSONFile defaultJson; 
    
    //Used to send error messages to the user
    ConfigAlertWindows alertWindows;

    //All listeners and pending notifications for listeners
    std::map<juce::String, juce::Array<Listener*>> keyListeners;
    std::map<juce::String, juce::Array<Listener*>> notificationQueue;

    //prevents concurrent access to listeners.
    juce::CriticalSection listenerLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConfigFile)
};
