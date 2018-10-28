#pragma once
#include "FileResource.h"
#include "ResourceHandler.h"
#include "Config.h"

/**
 * @file FileHandler.h
 *
 * @brief  Controls access to a FileResource object, managing all reading from 
 *         and writing to a single JSON configuration file.
 *
 * @tparam ResourceType  The FileResource subclass this FileHandler will access.
 *                         Required class members:
 *                         
 *                         static const juce::Identifier resourceKey;
 *                         (holding the SharedResource object key)
 */
template<class ResourceType>
class Config::FileHandler : public ResourceHandler<ResourceType>
{
protected:
    FileHandler() : ResourceHandler<ResourceType>(
            ResourceType::resourceKey,
            []()->SharedResource*
            {
                return new ResourceType();
            }) { }

public:
    virtual ~FileHandler() { }

    /**
     * @brief  Gets one of the values stored in the JSON configuration file.
     *
     * @tparam ValueType                    The type of value to retrieve.
     *
     * @param key                           The key identifying the desired 
     *                                      value.
     *
     * @return                              The requested configuration value.
     *
     * @throws FileResource::FileException  If the file is missing or could not 
     *                                      be read.
     *
     * @throws FileResource::TypeException  If no property with this key and 
     *                                      type ValueType was found.
     */
    template<typename ValueType>
    ValueType getConfigValue(const juce::Identifier& key) const
    {
        typename ResourceHandler<ResourceType>::LockedResourcePtr jsonPtr 
            = ResourceHandler<ResourceType>::getReadLockedResource();
        return jsonPtr->template getConfigValue<ValueType>(key);
    }
    
    /**
     * @brief  Sets a value stored in the JSON configuration file.
     * 
     * @param key                           The key used to select the value.
     * 
     * @param newValue                      A new value to store in the JSON 
     *                                      object.
     * 
     * @tparam ValueType                    The type of value being stored.
     * 
     * @return                              True if the value changed, false if 
     *                                      the existing value matched the new 
     *                                      value.
     * 
     * @throws FileResource::FileException  If the file is missing or could not 
     *                                      be read.
     * 
     * @throws FileResource::TypeException  If a property exists that shares 
     *                                      this key but is not of type 
     *                                      ValueType.
     */
    template<typename ValueType > 
    bool setConfigValue(const juce::Identifier& key, ValueType newValue)
    {
        typename ResourceHandler<ResourceType>::LockedResourcePtr jsonPtr 
            = ResourceHandler<ResourceType>::getWriteLockedResource();
        return jsonPtr->template setConfigValue<ValueType>(key, newValue);
    }
};
