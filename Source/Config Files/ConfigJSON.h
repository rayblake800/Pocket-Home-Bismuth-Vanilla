#pragma once
#include <map>
#include "SharedResource.h"
#include "ResourceHandler.h"
#include "JSONFile.h"
#include "ConfigKey.h"
#include "JuceHeader.h"
#include "ConfigAlertWindows.h"

/**
 * @file ConfigJSON.h
 * 
 * @brief Reads and writes data from a JSON configuration file. 
 * 
 *  ConfigJSON provides an abstract base for classes that read and write JSON 
 * configuration files.  Each ConfigJSON subclass is responsible for a single
 * JSON file containing data that can be altered by the user.  Along with 
 * reading and writing data, ConfigJSON allows objects to be defined as listener
 * objects, which will receive notifications whenever data keys they select are
 * changed.
 * 
 * As an implementation of the SharedResource class, each ConfigJSON subclass
 * will have only one object instance at a time, to prevent concurrent access
 * to the JSON file, and limit the amount of file I/O necessary.  Each
 * ConfigJSON subclass should be accessed only through a ConfigFile subclass
 * specific to that ConfigJSON.
 *
 * A default version of each ConfigJSON's JSON resource file should be placed in
 * the configuration subdirectory of the asset folder.  Any missing or invalid 
 * parameters in config files will be replaced with values from the default 
 * file.
 *
 * ConfigJSON reads from each JSON file only once per program instance, so any 
 * external changes to the file that occur while the program is running will
 * most likely be ignored and may be overwritten.
 */
class ConfigJSON : public SharedResource
{
protected:
    /**
     * This constructor should only be called when constructing ConfigJSON
     * subclasses.
     *
     * @param resourceKey     The SharedResource object key for the specific
     *                        ConfigJSON variant being created.
     * 
     * @param configFilename  The name of a JSON file to read or create in
     *                        the config file directory. There should be a file
     *                        with the same name in the asset folder filled with
     *                        default values.
     */
    ConfigJSON(const juce::Identifier& resourceKey,
            const juce::String& configFilename);

public:
    /**
     * Writes any pending changes to the file before destruction.
     */
    virtual ~ConfigJSON();

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
     *                                   key string for this ConfigJSON.
     * 
     * @throws JSONFile::TypeException   If the key does not map to a value of 
     *                                   type T in this config file.
     */
    template<typename T > T getConfigValue(const juce::Identifier& key)
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
            DBG("ConfigJSON::" << __func__ 
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
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        return T();
    }

    /**
     * Sets one of this ConfigJSON's values, notifying listeners and writing to
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
     *                                   key string for this ConfigJSON.
     */
    template<typename T>
    void setConfigValue(const juce::Identifier& key, T newValue)
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
     * Sets a configuration data value back to its default setting.  If this
     * changes the value, listeners will be notified and changes will be saved.
     * 
     * @param key       A key value defined in the config file.
     * 
     * @throws BadKeyException   If the key parameter was not a valid key string
     *                           for this ConfigJSON.
     */
    virtual void restoreDefaultValue(const juce::Identifier& key);

    /**
     * Restores all values in the configuration file to their defaults. All 
     * updated values will notify their Listeners and be written to the JSON
     * file.
     */
    virtual void restoreDefaultValues();
     
    /**
     * Signals an attempt to access an invalid config value in a ConfigJSON.
     */
    struct BadKeyException : public std::exception
    {
    public:
        /**
         * @param invalidKey  The invalid key string that caused this exception.
         */
        BadKeyException(const juce::Identifier& invalidKey) : 
        invalidKey(invalidKey) { }
        
        /**
         * Gets the invalid key that caused the exception.
         * 
         * @return  The unexpected key value.
         */
        const juce::Identifier& getInvalidKey()
        {
            return invalidKey;
        }
    private:
        const juce::Identifier& invalidKey;
    };

    /**
     * Listeners receive updates whenever key values they track are updated in
     * a config file.  Listeners may track any number of keys in any number of
     * ConfigJSON objects.
     */
    class Listener : private ResourceHandler<ConfigJSON>
    {
    protected:
        friend class ConfigJSON;

        /**
         * @param configJSONKey  The key string identifying this listener's 
         *                       specific ConfigJSON object.
         *
         * @param initJSON       A function that will initialize the ConfigJSON
         *                       if it does not yet exist.
         */
        Listener(const juce::Identifier& configJSONKey,
                const std::function<SharedResource*()> initJSON);

        virtual ~Listener() { }

        /**
         * Calls configValueChanged() for every key tracked by this listener.
         */
        void loadAllConfigProperties();

    public:
        /**
         * Subscribe to receive updates on a ConfigJSON value.
         *
         * @param keyToTrack  Whenever a value with this key is updated, the
         *                    Listener will be notified.
         */
        void subscribeToKey(const juce::Identifier& keyToTrack);

        /**
         * Unsubscribe from updates on a ConfigJSON value.
         *
         * @param keyToRemove  This Listener will no longer receive updates when
         *                     the value with this key is updated.
         */
        void unsubscribeFromKey(const juce::Identifier& keyToRemove);

    private:
        /**
         * Called whenever a key tracked by this listener changes in the config
         * file. Listeners should implement this to handle config changes.
         * 
         * @param propertyKey   Passes in the updated value's key.
         */
        virtual void configValueChanged
        (const juce::Identifier& propertyKey) = 0;

        //Tracks all keys this listener follows.
        juce::Array<juce::Identifier, juce::CriticalSection> subscribedKeys;
    };

protected:
    /**
     * Announce new changes to each object tracking a particular key.
     * 
     * @param key  Maps to a value that has changed in this ConfigJSON. 
     */
    void notifyListeners(const juce::Identifier& key);

    /**
     * Loads all initial configuration data from the JSON config file. This 
     * checks for all expected data keys, and replaces any missing or invalid
     * values with ones from the default config file. ConfigJSON subclasses
     * should call this once, after they load any custom object or array data.
     */
    void loadJSONData();

    /**
     * Checks if a key string is valid for this ConfigJSON.
     * 
     * @param key  A key string value to check.
     * 
     * @return  True iff the key is valid for this file.
     */
    virtual bool isValidKey(const juce::Identifier& key) const;
    
    /**
     * Get the set of all basic (non-array, non-object) properties tracked by
     * this ConfigJSON.
     * 
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<ConfigKey>& getConfigKeys() const = 0;

    
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
    template<typename T> T initProperty(const juce::Identifier& key)
    {
        try
        {
            if(!configJson.propertyExists<T>(key))
            {
                DBG("ConfigJSON::" << __func__ << ": Key \"" << key
                        << "\" not found in " << filename 
                        << ", checking default config file");
                if(!defaultJson.propertyExists<T>(key))
                {
                    DBG("ConfigJSON::" << __func__ << ": Key \"" << key 
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
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigJSON::" << __func__ 
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
    bool updateProperty(const juce::Identifier& key, T newValue)
    {
        try
        {
            return configJson.setProperty<T>(key, newValue);
        }
        catch (JSONFile::FileException e)
        {
            //Failed to write to .json file
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        return false;
    }

    /**
     * Re-writes all data back to the config file, as long as there are
     * changes to write.
     * 
     * @pre Any code calling this function is expected to have already
     *      acquired the ConfigJSON's lock
     */
    void writeChanges();

private:
    
    /**
     * If ConfigJSONs define custom object or array data types, they must
     * override this method to write that custom data back to the JSONFile
     * object. 
     */
    virtual void writeDataToJSON() { }
    
    //The name of this JSON config file.
    const juce::String filename;
    
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConfigJSON)
};
