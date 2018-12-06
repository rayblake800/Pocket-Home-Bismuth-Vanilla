#pragma once
/**
 * @file  Config/FileResource.h
 * 
 * @brief Reads and writes data from a JSON configuration file. 
 */

#include "Config/Implementation/ListenerInterface.h"
#include "SharedResource/Resource.h"
#include "SharedResource/Handler.h"
#include "JSONFile.h"
#include "DataKey.h"
#include "JuceHeader.h"
#include <map>

namespace Config { class FileResource; }
namespace Config { struct DataKey; }

/**
 *  FileResource provides an abstract base for classes that read and write JSON 
 * configuration files. Each FileResource subclass is responsible for a single
 * JSON file containing data that can be altered by the user. Along with 
 * reading and writing data, FileResource allows objects to be defined as 
 * listener objects, which will receive notifications whenever data keys they 
 * select are changed.
 * 
 *  As an implementation of the SharedResource class, each FileResource subclass
 * will have only one object instance at a time, to prevent concurrent access
 * to the JSON file, and limit the amount of file I/O necessary. Each
 * FileResource subclass should be accessed only through a ConfigFile subclass
 * specific to that FileResource.
 *
 *  A default version of each FileResource's JSON resource file should be placed 
 * in the configuration subdirectory of the asset folder. Any missing or 
 * invalid parameters in config files will be replaced with values from the 
 * default file.
 *
 *  FileResource reads from each JSON file only once per program instance, so 
 * any external changes to the file that occur while the program is running will
 * most likely be ignored and may be overwritten.
 */
class Config::FileResource : public SharedResource::Resource
{
protected:
    /**
     * @brief Creates the FileResource resource, and prepares to read JSON data.
     *
     * @param resourceKey     The SharedResource object key for the specific
     *                        FileResource variant being created.
     * 
     * @param configFilename  The name of a JSON file to read or create in
     *                        the config file directory. There should be a file
     *                        with the same name in the asset folder filled with
     *                        default values.
     */
    FileResource(const juce::Identifier& resourceKey,
            const juce::String& configFilename);

public:
    /**
     * @brief  Writes any pending changes to the file before destruction.
     */
    virtual ~FileResource();

    /**
     * @brief  Gets one of the values stored in the JSON configuration file.
     * 
     * @param key                       The key string that maps to the desired 
     *                                  value.
     * 
     * @tparam ValueType                The value's data type.
     * 
     * @throws BadKeyException          If the key parameter was not a valid 
     *                                  key string for this FileResource.
     * 
     * @throws JSONFile::TypeException  If the key does not map to a value of 
     *                                  type ValueType in this config file.
     * 
     * @return                          The value read from the config file.
     */
    template<typename ValueType >
    ValueType getConfigValue(const juce::Identifier& key)
    {      
        using namespace juce;
        if(!isValidKey(key))
        {
            throw BadKeyException(key);
        }
        try
        {
            return configJson.getProperty<ValueType>(key);
        }
        catch(JSONFile::TypeException e)
        {
            DBG("FileResource::" << __func__ 
                    << ": Failed to load key \"" 
                    << e.getPropertyKey()
                    <<"\" , expected type:" << e.getExpectedType()
                    <<", actual type: "  << e.getFoundType()
                    << ", error = " << e.what());
            throw e;
        }
        catch (JSONFile::FileException e)
        {
            //Failed to access .json file
            DBG("FileResource::" << __func__ << ": " << e.what());
        }
        return ValueType();
    }

    /**
     * @brief  Sets one of this FileResource's values, notifying listeners and 
     *         writing to the JSON file if the value is changed.
     * 
     * @param key               The key string that maps to the value being 
     *                          updated.
     * 
     * @param newValue          The new value to save to the file.
     * 
     * @tparam ValueType        The value data type.
     * 
     * @throws BadKeyException  If the key parameter was not a valid key string
     *                          for this FileResource.
     *
     * @return                  True if the value changed, false if the new
     *                          value matched the old value.
     */
    template<typename ValueType>
    bool setConfigValue(const juce::Identifier& key, ValueType newValue)
    {
        using namespace juce;
        if(!isValidKey(key))
        {
            throw BadKeyException(key);
        }
        if(updateProperty<ValueType>(key, newValue))
        {
            configJson.writeChanges();
            foreachHandler<ListenerInterface>(
                [this, &key] (ListenerInterface* listener)
            {
                if(listener->isKeyTracked(key))
                {
                    listener->configValueChanged(key);
                }
            });
            return true;
        }
        return false;
    }
     
    /**
     * @brief  Sets a configuration data value back to its default setting. 
     *
     * If this changes the value, listeners will be notified and changes will be
     * saved.
     * 
     * @param key                A key value defined in the config file.
     * 
     * @throws BadKeyException   If the key parameter was not a valid key string
     *                           for this FileResource.
     */
    virtual void restoreDefaultValue(const juce::Identifier& key);

    /**
     * @brief  Restores all values in the configuration file to their defaults. 
     *
     * All updated values will notify their Listeners and be written to the JSON
     * file.
     */
    virtual void restoreDefaultValues();
     
    /**
     * Signals an attempt to access an invalid config value in a FileResource.
     */
    struct BadKeyException : public std::exception
    {
    public:
        /**
         * @param invalidKey  The invalid key string that caused this exception.
         */
        BadKeyException(const juce::Identifier& invalidKey) : 
        invalidKey(invalidKey) { }
        
        virtual const char* what() const noexcept override
        {
            return juce::CharPointer_UTF8(invalidKey);
        }
        
        /**
         * @brief  Gets the invalid key that caused the exception.
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

protected:
    /**
     * @brief  Loads all initial configuration data from the JSON config file. 
     *
     *  This checks for all expected data keys, and replaces any missing or 
     * invalid values with ones from the default config file. FileResource 
     * subclasses should call this once, after they load any custom object or 
     * array data.
     */
    void loadJSONData();

    /**
     * @brief  Checks if a key string is valid for this FileResource.
     * 
     * @param key  A key string value to check.
     * 
     * @return     Whether the key is valid for this file.
     */
    virtual bool isValidKey(const juce::Identifier& key) const;
    
    /**
     * Get the set of all basic (non-array, non-object) properties tracked by
     * this FileResource.
     * 
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<DataKey>& getConfigKeys() const = 0;

    
    /**
     * @brief  Checks for an expected property value in the JSON config data.  
     *
     *  If the value is not found or has an invalid type, the property will be 
     * copied from the default config file.
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
                DBG("FileResource::" << __func__ << ": Key \"" << key
                        << "\" not found in " << filename 
                        << ", checking default config file");
                if(!defaultJson.propertyExists<T>(key))
                {
                    DBG("FileResource::" << __func__ << ": Key \"" << key 
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
            DBG("FileResource::" << __func__ << ": " << e.what());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("FileResource::" << __func__ 
                    << ": Failed to load default value for " 
                    << e.getPropertyKey()
                    <<", expected type:" << e.getExpectedType()
                    <<", actual type: "  << e.getFoundType()
                    << ", error = " << e.what());
        }
        return T();
    }
    
    /**
     * @brief  Updates a property in the JSON config file.  
     *
     * This will not check key validity, immediately write any changes, or 
     * notify listeners.
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
            DBG("FileResource::" << __func__ << ": " << e.what());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("FileResource::" << __func__ << ": " << e.what());
        }
        return false;
    }

    /**
     * @brief  Re-writes all data back to the config file, as long as there are
     *         changes to write.
     */
    void writeChanges();

private:
     
    /**
     * @brief  Sets a configuration data value back to its default setting, 
     *         notifying listeners if the value changes.
     *
     * This does not ensure that the key is valid for this ConfigFile.  An
     * AlertWindow will be shown if any problems occur while accessing JSON
     * data.
     *
     * @param key  The key of the value that will be restored.
     */
    void restoreDefaultValue(const DataKey& key);
    
    /**
     * @brief  Writes any custom object or array data back to the JSON file.
     *
     * FileResource subclasses with custom object or array data must override 
     * this method to write that data back to the file.
     */
    virtual void writeDataToJSON() { }
    
    /* The name of this JSON config file. */
    const juce::String filename;

    /* Holds configuration values read from the file. */
    JSONFile configJson;
    
    /* Holds default config file values. */
    JSONFile defaultJson; 
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileResource)
};
