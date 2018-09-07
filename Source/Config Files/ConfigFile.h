#pragma once
#include "ConfigJSON.h"
#include "ResourceHandler.h"

/**
 * @file ConfigFile.h
 *
 * @brief  Controls access to a ConfigJSON object, managing all reading from and
 *         writing to a single JSON configuration file.
 *
 * @tparam ConfigJSONType  The ConfigJSON subclass this ConfigFile will access.
 *                         Required class members:
 *                         
 *                         static const juce::Identifier resourceKey;
 *                         (holding the SharedResource object key)
 */
template<class ConfigJSONType>
class ConfigFile : public ResourceHandler<ConfigJSONType>
{
protected:
    ConfigFile() : ResourceHandler<ConfigJSONType>(
            ConfigJSONType::resourceKey,
            []()->SharedResource*
            {
                return new ConfigJSONType();
            }) { }

public:
    virtual ~ConfigFile() { }

    /**
     * Gets one of the values stored in the JSON configuration file.
     *
     * @tparam ValueType  The type of value to retrieve.
     *
     * @param key         The key identifying the desired value.
     *
     * @return  The requested configuration value.
     *
     * @throws ConfigJSON::FileException  If the file is missing or could not be
     *                                    read.
     *
     * @throws ConfigJSON::TypeException  If no property with this key and type
     *                                    ValueType was found.
     */
    template<typename ValueType>
    ValueType getConfigValue(const juce::Identifier& key)
    {
        auto jsonPtr = getReadLockedResource();
        return jsonPtr->getConfigValue<ValueType>(key);
    }
    
    /**
     * Sets a property stored in this JSON data.
     * 
     * @param key         The key string used to set the value.
     * 
     * @param newValue    A new value to store in the JSON object.
     * 
     * @tparam ValueType  The type of value being stored.
     * 
     * @return  True if the property changed, false if the existing value
     *          matched the new value.
     * 
     * @throws ConfigJSON::FileException   If the file is missing or could not 
     *                                     be read.
     * 
     * @throws ConfigJSON::TypeException  If a property exists that shares this 
     *                                    key but is not of type ValueType.
     */
    template<typename ValueType > 
    bool setProperty(const juce::Identifier& key, ValueType newValue)
    {
        auto jsonPtr = getWriteLockedResource();
        return jsonPtr->setConfigValue<ValueType>(key, newValue);
    }


};
